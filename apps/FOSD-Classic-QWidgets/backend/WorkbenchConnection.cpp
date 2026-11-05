#include "WorkbenchRuntime.h"
#include <QFileInfo>
#include <cmath>
namespace fosd {
namespace application {
using namespace ui::classic;
using namespace communication;
void WorkbenchRuntime::transportState(Channel channel, ConnectionState state) {
    m_states[int(channel)] = state;
    if(state==ConnectionState::Closed) { m_history.clear(); m_historyClock.restart(); }
    if (state == ConnectionState::Closed && m_baselineClock.isValid()) { m_baselineClock.invalidate(); notice(tr("置零取消：通信已关闭")); }
    const bool serial = channel == Channel::Serial;
    if (!serial && channel != m_networkChannel)
        return;
    const bool closed = state == ConnectionState::Closed;
    emit presentationChanged(PanelId::Connection, {{serial ? "pbtn_OpenSerial" : "pbtn_OpenNet",
                                                    closed ? tr("打开") : tr("关闭")}});
    emit presentationChanged(PanelId::ConnectionDebug,
                             {{"edit_ConnectState", QStringLiteral("Serial=%1 UDP=%2 TCP=%3")
                                                        .arg(int(m_states[0]))
                                                        .arg(int(m_states[1]))
                                                        .arg(int(m_states[2]))}});
    notice(tr("%1 状态：%2")
               .arg(serial ? tr("串口") : channel == Channel::Udp ? "UDP" : "TCP")
               .arg(int(state)));
}

void WorkbenchRuntime::rawReceived(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    const int i = int(channel);
    if (channel == Channel::Udp) {
        // Keep only the latest short preview. Successful parsing replaces it with metadata.
        m_udpPreview = bytes.left(64);
        m_udpPacketBytes = bytes.size();
        m_udpPeer = peer;
        m_udpReceivedAt = QDateTime::currentDateTime();
        ++m_udpPacketCount;
        m_udpLogFrame.reset();
        m_udpLogStatus.clear();
        m_udpLogPending = false; // Wait for the decoder/parser result; never dump a valid packet.
        return;
    }
    if (channel == Channel::Serial || channel == Channel::Udp) {
        auto &cache = m_rawBytes[i];
        cache.append(bytes);
        if (cache.size() > 65536)
            cache = cache.right(65536);
    } else {
        auto &cache = m_rawText[i];
        cache.append(QString::fromLatin1(bytes.toHex(' ').toUpper()) + '\n');
        if (cache.size() > 65536)
            cache = cache.right(65536);
    }
    m_rawDirty[i] = true;
    // Actual widgets are refreshed at 10 Hz, independently of the receive rate.
}

QString WorkbenchRuntime::rawDisplay(int channel) const {
    if (channel == int(Channel::Udp))
        return m_udpLogLines.join('\n');
    if (channel == int(Channel::Tcp))
        return m_rawText[channel];
    const auto &bytes = m_rawBytes[channel];
    return m_rawHex[channel] ? QString::fromLatin1(bytes.toHex(' ').toUpper())
                             : QString::fromUtf8(bytes);
}

QString WorkbenchRuntime::udpLogEntry() const {
    const auto frame = m_udpLogFrame;
    const auto timestamp = frame ? frame->receivedAtUtc.toLocalTime() : m_udpReceivedAt;
    const auto address = frame ? frame->source.address : m_udpPeer.address.toString();
    const auto port = frame ? frame->source.port : m_udpPeer.port;
    QString line = tr("[%1] UDP %2:%3 | %4 B | 累计=%5 | ")
        .arg(timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz"), address)
        .arg(port).arg(m_udpPacketBytes).arg(m_udpPacketCount);
    if (frame) {
        line += frame->protocolId;
        const auto wavelengths = std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data);
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
    } else {
        line += tr("未识别/无效包：%1 | 预览%2：%3")
            .arg(m_udpLogStatus.left(160), m_rawHex[1] ? "Hex" : "UTF-8",
                 m_rawHex[1] ? QString::fromLatin1(m_udpPreview.toHex(' ').toUpper())
                             : QString::fromUtf8(m_udpPreview).replace('\r', "\\r").replace('\n', "\\n"));
        if (m_udpPacketBytes > m_udpPreview.size())
            line += tr(" …（仅显示前64字节）");
    }
    return line;
}

void WorkbenchRuntime::clearReceiveLogs() {
    for (int i = 0; i < 3; ++i) {
        m_rawText[i].clear();
        m_presentedRaw[i].clear();
        m_rawDirty[i] = false;
    }
    for (auto &bytes : m_rawBytes)
        bytes.clear();
    m_udpLogLines.clear();
    m_udpLogFrame.reset();
    m_udpPreview.clear();
    m_udpLogStatus.clear();
    m_udpPacketBytes = 0;
    m_udpPacketCount = 0;
    m_udpLogPending = false;
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

bool WorkbenchRuntime::startStorage(const QString &path, bool single, QString *error) {
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
    if (single && !m_frame) {
        if (error)
            *error = tr("尚无可保存的波长快照");
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
    if (!single) {
        configuration.dataTypes = storage::DataType::Wavelength |
                                  storage::DataType::CalibratedHardware |
                                  storage::DataType::PointDisplacement;
        configuration.allPoints = true;
    }
    m_storagePending = true;
    m_calculation->storage().start(configuration);
    if (single) {
        m_calculation->storage().writeWavelengthFrame(m_frame);
        m_calculation->storage().stop();
    }
    return true;
}

bool WorkbenchRuntime::connectionAction(const UiRequest &request, UiResponse *response) {
    if (request.panel != PanelId::Connection && request.panel != PanelId::ConnectionDebug)
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
    } else if (action == "pbtn_RefreshSerial")
        initializePorts();
    else if (action == "cbox_TcpRole" || action == "cbox_SerialNameChoose" || action == "cbox_SerialBaudChoose" ||
             action == "cbox_LocakIP")
        response->message = tr("参数将在下次打开通道时生效");
    else if (action == "cbox_CommuteChoose") {
        if (m_states[1] != ConnectionState::Closed || m_states[2] != ConnectionState::Closed) {
            response->success = false;
            response->message = tr("请先关闭网络连接再切换方式");
            response->fields["cbox_CommuteChoose"] =
                QVariantMap{{"index", m_networkChannel == Channel::Udp ? 0 : 1}};
        } else
            m_networkChannel =
                index(fields, "cbox_CommuteChoose") == 0 ? Channel::Udp : Channel::Tcp;
    } else if (action == "pbtn_OpenSerial") {
        if (m_states[0] != ConnectionState::Closed)
            communication.close(Channel::Serial);
        else {
            SerialConfig configuration;
            configuration.portName = text(fields, "cbox_SerialNameChoose");
            bool ok = false;
            configuration.baudRate = text(fields, "cbox_SerialBaudChoose").toInt(&ok);
            if (configuration.portName.isEmpty() || !ok || configuration.baudRate <= 0) {
                response->success = false;
                response->message = tr("请选择有效串口和波特率");
            } else
                communication.openSerial(configuration);
        }
    } else if (action == "pbtn_OpenNet") {
        if (m_states[1] != ConnectionState::Closed || m_states[2] != ConnectionState::Closed)
            communication.close(m_networkChannel);
        else {
            const Channel channel =
                index(fields, "cbox_CommuteChoose") == 0 ? Channel::Udp : Channel::Tcp;
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
            } else if (valid || index(fields,"cbox_TcpRole") == 1) {
                valid = true;
                TcpConfig configuration;
                configuration.role = index(fields,"cbox_TcpRole") == 1 ? TcpRole::Server : TcpRole::Client;
                configuration.host = target;
                if (configuration.role == TcpRole::Server) {
                    bool ok=false; uint port=text(fields,"edit_LocalPort").toUInt(&ok);
                    configuration.listenAddress=QHostAddress(text(fields,"cbox_LocakIP"));
                    if (!ok || port == 0 || port > 65535 || configuration.listenAddress.isNull()) { response->success=false; response->message=tr("监听地址或端口无效"); return true; }
                    configuration.port=quint16(port);
                } else configuration.port = quint16(targetPort);
                m_networkChannel = channel;
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
    } else if (action == "cbox_SerialDisplayMode" || action == "cbox_UdpDisplayMode") {
        const int channel = action == "cbox_SerialDisplayMode" ? 0 : 1;
        m_rawHex[channel] = index(fields, action) == 0;
        if (channel == 1) {
            if (!m_udpLogFrame && !m_udpLogStatus.isEmpty())
                m_udpLogPending = true;
            response->message = tr("已切换未识别包的预览格式；有效包显示时间和摘要");
            return true;
        }
        m_rawDirty[channel] = true;
        const char *name = channel == 0 ? "ptedit_SerialRec" : "ptedit_UdpRec";
        response->fields[name] = rawDisplay(channel);
        response->message = m_rawHex[channel] ? tr("接收区已切换为 Hex 显示")
                                             : tr("接收区已切换为字符串显示（UTF-8）");
    } else if (action == "pbtn_SerialClear" || action == "pbtn_UdpClear" ||
               action == "pbtn_TcpClear") {
        const int channel = action == "pbtn_SerialClear" ? 0 : action == "pbtn_UdpClear" ? 1 : 2;
        m_rawText[channel].clear();
        if (channel < 2)
            m_rawBytes[channel].clear();
        if (channel == 1) {
            m_udpLogLines.clear();
            m_udpLogFrame.reset();
            m_udpPreview.clear();
            m_udpLogStatus.clear();
            m_udpPacketBytes = 0;
            m_udpPacketCount = 0;
            m_udpLogPending = false;
        }
        m_rawDirty[channel] = true;
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
        response->message = tr("已更新波长表显示通道；原始数据继续完整解算和存储");
    } else if (action == "pbtn_SaveDynamic") {
        if (!fields.value("pbtn_SaveDynamic").toBool())
            m_calculation->storage().stop();
        else
            response->success = startStorage(text(fields, "filePath"), false, &response->message);
        if (!response->success)
            response->fields["pbtn_SaveDynamic"] = m_recording || m_storagePending;
    } else if (action == "pbtn_SaveStatic")
        response->success = startStorage(text(fields, "filePath"), true, &response->message);
    else if (request.panel == PanelId::ConnectionDebug && action == "pbtn_ConnectDebugStart") {
        response->fields["edit_ConnectState"] = QStringLiteral("Serial=%1 UDP=%2 TCP=%3; frame=%4")
                                                    .arg(int(m_states[0]))
                                                    .arg(int(m_states[1]))
                                                    .arg(int(m_states[2]))
                                                    .arg(m_frame ? m_frame->revision : 0);
    } else
        return false;
    return true;
}
} // namespace application
} // namespace fosd
