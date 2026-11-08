#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include <QSerialPortInfo>
#include <QNetworkInterface>
#include <cmath>
using namespace fosd;
using namespace fosd::communication;
#include "FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.h"
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
namespace {
QVariant table(const QStringList &headers, const QVariantList &rows) {
    return QVariantMap{{"headers", headers}, {"rows", rows}};
}
} // namespace
ConnectionPresenter::ConnectionPresenter(PanelId panel, IView *view, IWorkbenchView *host,
                                         QObject *parent)
    : PanelPresenter(panel, view, host, parent) {
}

bool ConnectionPresenter::localAction(const QString &action, const QVariantMap &fields) {
    if (action == "pbtn_ChannelCalibration") {
        showPanel(PanelId::ChannelCalibration);
        if (m_model)
            publishCalibration();
        return true;
    }
    if (action == "pbtn_SaveSpectrum") {
        if (!m_host)
            return false;
        const auto path = m_host->chooseFile(true, tr("保存波长曲线"),
                                             tr("PNG (*.png);;JPEG (*.jpg);;BMP (*.bmp)"));
        if (!path.isEmpty())
            m_view->applyFields({{"cplot_Spectrum", QVariantMap{{"savePath", path}}}});
        return true;
    }
    if (action == "ckbox_SpectrumAutoRange")
        return true;
    if (action == "pbtn_SpectrumReset") {
        m_view->applyFields({{"_spectrumReset", true}});
        return true;
    }
    if (action.startsWith("ckbox_CH"))
        return true; // Staged locally until pbtn_SelectCH.
    if (action == "pbtn_SelectAllCH") {
        QVariantMap fields;
        for (int index = 1; index <= 16; ++index)
            fields[QStringLiteral("ckbox_CH%1").arg(index)] = true;
        m_view->applyFields(fields);
        return true;
    }
    return PanelPresenter::localAction(action, fields);
}

bool ConnectionPresenter::prepareAction(const QString &action, QVariantMap &fields) {
    if (!m_host)
        return true;
    if (action != "pbtn_SaveDynamic")
        return true;
    if (!fields.value("pbtn_SaveDynamic").toBool())
        return true;
    const auto path =
        m_host->chooseFile(true, tr("选择存储文件"), tr("CSV (*.csv);;SQLite (*.sqlite *.db)"));
    if (path.isEmpty()) {
        m_view->applyFields({{"pbtn_SaveDynamic", false}});
        return false;
    }
    fields["filePath"] = path;
    return true;
}

