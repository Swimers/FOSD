#include "WorkbenchRuntime.h"
#include <QFileInfo>
#include <cmath>
namespace fosd {
namespace application {
using namespace ui::classic;
using namespace communication;
void WorkbenchRuntime::transportState(Channel channel, ConnectionState state) {
    m_states[int(channel)] = state;
    if (channel == Channel::Tcp && (state == ConnectionState::Closed || state == ConnectionState::Listening))
        m_logTcpPeer = {};
    if(state==ConnectionState::Closed) { m_history.clear(); m_historyClock.restart(); }
    if (state == ConnectionState::Closed && m_baselineClock.isValid()) { m_baselineClock.invalidate(); notice(tr("置零取消：通信已关闭")); }
    const bool serial = channel == Channel::Serial;
    if (!serial && channel != m_networkChannel)
        return;
    const bool closed = state == ConnectionState::Closed;
    emit presentationChanged(PanelId::Connection, {{serial ? "pbtn_OpenSerial" : "pbtn_OpenNet",
                                                    closed ? tr("打开") : tr("关闭")}});
    notice(tr("%1 状态：%2")
               .arg(serial ? tr("串口") : channel == Channel::Udp ? "UDP" : "TCP")
               .arg(int(state)));
}

void WorkbenchRuntime::rawReceived(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    if (channel == Channel::Tcp && peer.isValid())
        m_logTcpPeer = peer;
    if (channel == Channel::Udp) {
        m_udpPreview = bytes.left(64);
        m_udpPacketBytes = bytes.size();
        m_udpPeer = peer;
        m_udpReceivedAt = QDateTime::currentDateTime();
        ++m_udpPacketCount;
        m_udpLogFrame.reset();
        m_udpLogStatus.clear();
        m_udpLogPending = false;
        return; // The production parser/decoder supplies the summary or rejection.
    }
    appendTraffic(channel, bytes, peer, false);
}

void WorkbenchRuntime::rawSent(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    if (channel == Channel::Udp)
        flushUdpTraffic();
    appendTraffic(channel, bytes, peer, true);
}

void WorkbenchRuntime::appendTraffic(Channel channel, const QByteArray &bytes, const Endpoint &peer,
                                   bool outgoing, const QString &summary, int byteCount,
                                   bool showPayload, const QDateTime &at) {
    const int i = int(channel);
    TrafficEntry entry;
    entry.id = ++m_nextTrafficId[i];
    entry.at = at.isValid() ? at : QDateTime::currentDateTime();
    entry.outgoing = outgoing;
    entry.byteCount = byteCount < 0 ? bytes.size() : byteCount;
    entry.preview = bytes.left(64);
    entry.summary = summary;
    entry.showPayload = showPayload;
    if (outgoing || channel == Channel::Udp)
        entry.utf8 = QString::fromUtf8(bytes).left(64);
    else {
        if (!m_receiveDecoder[i])
            m_receiveDecoder[i].reset(QTextCodec::codecForName("UTF-8")->makeDecoder());
        // Decode all bytes to preserve UTF-8 state across serial/TCP reads, but retain a short preview.
        entry.utf8 = m_receiveDecoder[i]->toUnicode(bytes).left(64);
    }
    auto endpoint = [](const Endpoint &value) {
        return value.address.toString() + ':' + QString::number(value.port);
    };
    if (channel == Channel::Serial) {
        entry.endpoint = tr("串口 %1 @%2")
            .arg(m_logSerial.portName.isEmpty() ? tr("未指定") : m_logSerial.portName)
            .arg(m_logSerial.baudRate);
    } else if (channel == Channel::Udp) {
        const auto remote = peer.isValid() ? peer : m_logUdp.defaultPeer;
        const QString local = m_logUdp.bindAddress.toString() + ':' +
            (m_logUdp.localPort ? QString::number(m_logUdp.localPort) : tr("自动端口"));
        entry.endpoint = outgoing ? local + " -> " + endpoint(remote)
                                  : endpoint(remote) + " -> " + local;
    } else {
        const QString local = m_logTcp.role == TcpRole::Server
            ? m_logTcp.listenAddress.toString() + ':' + QString::number(m_logTcp.port)
            : tr("本地系统分配端口");
        const QString remote = peer.isValid() ? endpoint(peer)
            : m_logTcp.role == TcpRole::Client ? m_logTcp.host + ':' + QString::number(m_logTcp.port)
            : m_logTcpPeer.isValid() ? endpoint(m_logTcpPeer) : tr("客户端端口待接收数据确认");
        entry.endpoint = outgoing ? local + " -> " + remote : remote + " -> " + local;
    }
    auto &entries = m_traffic[i];
    entries.append(entry);
    while (entries.size() > 200)
        entries.removeFirst();
}

QString WorkbenchRuntime::formatTraffic(int channel, const TrafficEntry &entry) const {
    const char *names[] = {"SERIAL", "UDP", "TCP"};
    QString line = QStringLiteral("[%1] %2 %3 | %4 | %5 B")
        .arg(entry.at.toString("yyyy-MM-dd HH:mm:ss.zzz"), names[channel],
             entry.outgoing ? tr("TX 发送请求") : tr("RX 接收"), entry.endpoint)
        .arg(entry.byteCount);
    if (!entry.summary.isEmpty())
        line += " | " + entry.summary;
    if (entry.showPayload) {
        const QString payload = m_rawHex[channel]
            ? QString::fromLatin1(entry.preview.toHex(' ').toUpper())
            : QString(entry.utf8).replace('\r', "\\r").replace('\n', "\\n")
                                .replace(QChar(0x2028), "\\u2028").replace(QChar(0x2029), "\\u2029");
        line += tr(" | 预览%1：%2").arg(m_rawHex[channel] ? "Hex" : "UTF-8", payload);
        if (entry.byteCount > entry.preview.size())
            line += tr(" …（预览已截断）");
    }
    return line;
}

QString WorkbenchRuntime::udpLogEntry() const {
    QString line = tr("累计=%1").arg(m_udpPacketCount);
    if (m_udpLogFrame) {
        line += " | " + m_udpLogFrame->protocolId;
        const auto wavelengths = std::dynamic_pointer_cast<const core::WavelengthFrame>(m_udpLogFrame->data);
        if (wavelengths) {
            line += tr(" | 序号=%1 | 版本=%2 类型=%3 | 记录=%4")
                .arg(wavelengths->sequence).arg(wavelengths->version)
                .arg(wavelengths->dataType).arg(wavelengths->records.size());
            if (!wavelengths->records.isEmpty()) {
                const auto &first = wavelengths->records.first();
                line += tr(" | 首条=CH%1/%2 %3 nm")
                    .arg(first.channel).arg(first.sensor).arg(first.wavelength, 0, 'g', 9);
            }
        }
    } else
        line += tr(" | 未识别/无效包：%1").arg(m_udpLogStatus.left(160));
    return line;
}

void WorkbenchRuntime::flushUdpTraffic() {
    if (!m_udpLogPending)
        return;
    m_udpLogPending = false;
    appendTraffic(Channel::Udp, m_udpPreview, m_udpPeer, false, udpLogEntry(),
                  m_udpPacketBytes, !m_udpLogFrame, m_udpReceivedAt);
}

void WorkbenchRuntime::clearTraffic(int channel) {
    m_traffic[channel].clear();
    m_publishedTrafficId[channel] = 0;
    m_resetTraffic[channel] = true;
    m_receiveDecoder[channel].reset();
    if (channel == int(Channel::Udp)) {
        m_udpLogFrame.reset();
        m_udpPreview.clear();
        m_udpLogStatus.clear();
        m_udpPacketBytes = 0;
        m_udpPacketCount = 0;
        m_udpLogPending = false;
    }
}

void WorkbenchRuntime::clearReceiveLogs() {
    for (int i = 0; i < 3; ++i)
        clearTraffic(i);
    emit presentationChanged(PanelId::Connection,
        {{"ptedit_SerialRec", QString()}, {"ptedit_UdpRec", QString()}, {"ptedit_TcpRec", QString()}});
}

bool WorkbenchRuntime::sendSerialCommand(const QString &command, QString *error) {
    auto bytes = command.toUtf8();
    while (bytes.endsWith('\r') || bytes.endsWith('\n'))
        bytes.chop(1);
    if (bytes.trimmed().isEmpty()) {
        *error = tr("请输入串口发送指令");
        return false;
    }
    if (m_states[int(Channel::Serial)] != ConnectionState::Connected) {
        *error = tr("请先打开串口");
        return false;
    }
    bytes.append("\r\n");
    m_protocol->communication().send(Channel::Serial, bytes);
    return true;
}

bool WorkbenchRuntime::startStorage(const QString &path, QString *error) {
    if (m_recording || m_storagePending) {
        if (error)
            *error = tr("已有存储会话正在运行");
        return false;
    }
    if (path.trimmed().isEmpty()) {
        if (error)
            *error = tr("请选择输出文件");
        return false;
    }
    storage::StorageConfiguration configuration;
    configuration.path = path;
    const auto suffix = QFileInfo(path).suffix().toLower();
    if (suffix != "csv" && suffix != "db" && suffix != "sqlite") {
        if (error)
            *error = tr("请使用 .csv、.sqlite 或 .db 扩展名");
        return false;
    }
    configuration.format =
        suffix == "csv" ? storage::StorageFormat::Csv : storage::StorageFormat::SQLite;
    configuration.dataTypes = storage::DataType::Wavelength |
                              storage::DataType::CalibratedHardware |
                              storage::DataType::PointDisplacement;
    configuration.allPoints = true;
    m_storagePending = true;
    emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic",true},{"pbtn_SaveDynamic.enabled",false}});
    m_calculation->storage().start(configuration);
    return true;
}

bool WorkbenchRuntime::connectionAction(const UiRequest &request, UiResponse *response) {
    if (request.panel != PanelId::Connection)
        return false;
    const auto &action = request.action;
    const auto &fields = request.fields;
    auto &communication = m_protocol->communication();
    response->success = true;
    response->message = tr("请求已提交");
    if (action == "pbtn_CalculationStart" || action == "pbtn_CalculationStop") {
        const bool start = action == "pbtn_CalculationStart";
        if (start && (m_configurationPending || !m_configurationRevision)) { response->success=false; response->message=tr("请先完成置零或读取有效配置"); return true; }
        m_solving = start;
        if (start) {
            const auto latest = m_protocol->dataModel()->latest();
            m_runStartFrameRevision = latest ? latest->revision : 0;
        }
        m_calculation->setRunning(m_solving);
        if (!m_solving) { m_visualization->setResultRepository(nullptr); m_result.reset(); }
        response->message=m_solving ? tr("解算已启动，等待有效数据") : tr("解算已停止，通信继续");
        response->fields["pbtn_CalculationStart.enabled"] = !m_solving;
        response->fields["pbtn_CalculationStop.enabled"] = m_solving;
        notice(response->message);
    } else if (action == "pbtn_ConfigRead") {
        Calculation::CalculationConfiguration c;
        response->success=m_parameters.read(&c,true,&response->message) && configure(c,&response->message);
    } else if (action == "pbtn_ChannelCalibration") {
        publishCalibration();
    } else if (action == "pbtn_RefreshLocalAddress") {
        initializeLocalAddresses(text(fields, "cbox_LocakIP"));
        response->message = tr("本地 IPv4 地址已刷新");
    } else if (action == "pbtn_RefreshSerial") {
        const auto serial = fields.value("cbox_SerialNameChoose").toMap();
        initializePorts(serial.value("data", serial.value("text")).toString().section(':', 0, 0),
                        text(fields, "cbox_LocakIP"));
        response->message = tr("串口与本地网口已刷新");
    } else if (action == "cbox_SerialNameChoose" || action == "cbox_SerialBaudChoose" ||
             action == "cbox_LocakIP")
        response->message = tr("参数将在下次打开通道时生效");
    else if (action == "cbox_CommuteChoose") {
        if (m_states[1] != ConnectionState::Closed || m_states[2] != ConnectionState::Closed) {
            response->success = false;
            response->message = tr("请先关闭网络连接再切换方式");
            response->fields["cbox_CommuteChoose"] =
                QVariantMap{{"index", m_networkChannel == Channel::Udp ? 0 : m_tcpRole == TcpRole::Server ? 1 : 2}};
        } else {
            const int mode = index(fields, "cbox_CommuteChoose");
            if (mode < 0 || mode > 2) {
                response->success = false;
                response->message = tr("请选择 UDP、TCP Server 或 TCP Client");
                return true;
            }
            config::IniConfigStore store(config::ParameterStore::runtimePath());
            response->success = store.writeValue("classic.connection", "networkMode", mode, &response->message);
            if (response->success)
                publishNetworkConfiguration(mode);
        }
    } else if (action == "pbtn_OpenSerial") {
        if (m_states[0] != ConnectionState::Closed)
            communication.close(Channel::Serial);
        else {
            SerialConfig configuration;
            QString ignored;
            m_parameters.read(&configuration, false, &ignored);
            const auto serial = fields.value("cbox_SerialNameChoose").toMap();
            configuration.portName = serial.value("data").toString();
            if (configuration.portName.isEmpty())
                configuration.portName = text(fields, "cbox_SerialNameChoose").section(':', 0, 0).trimmed();
            bool ok = false;
            configuration.baudRate = text(fields, "cbox_SerialBaudChoose").toInt(&ok);
            bool bitsOk = true, parityOk = true, stopOk = true, flowOk = true;
            if (fields.contains("edit_SerialDataBits"))
                configuration.dataBits = static_cast<QSerialPort::DataBits>(text(fields, "edit_SerialDataBits").toInt(&bitsOk));
            if (fields.contains("edit_SerialParity")) {
                const QMap<QString, QSerialPort::Parity> choices{
                    {"none",QSerialPort::NoParity},{"n",QSerialPort::NoParity},{tr("无"),QSerialPort::NoParity},
                    {"odd",QSerialPort::OddParity},{"o",QSerialPort::OddParity},{tr("奇"),QSerialPort::OddParity},
                    {"even",QSerialPort::EvenParity},{"e",QSerialPort::EvenParity},{tr("偶"),QSerialPort::EvenParity},
                    {"mark",QSerialPort::MarkParity},{"space",QSerialPort::SpaceParity}};
                const auto value = text(fields,"edit_SerialParity").trimmed().toLower();
                parityOk = choices.contains(value);
                if (parityOk) configuration.parity = choices.value(value);
            }
            if (fields.contains("edit_SerialStopBits")) {
                const auto value = text(fields,"edit_SerialStopBits").trimmed();
                stopOk = value == "1" || value == "1.5" || value == "2";
                configuration.stopBits = value == "1.5" ? QSerialPort::OneAndHalfStop : value == "2" ? QSerialPort::TwoStop : QSerialPort::OneStop;
            }
            if (fields.contains("edit_SerialFlowControl")) {
                const QMap<QString, QSerialPort::FlowControl> choices{
                    {"none",QSerialPort::NoFlowControl},{tr("无"),QSerialPort::NoFlowControl},
                    {"hardware",QSerialPort::HardwareControl},{tr("硬件"),QSerialPort::HardwareControl},
                    {"software",QSerialPort::SoftwareControl},{tr("软件"),QSerialPort::SoftwareControl}};
                const auto value = text(fields,"edit_SerialFlowControl").trimmed().toLower();
                flowOk = choices.contains(value);
                if (flowOk) configuration.flowControl = choices.value(value);
            }
            if (!ok || !bitsOk || !parityOk || !stopOk || !flowOk ||
                !config::validateParameter(configuration, true, &response->message)) {
                response->success = false;
                response->message = tr("串口参数无效：请检查端口、波特率、数据位、校验位、停止位和流控");
            } else
                communication.openSerial(configuration);
        }
    } else if (action == "pbtn_OpenNet") {
        if (m_states[1] != ConnectionState::Closed || m_states[2] != ConnectionState::Closed)
            communication.close(m_networkChannel);
        else {
            const int mode = index(fields, "cbox_CommuteChoose");
            if (mode < 0 || mode > 2) {
                response->success = false; response->message = tr("通信方式无效"); return true;
            }
            const Channel channel = mode == 0 ? Channel::Udp : Channel::Tcp;
            bool portOk = false;
            const uint targetPort = text(fields, "edit_TargetPort").toUInt(&portOk);
            const QString target = text(fields, "edit_TargetIP").trimmed();
            bool valid = portOk && targetPort > 0 && targetPort <= 65535 && !target.isEmpty();
            if (channel == Channel::Udp) {
                bool localOk = false;
                const uint localPort = text(fields, "edit_LocalPort").toUInt(&localOk);
                UdpConfig configuration;
                configuration.bindAddress = QHostAddress(text(fields, "cbox_LocakIP"));
                configuration.localPort = quint16(localPort);
                configuration.defaultPeer.address = QHostAddress(target);
                configuration.defaultPeer.port = quint16(targetPort);
                valid = valid && localOk && localPort <= 65535 &&
                        !configuration.bindAddress.isNull() && configuration.defaultPeer.isValid();
                if (valid) {
                    m_networkChannel = channel;
                    communication.openUdp(configuration);
                }
            } else if (valid || mode == 1) {
                valid = true;
                TcpConfig configuration;
                configuration.role = mode == 1 ? TcpRole::Server : TcpRole::Client;
                configuration.host = target;
                if (configuration.role == TcpRole::Server) {
                    bool ok=false; uint port=text(fields,"edit_LocalPort").toUInt(&ok);
                    configuration.listenAddress=QHostAddress(text(fields,"cbox_LocakIP"));
                    if (!ok || port == 0 || port > 65535 || configuration.listenAddress.isNull()) { response->success=false; response->message=tr("监听地址或端口无效"); return true; }
                    configuration.port=quint16(port);
                } else configuration.port = quint16(targetPort);
                m_networkChannel = channel;
                m_tcpRole = configuration.role;
                communication.openTcp(configuration);
            }
            if (!valid) {
                response->success = false;
                response->message = tr("网络地址或端口无效（UDP 本地端口可填 0）");
            }
        }
    } else if (action.startsWith("edit_SerialCommand") ||
               action.startsWith("pbtn_SerialCommand")) {
        const bool send = action.startsWith("pbtn_");
        const QString prefix = send ? "pbtn_SerialCommand" : "edit_SerialCommand";
        bool valid = false;
        const int slot = action.mid(prefix.size()).toInt(&valid);
        if (!valid || slot < 0 || slot >= 10)
            return false;
        const auto command = text(fields, QStringLiteral("edit_SerialCommand%1").arg(slot));
        config::IniConfigStore store(config::ParameterStore::runtimePath());
        response->success = store.writeValue("classic.serialCommands", QString::number(slot),
                                             command, &response->message);
        if (response->success) {
            response->message = tr("快捷指令已保存");
            if (send) {
                response->success = sendSerialCommand(command, &response->message);
                if (response->success)
                    response->message = tr("指令已提交发送（末尾 CRLF）");
            }
        }
    } else if (action == "pbtn_SerialSend") {
        response->success = sendSerialCommand(text(fields, "ptedit_SerialSend"), &response->message);
        if (response->success)
            response->message = tr("指令已提交发送（末尾 CRLF）");
    } else if (action == "pbtn_UdpSend" ||
               action == "pbtn_TcpSend") {
        const Channel channel = action == "pbtn_UdpSend" ? Channel::Udp : Channel::Tcp;
        const auto input = channel == Channel::Udp ? "ptedit_UdpSend" : "ptedit_TcpSen";
        Endpoint peer;
        if (channel == Channel::Udp) {
            bool ok = false;
            const uint port = text(fields, "edit_TargetPort").toUInt(&ok);
            peer.address = QHostAddress(text(fields, "edit_TargetIP"));
            peer.port = quint16(port);
            if (!ok || port > 65535 || !peer.isValid()) {
                response->success = false;
                response->message = tr("UDP 目标地址无效");
                return true;
            }
        }
        if (m_states[int(channel)] == ConnectionState::Closed) {
            response->success = false;
            response->message = tr("请先打开通信通道");
        } else
            communication.send(channel, text(fields, input).toUtf8(), peer);
    } else if (action == "cbox_SerialDisplayMode" || action == "cbox_UdpDisplayMode" || action == "cbox_TcpDisplayMode") {
        const int channel = action == "cbox_SerialDisplayMode" ? 0 : action == "cbox_UdpDisplayMode" ? 1 : 2;
        m_rawHex[channel] = index(fields, action) == 0;
        m_resetTraffic[channel] = true;
        response->message = tr("已切换收发日志的预览格式；有效 UDP 包保留摘要");
    } else if (action == "pbtn_SerialClear" || action == "pbtn_UdpClear" || action == "pbtn_TcpClear") {
        const int channel = action == "pbtn_SerialClear" ? 0 : action == "pbtn_UdpClear" ? 1 : 2;
        clearTraffic(channel);
        const char *names[] = {"ptedit_SerialRec", "ptedit_UdpRec", "ptedit_TcpRec"};
        response->fields[names[channel]] = QString();
    } else if (action == "pbtn_WaveLengthReset")
        response->success = baseline(&response->message);
    else if (action == "pbtn_ParaSet" || action == "pbtn_ParaRest") {
        auto configuration = m_configuration;
        bool aOk = true, bOk = true;
        configuration.strainCalibration.A =
            action == "pbtn_ParaRest" ? 1.0 : text(fields, "textEdit_APara").toDouble(&aOk);
        configuration.strainCalibration.B =
            action == "pbtn_ParaRest" ? 0.0 : text(fields, "textEdit_BPara").toDouble(&bOk);
        if (!aOk || !bOk) {
            response->success = false;
            response->message = tr("A、B 必须为有效数值");
        } else
            response->success = configure(configuration, &response->message);
    } else if (action == "pbtn_SelectCH") {
        m_filterChannels = true;
        m_channels.clear();
        for (int channel = 1; channel <= 16; ++channel)
            if (fields.value(QStringLiteral("ckbox_CH%1").arg(channel)).toBool())
                m_channels.insert(channel);
        QStringList selected; for(int ch:m_channels) selected.append(QString::number(ch));
        config::IniConfigStore store(config::ParameterStore::runtimePath()); QString error;
        if(!store.writeValue("classic.display","channels",selected,&error)) {response->success=false; response->message=error;}
        m_presentedFrame = 0;
        response->message = tr("已更新曲线显示通道；波长表、解算和存储保留全部通道");
    } else if (action == "pbtn_SaveDynamic") {
        if (m_storagePending) {
            response->success = false;
            response->message = tr("请等待保存操作完成");
            response->fields["pbtn_SaveDynamic"] = true;
            response->fields["pbtn_SaveDynamic.enabled"] = false;
            return true;
        }
        if (!fields.value("pbtn_SaveDynamic").toBool()) {
            if (!m_recording) {
                response->fields["pbtn_SaveDynamic"] = false;
                return true;
            }
            m_storagePending = true;
            emit presentationChanged(PanelId::Connection, {{"pbtn_SaveDynamic",true},{"pbtn_SaveDynamic.enabled",false}});
            m_calculation->storage().stop();
        } else
            response->success = startStorage(text(fields, "filePath"), &response->message);
        if (!response->success) {
            response->fields["pbtn_SaveDynamic"] = m_recording;
            response->fields["pbtn_SaveDynamic.enabled"] = true;
        }
    } else
        return false;
    return true;
}
} // namespace application
} // namespace fosd
