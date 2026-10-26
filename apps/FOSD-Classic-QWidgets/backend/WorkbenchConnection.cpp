#include "WorkbenchRuntime.h"
#include <QFileInfo>
#include <cmath>
namespace fosd {
namespace application {
using namespace ui::classic;
using namespace communication;
void WorkbenchRuntime::transportState(Channel channel, ConnectionState state) {
    m_states[int(channel)] = state;
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

void WorkbenchRuntime::rawReceived(Channel channel, const QByteArray &bytes, const Endpoint &) {
    auto &cache = m_rawText[int(channel)];
    cache.append(channel == Channel::Serial
                     ? QString::fromUtf8(bytes)
                     : QString::fromLatin1(bytes.toHex(' ').toUpper()) + '\n');
    if (cache.size() > 65536)
        cache = cache.right(65536);
    // Actual widgets are refreshed at 10 Hz, independently of the receive rate.
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
    if (action == "pbtn_RefreshSerial")
        initializePorts();
    else if (action == "cbox_SerialNameChoose" || action == "cbox_SerialBaudChoose" ||
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
            } else if (valid) {
                TcpConfig configuration;
                configuration.host = target;
                configuration.port = quint16(targetPort);
                m_networkChannel = channel;
                communication.openTcp(configuration);
            }
            if (!valid) {
                response->success = false;
                response->message = tr("网络地址或端口无效（UDP 本地端口可填 0）");
            }
        }
    } else if (action == "pbtn_SerialSend" || action == "pbtn_UdpSend" ||
               action == "pbtn_TcpSend") {
        const Channel channel = action == "pbtn_SerialSend"
                                    ? Channel::Serial
                                    : action == "pbtn_UdpSend" ? Channel::Udp : Channel::Tcp;
        const auto input = channel == Channel::Serial
                               ? "ptedit_SerialSend"
                               : channel == Channel::Udp ? "ptedit_UdpSend" : "ptedit_TcpSen";
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
    } else if (action == "pbtn_SerialClear" || action == "pbtn_UdpClear" ||
               action == "pbtn_TcpClear") {
        const int channel = action == "pbtn_SerialClear" ? 0 : action == "pbtn_UdpClear" ? 1 : 2;
        m_rawText[channel].clear();
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