void ConnectionPresenter::bindModel() {
    connect(m_model, &application::WorkbenchModel::configurationChanged, this,
            &ConnectionPresenter::publishCalibration);
    if (m_panel != PanelId::Connection)
        return;
    m_historyClock.start();
    m_receiveLogCleanupTimer.setParent(this);
    m_receiveLogCleanupTimer.setObjectName("receiveLogCleanupTimer");
    m_receiveLogCleanupTimer.setInterval(60000);
    connect(&m_receiveLogCleanupTimer, &QTimer::timeout, this,
            &ConnectionPresenter::clearReceiveLogs);
    m_receiveLogCleanupTimer.start();
    QString error;
    m_parameters.read(&m_logSerial, false, &error);
    m_parameters.read(&m_logUdp, false, &error);
    m_parameters.read(&m_logTcp, false, &error);
    connect(m_model, &application::WorkbenchModel::rawDataReceived, this,
            &ConnectionPresenter::rawReceived);
    connect(m_model, &application::WorkbenchModel::sendRequested, this,
            &ConnectionPresenter::rawSent);
    connect(m_model, &application::WorkbenchModel::serialConfigurationChanged, this,
            [this](const communication::SerialConfig &c) { m_logSerial = c; });
    connect(m_model, &application::WorkbenchModel::udpConfigurationChanged, this,
            [this](const communication::UdpConfig &c) { m_logUdp = c; });
    connect(m_model, &application::WorkbenchModel::tcpConfigurationChanged, this,
            [this](const communication::TcpConfig &c) {
                m_logTcp = c;
                m_logTcpPeer = {};
            });
    connect(m_model, &application::WorkbenchModel::frameObserved, this,
            [this](const core::FrameSnapshotPtr &frame) {
                if (frame && frame->source.transport == core::TransportKind::Udp) {
                    m_udpLogFrame = frame;
                    m_udpLogPending = true;
                }
            });
    connect(m_model, &application::WorkbenchModel::decoderRejected, this,
            [this](const communication::DecoderError &e) {
                if (e.channel == Channel::Udp) {
                    m_udpLogStatus = e.message;
                    m_udpLogPending = true;
                }
            });
    connect(m_model, &application::WorkbenchModel::protocolRejected, this,
            [this](const protocol::ProtocolError &e) {
                if (e.channel == Channel::Udp) {
                    m_udpLogFrame.reset();
                    m_udpLogStatus = e.message;
                    m_udpLogPending = true;
                }
            });
}
void ConnectionPresenter::refresh() {
    if (!m_model)
        return;
    if (m_panel == PanelId::ChannelCalibration) {
        presentOwn(m_panel, {{"pbtn_SenerSet.enabled", !m_model->state().configurationPending}});
        return;
    }
    if (m_panel != PanelId::Connection)
        return;
    for (int i = 0; i < 3; ++i) {
        auto state = m_model->connectionState(Channel(i));
        if (state != m_previousStates[i]) {
            if (state == ConnectionState::Closed) {
                m_history.clear();
                m_historyClock.restart();
            }
            if (i == 2 && (state == ConnectionState::Closed || state == ConnectionState::Listening))
                m_logTcpPeer = {};
            m_previousStates[i] = state;
        }
    }
    const auto s = m_model->state();
    const bool pending = s.recording == application::RecordingState::Starting ||
                         s.recording == application::RecordingState::Stopping;
    present(PanelId::Connection,
            {{"pbtn_OpenSerial",
              m_model->connectionState(Channel::Serial) == ConnectionState::Closed ? tr("打开")
                                                                                   : tr("关闭")},
             {"pbtn_OpenNet", m_model->connectionState(m_networkChannel) == ConnectionState::Closed
                                  ? tr("打开")
                                  : tr("关闭")},
             {"pbtn_SaveDynamic", s.recording != application::RecordingState::Stopped},
             {"pbtn_SaveDynamic.enabled", !pending},
             {"pbtn_CalculationStart.enabled", !s.solving},
             {"pbtn_CalculationStop.enabled", s.solving}});
}
bool ConnectionPresenter::handleAction(const QString &action, const QVariantMap &fields,
                                       QString &message, QVariantMap &updates) {
    const auto &f = fields;
    bool success = true;
    if (m_panel == PanelId::ChannelCalibration) {
        if (action != "pbtn_SenerSet")
            return false;
        auto configuration = m_model->configuration();
        configuration.channelCalibration.channelIndices.clear();
        configuration.channelCalibration.calibrationFactors.clear();
        configuration.channelCalibration.calibrationOffsets.clear();
        for (int i = 1; i <= 12; ++i) {
            bool indexOk = false, factorOk = false, offsetOk = false;
            const int source =
                text(f, QStringLiteral("edit_Sensor%1Order").arg(i)).toInt(&indexOk) - 1;
            const double factor =
                text(f, QStringLiteral("edit_Sensor%1Para").arg(i)).toDouble(&factorOk);
            const double offset =
                text(f, QStringLiteral("edit_Sensor%1Offset").arg(i)).toDouble(&offsetOk);
            if (!indexOk || !factorOk || !offsetOk || !std::isfinite(offset)) {
                success = false;
                message = tr("输入 %1：源序号、倍率和偏置必须为有效数值，偏置须为有限数值").arg(i);
                return success;
            }
            configuration.channelCalibration.channelIndices.push_back(source);
            configuration.channelCalibration.calibrationFactors.push_back(factor);
            configuration.channelCalibration.calibrationOffsets.push_back(offset);
        }
        success = m_model->applyCalibration(configuration, &message);
        return success;
    }

    if (m_panel != PanelId::Connection)
        return false;
    auto &communication = *m_model;
    success = true;
    message = tr("请求已提交");
    if (action == "pbtn_CalculationStart" || action == "pbtn_CalculationStop") {
        const bool start = action == "pbtn_CalculationStart";
        success = m_model->setCalculationRunning(start, &message);
        if (!success)
            return false;
        const bool m_solving = m_model->state().solving;
        message = m_solving ? tr("解算已启动，等待有效数据") : tr("解算已停止，通信继续");
        updates["pbtn_CalculationStart.enabled"] = !m_solving;
        updates["pbtn_CalculationStop.enabled"] = m_solving;
        m_model->notice(message);
    } else if (action == "pbtn_ConfigRead") {
        success = m_model->loadCalibration(&message);
    } else if (action == "pbtn_ChannelCalibration") {
        publishCalibration();
    } else if (action == "pbtn_RefreshLocalAddress") {
        initializeLocalAddresses(text(fields, "cbox_LocakIP"));
        message = tr("本地 IPv4 地址已刷新");
    } else if (action == "pbtn_RefreshSerial") {
        const auto serial = fields.value("cbox_SerialNameChoose").toMap();
        initializePorts(serial.value("data", serial.value("text")).toString().section(':', 0, 0),
                        text(fields, "cbox_LocakIP"));
        message = tr("串口与本地网口已刷新");
    } else if (action == "cbox_SerialNameChoose" || action == "cbox_SerialBaudChoose" ||
               action == "cbox_LocakIP")
        message = tr("参数将在下次打开通道时生效");
    else if (action == "cbox_CommuteChoose") {
        if (m_model->connectionState(Channel::Udp) != ConnectionState::Closed ||
            m_model->connectionState(Channel::Tcp) != ConnectionState::Closed) {
            success = false;
            message = tr("请先关闭网络连接再切换方式");
            updates["cbox_CommuteChoose"] = QVariantMap{
                {"index",
                 m_networkChannel == Channel::Udp ? 0 : m_tcpRole == TcpRole::Server ? 1 : 2}};
        } else {
            const int mode = index(fields, "cbox_CommuteChoose");
            if (mode < 0 || mode > 2) {
                success = false;
                message = tr("请选择 UDP、TCP Server 或 TCP Client");
                return success;
            }
            config::IniConfigStore store(config::ParameterStore::runtimePath());
            success = store.writeValue("classic.connection", "networkMode", mode, &message);
            if (success)
                publishNetworkConfiguration(mode);
        }
    } else if (action == "pbtn_OpenSerial") {
        if (m_model->connectionState(Channel::Serial) != ConnectionState::Closed)
            communication.closeChannel(Channel::Serial);
        else {
            SerialConfig configuration;
            QString ignored;
            m_parameters.read(&configuration, false, &ignored);
            const auto serial = fields.value("cbox_SerialNameChoose").toMap();
            configuration.portName = serial.value("data").toString();
            if (configuration.portName.isEmpty())
                configuration.portName =
                    text(fields, "cbox_SerialNameChoose").section(':', 0, 0).trimmed();
            bool ok = false;
            configuration.baudRate = text(fields, "cbox_SerialBaudChoose").toInt(&ok);
            bool bitsOk = true, parityOk = true, stopOk = true, flowOk = true;
            if (fields.contains("edit_SerialDataBits"))
                configuration.dataBits = static_cast<QSerialPort::DataBits>(
                    text(fields, "edit_SerialDataBits").toInt(&bitsOk));
            if (fields.contains("edit_SerialParity")) {
                const QMap<QString, QSerialPort::Parity> choices{
                    {"none", QSerialPort::NoParity},     {"n", QSerialPort::NoParity},
                    {tr("无"), QSerialPort::NoParity},   {"odd", QSerialPort::OddParity},
                    {"o", QSerialPort::OddParity},       {tr("奇"), QSerialPort::OddParity},
                    {"even", QSerialPort::EvenParity},   {"e", QSerialPort::EvenParity},
                    {tr("偶"), QSerialPort::EvenParity}, {"mark", QSerialPort::MarkParity},
                    {"space", QSerialPort::SpaceParity}};
                const auto value = text(fields, "edit_SerialParity").trimmed().toLower();
                parityOk = choices.contains(value);
                if (parityOk)
                    configuration.parity = choices.value(value);
            }
            if (fields.contains("edit_SerialStopBits")) {
                const auto value = text(fields, "edit_SerialStopBits").trimmed();
                stopOk = value == "1" || value == "1.5" || value == "2";
                configuration.stopBits =
                    value == "1.5" ? QSerialPort::OneAndHalfStop
                                   : value == "2" ? QSerialPort::TwoStop : QSerialPort::OneStop;
            }
            if (fields.contains("edit_SerialFlowControl")) {
                const QMap<QString, QSerialPort::FlowControl> choices{
                    {"none", QSerialPort::NoFlowControl},
                    {tr("无"), QSerialPort::NoFlowControl},
                    {"hardware", QSerialPort::HardwareControl},
                    {tr("硬件"), QSerialPort::HardwareControl},
                    {"software", QSerialPort::SoftwareControl},
                    {tr("软件"), QSerialPort::SoftwareControl}};
                const auto value = text(fields, "edit_SerialFlowControl").trimmed().toLower();
                flowOk = choices.contains(value);
                if (flowOk)
                    configuration.flowControl = choices.value(value);
            }
            if (!ok || !bitsOk || !parityOk || !stopOk || !flowOk ||
                !config::validateParameter(configuration, true, &message)) {
                success = false;
                message = tr("串口参数无效：请检查端口、波特率、数据位、校验位、停止位和流控");
            } else
                communication.openSerial(configuration);
        }
    } else if (action == "pbtn_OpenNet") {
        if (m_model->connectionState(Channel::Udp) != ConnectionState::Closed ||
            m_model->connectionState(Channel::Tcp) != ConnectionState::Closed)
            communication.closeChannel(m_networkChannel);
        else {
            const int mode = index(fields, "cbox_CommuteChoose");
            if (mode < 0 || mode > 2) {
                success = false;
                message = tr("通信方式无效");
                return success;
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
                    bool ok = false;
                    uint port = text(fields, "edit_LocalPort").toUInt(&ok);
                    configuration.listenAddress = QHostAddress(text(fields, "cbox_LocakIP"));
                    if (!ok || port == 0 || port > 65535 || configuration.listenAddress.isNull()) {
                        success = false;
                        message = tr("监听地址或端口无效");
                        return success;
                    }
                    configuration.port = quint16(port);
                } else
                    configuration.port = quint16(targetPort);
                m_networkChannel = channel;
                m_tcpRole = configuration.role;
                communication.openTcp(configuration);
            }
            if (!valid) {
                success = false;
                message = tr("网络地址或端口无效（UDP 本地端口可填 0）");
            }
        }
    } else if (action.startsWith("edit_SerialCommand") || action.startsWith("pbtn_SerialCommand")) {
        const bool send = action.startsWith("pbtn_");
        const QString prefix = send ? "pbtn_SerialCommand" : "edit_SerialCommand";
        bool valid = false;
        const int slot = action.mid(prefix.size()).toInt(&valid);
        if (!valid || slot < 0 || slot >= 10)
            return false;
        const auto command = text(fields, QStringLiteral("edit_SerialCommand%1").arg(slot));
        config::IniConfigStore store(config::ParameterStore::runtimePath());
        success =
            store.writeValue("classic.serialCommands", QString::number(slot), command, &message);
        if (success) {
            message = tr("快捷指令已保存");
            if (send) {
                success = m_model->sendSerialCommand(command, &message);
                if (success)
                    message = tr("指令已提交发送（末尾 CRLF）");
            }
        }
    } else if (action == "pbtn_SerialSend") {
        success = m_model->sendSerialCommand(text(fields, "ptedit_SerialSend"), &message);
        if (success)
            message = tr("指令已提交发送（末尾 CRLF）");
    } else if (action == "pbtn_UdpSend" || action == "pbtn_TcpSend") {
        const Channel channel = action == "pbtn_UdpSend" ? Channel::Udp : Channel::Tcp;
        const auto input = channel == Channel::Udp ? "ptedit_UdpSend" : "ptedit_TcpSen";
        Endpoint peer;
        if (channel == Channel::Udp) {
            bool ok = false;
            const uint port = text(fields, "edit_TargetPort").toUInt(&ok);
            peer.address = QHostAddress(text(fields, "edit_TargetIP"));
            peer.port = quint16(port);
            if (!ok || port > 65535 || !peer.isValid()) {
                success = false;
                message = tr("UDP 目标地址无效");
                return success;
            }
        }
        if (m_model->connectionState(channel) == ConnectionState::Closed) {
            success = false;
            message = tr("请先打开通信通道");
        } else
            communication.send(channel, text(fields, input).toUtf8(), peer);
    } else if (action == "cbox_SerialDisplayMode" || action == "cbox_UdpDisplayMode" ||
               action == "cbox_TcpDisplayMode") {
        const int channel =
            action == "cbox_SerialDisplayMode" ? 0 : action == "cbox_UdpDisplayMode" ? 1 : 2;
        m_rawHex[channel] = index(fields, action) == 0;
        m_resetTraffic[channel] = true;
        message = tr("已切换收发日志的预览格式；有效 UDP 包保留摘要");
    } else if (action == "pbtn_SerialClear" || action == "pbtn_UdpClear" ||
               action == "pbtn_TcpClear") {
        const int channel = action == "pbtn_SerialClear" ? 0 : action == "pbtn_UdpClear" ? 1 : 2;
        clearTraffic(channel);
        const char *names[] = {"ptedit_SerialRec", "ptedit_UdpRec", "ptedit_TcpRec"};
        updates[names[channel]] = QString();
    } else if (action == "pbtn_WaveLengthReset")
        success = m_model->baseline(&message);
    else if (action == "pbtn_ParaSet" || action == "pbtn_ParaRest") {
        auto configuration = m_model->configuration();
        bool aOk = true, bOk = true;
        configuration.strainCalibration.A =
            action == "pbtn_ParaRest" ? 1.0 : text(fields, "textEdit_APara").toDouble(&aOk);
        configuration.strainCalibration.B =
            action == "pbtn_ParaRest" ? 0.0 : text(fields, "textEdit_BPara").toDouble(&bOk);
        if (!aOk || !bOk) {
            success = false;
            message = tr("A、B 必须为有效数值");
        } else
            success = m_model->applyCalibration(configuration, &message);
    } else if (action == "pbtn_SelectCH") {
        m_filterChannels = true;
        m_channels.clear();
        for (int channel = 1; channel <= 16; ++channel)
            if (fields.value(QStringLiteral("ckbox_CH%1").arg(channel)).toBool())
                m_channels.insert(channel);
        QStringList selected;
        for (int ch : m_channels)
            selected.append(QString::number(ch));
        config::IniConfigStore store(config::ParameterStore::runtimePath());
        QString error;
        if (!store.writeValue("classic.display", "channels", selected, &error)) {
            success = false;
            message = error;
        }
        m_presentedFrame = 0;
        message = tr("已更新曲线显示通道；波长表、解算和存储保留全部通道");
    } else if (action == "pbtn_SaveDynamic") {
        success = m_model->setRecording(fields.value("pbtn_SaveDynamic").toBool(),
                                        text(fields, "filePath"), &message);
        const auto saving = m_model->state().recording;
        updates["pbtn_SaveDynamic"] = saving != application::RecordingState::Stopped;
        updates["pbtn_SaveDynamic.enabled"] = saving != application::RecordingState::Starting &&
                                              saving != application::RecordingState::Stopping;
    } else
        return false;
    return success;
}
void ConnectionPresenter::initialize() {
    if (!m_model)
        return;
    if (m_panel == PanelId::ChannelCalibration) {
        publishCalibration();
        return;
    }

    initializePorts();
    communication::SerialConfig serial;
    QString loadError;
    if (m_parameters.read(&serial, false, &loadError)) {
        const QMap<int, QString> parities{{QSerialPort::NoParity, "None"},
                                          {QSerialPort::OddParity, "Odd"},
                                          {QSerialPort::EvenParity, "Even"},
                                          {QSerialPort::MarkParity, "Mark"},
                                          {QSerialPort::SpaceParity, "Space"}};
        const QMap<int, QString> flows{{QSerialPort::NoFlowControl, "None"},
                                       {QSerialPort::HardwareControl, "Hardware"},
                                       {QSerialPort::SoftwareControl, "Software"}};
        present(PanelId::Connection,
                {{"cbox_SerialBaudChoose", QVariantMap{{"text", QString::number(serial.baudRate)}}},
                 {"edit_SerialDataBits", QString::number(int(serial.dataBits))},
                 {"edit_SerialParity", parities.value(serial.parity)},
                 {"edit_SerialStopBits", serial.stopBits == QSerialPort::OneAndHalfStop
                                             ? "1.5"
                                             : serial.stopBits == QSerialPort::TwoStop ? "2" : "1"},
                 {"edit_SerialFlowControl", flows.value(serial.flowControl)}});
    }

    config::IniConfigStore store(config::ParameterStore::runtimePath());
    QVariant channels;
    QVariantMap commands;
    for (int i = 0; i < 10; ++i) {
        QVariant command;
        if (store.readValue("classic.serialCommands", QString::number(i), &command))
            commands[QStringLiteral("edit_SerialCommand%1").arg(i)] = command;
    }
    if (!commands.isEmpty())
        present(PanelId::Connection, commands);
    if (store.readValue("classic.display", "channels", &channels)) {
        m_filterChannels = true;
        m_channels.clear();
        for (const auto &s : channels.toStringList())
            if (s.toInt() >= 1 && s.toInt() <= 16)
                m_channels.insert(s.toInt());
    }
    QVariant networkMode;
    int mode = 0;
    if (store.readValue("classic.connection", "networkMode", &networkMode) &&
        networkMode.toInt() >= 0 && networkMode.toInt() <= 2)
        mode = networkMode.toInt();
    publishNetworkConfiguration(mode);
    QVariantMap fields;
    for (int channel = 1; channel <= 16; ++channel)
        fields[QStringLiteral("ckbox_CH%1").arg(channel)] =
            !m_filterChannels || m_channels.contains(channel);
    present(PanelId::Connection, fields);

    publishCalibration();
    refresh();
}
void ConnectionPresenter::poll() {
    if (!m_model || m_panel != PanelId::Connection)
        return;
    const auto m_frame = m_model->frame();
    QVariantMap raw;
    flushUdpTraffic();
    const char *names[] = {"ptedit_SerialRec", "ptedit_UdpRec", "ptedit_TcpRec"};
    for (int i = 0; i < 3; ++i) {
        QVariantList entries;
        for (const auto &entry : m_traffic[i]) {
            if (m_resetTraffic[i] || entry.id > m_publishedTrafficId[i])
                entries.append(
                    QVariantMap{{"text", formatTraffic(i, entry)}, {"outgoing", entry.outgoing}});
        }
        if (!entries.isEmpty() || m_resetTraffic[i])
            raw[names[i]] = QVariantMap{{"entries", entries}, {"clear", m_resetTraffic[i]}};
        if (!m_traffic[i].isEmpty())
            m_publishedTrafficId[i] = m_traffic[i].last().id;
        m_resetTraffic[i] = false;
    }
    if (!raw.isEmpty())
        present(PanelId::Connection, raw);
    if (m_frame && m_presentedFrame != m_frame->revision) {
        const auto frame = std::dynamic_pointer_cast<const core::WavelengthFrame>(m_frame->data);
        if (frame) {
            QVariantList rows;
            for (const auto &record : frame->records) {
                const auto key = QString("CH%1_%2").arg(record.channel).arg(record.sensor);
                auto &samples = m_history[key];
                samples.append(
                    QVariant(QVariantList{m_historyClock.elapsed() / 1000.0, record.wavelength}));
                while (samples.size() > 300)
                    samples.removeFirst();
            }
            QVariantList series;
            for (auto it = m_history.constBegin(); it != m_history.constEnd(); ++it) {
                int ch = it.key().mid(2).section('_', 0, 0).toInt();
                if (!m_filterChannels || m_channels.contains(ch))
                    series.append(QVariantMap{{"name", it.key()}, {"samples", it.value()}});
            }
            present(PanelId::Connection, {{"cplot_Spectrum", QVariantMap{{"series", series}}}});
            for (const auto &record : frame->records) {
                rows.append(QVariant(QVariantList{record.channel, record.sensor,
                                                  QString::number(record.wavelength, 'f', 6)}));
                if (rows.size() >= 2048)
                    break;
            }
            present(
                PanelId::Connection,
                {{"tablev_Wavelength", table({tr("通道"), tr("传感器"), tr("波长 (nm)")}, rows)}});
        }
        m_presentedFrame = m_frame->revision;
    }
}
void ConnectionPresenter::rawReceived(Channel channel, const QByteArray &bytes,
                                      const Endpoint &peer) {
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

void ConnectionPresenter::rawSent(Channel channel, const QByteArray &bytes, const Endpoint &peer) {
    if (channel == Channel::Udp)
        flushUdpTraffic();
    appendTraffic(channel, bytes, peer, true);
}

void ConnectionPresenter::appendTraffic(Channel channel, const QByteArray &bytes,
                                        const Endpoint &peer, bool outgoing, const QString &summary,
                                        int byteCount, bool showPayload, const QDateTime &at) {
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
        // Decode all bytes to preserve UTF-8 state across serial/TCP reads, but retain a short
        // preview.
        entry.utf8 = m_receiveDecoder[i]->toUnicode(bytes).left(64);
    }
    auto endpoint = [](const Endpoint &value) {
        return value.address.toString() + ':' + QString::number(value.port);
    };
    if (channel == Channel::Serial) {
        entry.endpoint =
            tr("串口 %1 @%2")
                .arg(m_logSerial.portName.isEmpty() ? tr("未指定") : m_logSerial.portName)
                .arg(m_logSerial.baudRate);
    } else if (channel == Channel::Udp) {
        const auto remote = peer.isValid() ? peer : m_logUdp.defaultPeer;
        const QString local =
            m_logUdp.bindAddress.toString() + ':' +
            (m_logUdp.localPort ? QString::number(m_logUdp.localPort) : tr("自动端口"));
        entry.endpoint =
            outgoing ? local + " -> " + endpoint(remote) : endpoint(remote) + " -> " + local;
    } else {
        const QString local =
            m_logTcp.role == TcpRole::Server
                ? m_logTcp.listenAddress.toString() + ':' + QString::number(m_logTcp.port)
                : tr("本地系统分配端口");
        const QString remote = peer.isValid()
                                   ? endpoint(peer)
                                   : m_logTcp.role == TcpRole::Client
                                         ? m_logTcp.host + ':' + QString::number(m_logTcp.port)
                                         : m_logTcpPeer.isValid() ? endpoint(m_logTcpPeer)
                                                                  : tr("客户端端口待接收数据确认");
        entry.endpoint = outgoing ? local + " -> " + remote : remote + " -> " + local;
    }
    auto &entries = m_traffic[i];
    entries.append(entry);
    while (entries.size() > 200)
        entries.removeFirst();
}

