#include "CommunicationPresenter.h"

#include <QDateTime>
#include <QSerialPortInfo>

namespace fosd {
namespace assistant {
using namespace communication;
namespace {
std::unique_ptr<IPacketDecoder> rawOnlyDecoder(Channel channel) {
    Q_UNUSED(channel)
    return std::unique_ptr<IPacketDecoder>();
}
QString channelName(Channel channel) {
    switch (channel) {
    case Channel::Serial: return QStringLiteral("Serial");
    case Channel::Udp: return QStringLiteral("UDP");
    case Channel::Tcp: return QStringLiteral("TCP");
    }
    return QString();
}
}

CommunicationPresenter::CommunicationPresenter(QObject *parent)
    : QObject(parent), m_control(nullptr, rawOnlyDecoder) {
    for (int i = 0; i < 3; ++i)
        m_states[i] = ConnectionState::Closed;
    connect(&m_control, &ConnectControl::rawDataReceived,
            this, &CommunicationPresenter::receiveBytes);
    connect(&m_control, &ConnectControl::stateChanged,
            this, &CommunicationPresenter::updateState);
    connect(&m_control, &ConnectControl::errorOccurred,
            this, &CommunicationPresenter::handleError);
    refreshSerialPorts();
}

Channel CommunicationPresenter::channel() const {
    return static_cast<Channel>(m_selectedChannel);
}
void CommunicationPresenter::setSelectedChannel(int value) {
    if (value < 0 || value > 2 || value == m_selectedChannel)
        return;
    m_selectedChannel = value;
    emit selectionChanged();
    emit connectionChanged();
}
QString CommunicationPresenter::statusText() const {
    switch (m_states[m_selectedChannel]) {
    case ConnectionState::Closed: return QString::fromUtf8("未连接");
    case ConnectionState::Opening: return QString::fromUtf8("正在打开 / 连接");
    case ConnectionState::Ready: return QString::fromUtf8("已绑定，可收发");
    case ConnectionState::Listening: return QString::fromUtf8("监听中，等待客户端");
    case ConnectionState::Connected: return QString::fromUtf8("已连接");
    }
    return QString();
}
bool CommunicationPresenter::isOpen() const {
    return m_states[m_selectedChannel] != ConnectionState::Closed;
}
bool CommunicationPresenter::canSend() const {
    const ConnectionState state = m_states[m_selectedChannel];
    return state == ConnectionState::Ready || state == ConnectionState::Connected;
}
void CommunicationPresenter::refreshSerialPorts() {
    QStringList ports;
    const auto available = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : available)
        ports.append(port.portName());
    ports.sort();
    if (ports == m_serialPorts)
        return;
    m_serialPorts = ports;
    emit serialPortsChanged();
}
bool CommunicationPresenter::parsePort(const QString &text, quint16 &port) {
    bool ok = false;
    const uint value = text.trimmed().toUInt(&ok);
    if (!ok || value < 1 || value > 65535)
        return fail(QString::fromUtf8("配置"), QString::fromUtf8("端口必须是 1–65535 的整数"));
    port = static_cast<quint16>(value);
    return true;
}
bool CommunicationPresenter::openConnection(const QVariantMap &settings) {
    if (isOpen())
        return fail(QString::fromUtf8("打开"), QString::fromUtf8("请先关闭当前通道，再修改连接参数"));
    SerialConfig serial;
    UdpConfig udp;
    TcpConfig tcp;
    if (channel() == Channel::Serial) {
        serial.portName = settings.value(QStringLiteral("portName")).toString().trimmed();
        bool ok = false;
        serial.baudRate = settings.value(QStringLiteral("baudRate")).toString().toInt(&ok);
        if (serial.portName.isEmpty() || !ok || serial.baudRate <= 0)
            return fail(QString::fromUtf8("配置"), QString::fromUtf8("请选择串口并填写有效波特率"));
        const int dataBits = settings.value(QStringLiteral("dataBits"), 8).toInt();
        const int parity = settings.value(QStringLiteral("parity"), 0).toInt();
        const int stopBits = settings.value(QStringLiteral("stopBits"), 1).toInt();
        const int flow = settings.value(QStringLiteral("flowControl"), 0).toInt();
        if (dataBits < 5 || dataBits > 8 || (parity != 0 && parity != 2 && parity != 3)
                || (stopBits != 1 && stopBits != 2) || flow < 0 || flow > 2)
            return fail(QString::fromUtf8("配置"), QString::fromUtf8("串口数据位 / 校验 / 停止位 / 流控无效"));
        serial.dataBits = static_cast<QSerialPort::DataBits>(dataBits);
        serial.parity = static_cast<QSerialPort::Parity>(parity);
        serial.stopBits = static_cast<QSerialPort::StopBits>(stopBits);
        serial.flowControl = static_cast<QSerialPort::FlowControl>(flow);
    } else if (channel() == Channel::Udp) {
        udp.bindAddress = QHostAddress(settings.value(QStringLiteral("bindAddress")).toString().trimmed());
        if (udp.bindAddress.isNull())
            return fail(QString::fromUtf8("配置"), QString::fromUtf8("UDP 绑定地址必须是有效 IP 地址"));
        if (!parsePort(settings.value(QStringLiteral("localPort")).toString(), udp.localPort))
            return false;
    } else {
        const int role = settings.value(QStringLiteral("tcpRole"), 0).toInt();
        if (role != 0 && role != 1)
            return fail(QString::fromUtf8("配置"), QString::fromUtf8("TCP 模式无效"));
        tcp.role = role == 0 ? TcpRole::Client : TcpRole::Server;
        tcp.host = settings.value(QStringLiteral("host")).toString().trimmed();
        tcp.listenAddress = QHostAddress(tcp.host);
        if (tcp.host.isEmpty() || (tcp.role == TcpRole::Server && tcp.listenAddress.isNull()))
            return fail(QString::fromUtf8("配置"), QString::fromUtf8("请填写主机名；服务端必须填写有效绑定 IP"));
        if (!parsePort(settings.value(QStringLiteral("port")).toString(), tcp.port))
            return false;
    }
    m_lastError.clear();
    emit lastErrorChanged();
    updateState(channel(), ConnectionState::Opening);
    switch (channel()) {
    case Channel::Serial: m_control.openSerial(serial); break;
    case Channel::Udp: m_control.openUdp(udp); break;
    case Channel::Tcp: m_control.openTcp(tcp); break;
    }
    return true;
}
void CommunicationPresenter::closeConnection() {
    m_control.close(channel());
    updateState(channel(), ConnectionState::Closed);
}
bool CommunicationPresenter::sendData(const QString &text, bool hex, bool appendCrLf,
                                       const QString &address, const QString &portText) {
    if (!canSend())
        return fail(QString::fromUtf8("发送"), QString::fromUtf8("当前通道尚不能发送数据"));
    QByteArray bytes;
    if (hex) {
        QByteArray digits;
        for (QChar c : text) {
            if (c.isSpace())
                continue;
            const ushort value = c.unicode();
            if (!((value >= '0' && value <= '9') || (value >= 'a' && value <= 'f')
                    || (value >= 'A' && value <= 'F')))
                return fail(QString::fromUtf8("发送"), QString::fromUtf8("HEX 只能包含十六进制数字和空白"));
            digits.append(static_cast<char>(value));
        }
        if (digits.size() % 2 != 0)
            return fail(QString::fromUtf8("发送"), QString::fromUtf8("HEX 每个字节必须包含两位数字"));
        bytes = QByteArray::fromHex(digits);
    } else {
        bytes = text.toUtf8();
    }
    if (appendCrLf)
        bytes.append("\r\n");
    if (bytes.isEmpty())
        return fail(QString::fromUtf8("发送"), QString::fromUtf8("发送内容为空"));
    Endpoint peer;
    if (channel() == Channel::Udp) {
        peer.address = QHostAddress(address.trimmed());
        if (peer.address.isNull())
            return fail(QString::fromUtf8("发送"), QString::fromUtf8("UDP 目标必须是有效 IP 地址"));
        if (!parsePort(portText, peer.port))
            return false;
    }
    m_control.send(channel(), bytes, peer);
    m_submittedBytes += static_cast<qulonglong>(bytes.size());
    emit countersChanged();
    record(channel(), QString::fromUtf8("TX请求"), bytes);
    return true; // Queued request accepted, not a delivery acknowledgement.
}
void CommunicationPresenter::receiveBytes(Channel source, const QByteArray &bytes, const Endpoint &peer) {
    m_receivedBytes += static_cast<qulonglong>(bytes.size());
    emit countersChanged();
    QString kind = QStringLiteral("RX");
    if (peer.isValid())
        kind += QStringLiteral(" %1:%2").arg(peer.address.toString()).arg(peer.port);
    record(source, kind, bytes);
    emit dataReceived(static_cast<int>(source), bytes);
}
void CommunicationPresenter::updateState(Channel source, ConnectionState state) {
    const int index = static_cast<int>(source);
    if (m_states[index] == state)
        return;
    m_states[index] = state;
    if (index == m_selectedChannel)
        emit connectionChanged();
}
void CommunicationPresenter::handleError(const CommunicationError &error) {
    // Failed open attempts must release the UI's opening state, even when the
    // underlying serial/UDP implementation does not emit Closed on failure.
    if (m_states[static_cast<int>(error.channel)] == ConnectionState::Opening) {
        m_control.close(error.channel);
        updateState(error.channel, ConnectionState::Closed);
    }
    m_lastError = QString::fromUtf8("%1 · %2: %3 (code %4)")
            .arg(channelName(error.channel), error.operation, error.message).arg(error.nativeCode);
    emit lastErrorChanged();
    record(error.channel, QString::fromUtf8("错误"), QByteArray(), m_lastError);
}
bool CommunicationPresenter::fail(const QString &operation, const QString &message) {
    m_lastError = QString::fromUtf8("%1 · %2: %3").arg(channelName(channel()), operation, message);
    emit lastErrorChanged();
    record(channel(), QString::fromUtf8("错误"), QByteArray(), m_lastError);
    return false;
}
void CommunicationPresenter::record(Channel source, const QString &kind,
                                     const QByteArray &bytes, const QString &message) {
    LogEntry entry;
    entry.prefix = QStringLiteral("[%1] %2 %3 (%4 B) ")
            .arg(QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss.zzz")),
                 channelName(source), kind).arg(bytes.size());
    // Retain bounded display history. Original bytes are emitted intact above.
    entry.bytes = bytes.left(4096);
    entry.message = message;
    if (bytes.size() > entry.bytes.size())
        entry.message += QString::fromUtf8(" [显示截断，原始共 %1 B]").arg(bytes.size());
    m_logBytes += entry.bytes.size();
    m_log.append(entry);
    while (m_log.size() > 300 || m_logBytes > 256 * 1024)
        m_logBytes -= m_log.takeFirst().bytes.size();
    emit activityLogChanged();
}
QString CommunicationPresenter::activityLog() const {
    QStringList lines;
    for (const LogEntry &entry : m_log) {
        QString payload = m_hexDisplay ? QString::fromLatin1(entry.bytes.toHex(' ').toUpper())
                                       : QString::fromUtf8(entry.bytes.constData(), entry.bytes.size());
        if (!m_hexDisplay) {
            payload.replace(QChar('\0'), QStringLiteral("\\0"));
            payload.replace(QChar('\r'), QStringLiteral("\\r"));
        }
        lines.append(entry.prefix + payload + entry.message);
    }
    return lines.join(QChar('\n'));
}
void CommunicationPresenter::setHexDisplay(bool value) {
    if (m_hexDisplay == value)
        return;
    m_hexDisplay = value;
    emit hexDisplayChanged();
    emit activityLogChanged();
}
void CommunicationPresenter::clearLog() {
    m_log.clear();
    m_logBytes = 0;
    emit activityLogChanged();
}
void CommunicationPresenter::resetCounters() {
    m_receivedBytes = 0;
    m_submittedBytes = 0;
    emit countersChanged();
}

} // namespace assistant
} // namespace fosd