QString ConnectionPresenter::formatTraffic(int channel, const TrafficEntry &entry) const {
    const char *names[] = {"SERIAL", "UDP", "TCP"};
    QString line = QStringLiteral("[%1] %2 %3 | %4 | %5 B")
                       .arg(entry.at.toString("yyyy-MM-dd HH:mm:ss.zzz"), names[channel],
                            entry.outgoing ? tr("TX 发送请求") : tr("RX 接收"), entry.endpoint)
                       .arg(entry.byteCount);
    QString dataLine = entry.summary;
    if (entry.showPayload) {
        const QString payload = m_rawHex[channel]
                                    ? QString::fromLatin1(entry.preview.toHex(' ').toUpper())
                                    : QString(entry.utf8)
                                          .replace('\r', "\\r")
                                          .replace('\n', "\\n")
                                          .replace(QChar(0x2028), "\\u2028")
                                          .replace(QChar(0x2029), "\\u2029");
        if (!dataLine.isEmpty())
            dataLine += " | ";
        dataLine += tr("预览%1：%2").arg(m_rawHex[channel] ? "Hex" : "UTF-8", payload);
        if (entry.byteCount > entry.preview.size())
            dataLine += tr(" …（预览已截断）");
    }
    return line + '\n' + dataLine;
}

QString ConnectionPresenter::udpLogEntry() const {
    QString line = tr("累计=%1").arg(m_udpPacketCount);
    if (m_udpLogFrame) {
        line += " | " + m_udpLogFrame->protocolId;
        const auto wavelengths =
            std::dynamic_pointer_cast<const core::WavelengthFrame>(m_udpLogFrame->data);
        if (wavelengths) {
            line += tr(" | 序号=%1 | 版本=%2 类型=%3 | 记录=%4")
                        .arg(wavelengths->sequence)
                        .arg(wavelengths->version)
                        .arg(wavelengths->dataType)
                        .arg(wavelengths->records.size());
            if (!wavelengths->records.isEmpty()) {
                const auto &first = wavelengths->records.first();
                line += tr(" | 首条=CH%1/%2 %3 nm")
                            .arg(first.channel)
                            .arg(first.sensor)
                            .arg(first.wavelength, 0, 'g', 9);
            }
        }
    } else
        line += tr(" | 未识别/无效包：%1").arg(m_udpLogStatus.left(160));
    return line;
}

void ConnectionPresenter::flushUdpTraffic() {
    if (!m_udpLogPending)
        return;
    m_udpLogPending = false;
    appendTraffic(Channel::Udp, m_udpPreview, m_udpPeer, false, udpLogEntry(), m_udpPacketBytes,
                  !m_udpLogFrame, m_udpReceivedAt);
}

void ConnectionPresenter::clearTraffic(int channel) {
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

void ConnectionPresenter::clearReceiveLogs() {
    for (int i = 0; i < 3; ++i)
        clearTraffic(i);
    present(PanelId::Connection, {{"ptedit_SerialRec", QString()},
                                  {"ptedit_UdpRec", QString()},
                                  {"ptedit_TcpRec", QString()}});
}
void ConnectionPresenter::initializePorts(const QString &serialPort, const QString &localAddress) {
    QStringList ports, labels;
    QVariantList data;
    for (const auto &port : QSerialPortInfo::availablePorts()) {
        ports.append(port.portName());
        labels.append(port.portName() + ":" +
                      (port.description().isEmpty() ? tr("串口设备") : port.description()));
        data.append(port.portName());
    }
    communication::SerialConfig saved;
    QString error;
    m_parameters.read(&saved, false, &error);
    const auto preferred = serialPort.isEmpty() ? saved.portName : serialPort;
    int selected = ports.indexOf(preferred);
    if (selected < 0)
        selected = ports.isEmpty() ? -1 : 0;
    present(PanelId::Connection,
            {{"cbox_SerialNameChoose",
              QVariantMap{{"items", labels}, {"data", data}, {"index", selected}}}});
    initializeLocalAddresses(localAddress);
}

void ConnectionPresenter::initializeLocalAddresses(const QString &localAddress) {
    QStringList addresses{QStringLiteral("0.0.0.0"), QStringLiteral("127.0.0.1")};
    for (const auto &address : QNetworkInterface::allAddresses())
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            !addresses.contains(address.toString()))
            addresses.append(address.toString());
    const int localIndex = addresses.indexOf(localAddress);
    present(PanelId::Connection,
            {{"cbox_LocakIP",
              QVariantMap{{"items", addresses}, {"index", localIndex < 0 ? 0 : localIndex}}}});
}

void ConnectionPresenter::publishNetworkConfiguration(int mode) {
    m_networkChannel = mode == 0 ? communication::Channel::Udp : communication::Channel::Tcp;
    m_tcpRole = mode == 1 ? communication::TcpRole::Server : communication::TcpRole::Client;
    QVariantMap fields{{"cbox_CommuteChoose", QVariantMap{{"index", mode}}},
                       {"cbox_LocakIP.enabled", mode != 2},
                       {"edit_LocalPort.enabled", mode != 2},
                       {"edit_TargetIP.enabled", mode != 1},
                       {"edit_TargetPort.enabled", mode != 1}};
    QString error;
    if (mode == 0) {
        communication::UdpConfig saved;
        if (m_parameters.read(&saved, false, &error)) {
            fields["cbox_LocakIP"] = QVariantMap{{"text", saved.bindAddress.toString()}};
            fields["edit_LocalPort"] = QString::number(saved.localPort);
            if (saved.defaultPeer.isValid()) {
                fields["edit_TargetIP"] = saved.defaultPeer.address.toString();
                fields["edit_TargetPort"] = QString::number(saved.defaultPeer.port);
            }
        }
    } else {
        communication::TcpConfig saved;
        if (m_parameters.read(&saved, false, &error)) {
            fields["cbox_LocakIP"] = QVariantMap{{"text", saved.listenAddress.toString()}};
            fields["edit_TargetIP"] = saved.host;
            if (saved.port)
                fields[mode == 1 ? "edit_LocalPort" : "edit_TargetPort"] =
                    QString::number(saved.port);
        }
    }
    present(PanelId::Connection, fields);
}
void ConnectionPresenter::publishCalibration() {
    present(
        PanelId::Connection,
        {{"textEdit_APara", QString::number(m_model->configuration().strainCalibration.A, 'g', 12)},
         {"textEdit_BPara",
          QString::number(m_model->configuration().strainCalibration.B, 'g', 12)}});
    QVariantMap fields;
    for (int i = 0; i < 12; ++i) {
        const auto &channels = m_model->configuration().channelCalibration;
        fields[QStringLiteral("edit_Sensor%1Order").arg(i + 1)] = QString::number(
            i < int(channels.channelIndices.size()) ? channels.channelIndices[size_t(i)] + 1
                                                    : i + 1);
        fields[QStringLiteral("edit_Sensor%1Para").arg(i + 1)] = QString::number(
            i < int(channels.calibrationFactors.size()) ? channels.calibrationFactors[size_t(i)]
                                                        : 1.0,
            'g', 12);
        fields[QStringLiteral("edit_Sensor%1Offset").arg(i + 1)] = QString::number(
            i < int(channels.calibrationOffsets.size()) ? channels.calibrationOffsets[size_t(i)]
                                                        : 0.0,
            'g', 12);
    }
    present(PanelId::ChannelCalibration, fields);
}

} // namespace classic
} // namespace ui
} // namespace fosd
