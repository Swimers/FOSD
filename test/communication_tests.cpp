#include "communication/ConnectControl.h"
#include "communication/PacketDecoders.h"

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QUdpSocket>
#include <QtTest>
#include <atomic>
#include <functional>

using namespace fosd::communication;

class PassthroughDecoder final : public IPacketDecoder {
public:
    PassthroughDecoder() : IPacketDecoder(Channel::Udp) {}
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &) override {
        return QVector<QByteArray>() << bytes;
    }
};

namespace {

std::unique_ptr<IPacketDecoder> createInvalidDecoder(Channel channel) {
    switch (channel) {
    case Channel::Serial:
        return std::unique_ptr<IPacketDecoder>(new SerialPacketDecoder(QByteArray(), 1));
    case Channel::Udp:
        return std::unique_ptr<IPacketDecoder>(new UdpPacketDecoder(QVector<PacketRule>()));
    case Channel::Tcp:
        return std::unique_ptr<IPacketDecoder>(new TcpPacketDecoder({PacketRule()}));
    }
    return nullptr;
}

std::unique_ptr<IPacketDecoder> createPassthroughDecoder(
    Channel channel, std::atomic<bool> *factoryOnWorker, QThread *mainThread) {
    factoryOnWorker->store(QThread::currentThread() != mainThread);
    if (channel == Channel::Udp)
        return std::unique_ptr<IPacketDecoder>(new PassthroughDecoder);
    return nullptr;
}

} // namespace

class CommunicationTests : public QObject {
    Q_OBJECT

private slots:
    void udpRoundTripAndDecoder();
    void udpDefaultDecoder();
    void tcpDefaultDecoderAndReconnect();
    void tcpClientRoundTrip();
    void tcpServerRoundTrip();
    void invalidSerialConfiguration();
    void sendOnClosedSerialReportsError();
    void invalidDecoderConfigurationReportsErrors();
};

void CommunicationTests::invalidDecoderConfigurationReportsErrors() {
    ConnectControl control(nullptr, createInvalidDecoder);
    QSignalSpy errorSpy(&control, &ConnectControl::decoderErrorOccurred);
    QSignalSpy transportErrorSpy(&control, &ConnectControl::errorOccurred);
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy rawSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy packetSpy(&control, &ConnectControl::packetReceived);
    QTRY_COMPARE(errorSpy.count(), 3);
    const QVector<Channel> channels = {Channel::Serial, Channel::Udp, Channel::Tcp};
    for (int index = 0; index < channels.size(); ++index) {
        const DecoderError error = qvariant_cast<DecoderError>(errorSpy.at(index).at(0));
        QCOMPARE(error.channel, channels.at(index));
        QCOMPARE(error.operation, QStringLiteral("initialize"));
        QVERIFY(!error.message.isEmpty());
        QCOMPARE(error.code, 0);
    }
    // Configuration failure is reported once, and transport I/O stays usable.
    UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = 0;
    control.openUdp(config);
    QTRY_VERIFY(!stateSpy.isEmpty());
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Ready);
    QCOMPARE(errorSpy.count(), 3);
    QCOMPARE(transportErrorSpy.count(), 0);
    QCOMPARE(packetSpy.count(), 0);
    QCOMPARE(rawSpy.count(), 0);
}

void CommunicationTests::udpDefaultDecoder() {
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 port = probe.localPort();
    probe.close();
    ConnectControl control;
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy rawSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy packetSpy(&control, &ConnectControl::packetReceived);
    QSignalSpy decoderErrorSpy(&control, &ConnectControl::decoderErrorOccurred);
    UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = port;
    control.openUdp(config);
    QTRY_VERIFY(!stateSpy.isEmpty());
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Ready);
    QUdpSocket peer;
    const QByteArray packet = QByteArray::fromHex("5aa50100000001000000");
    QCOMPARE(peer.writeDatagram("invalid", 7, QHostAddress::LocalHost, port), qint64(7));
    QCOMPARE(peer.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
    QTRY_COMPARE(rawSpy.count(), 2);
    QTRY_COMPARE(packetSpy.count(), 1);
    QTRY_COMPARE(decoderErrorSpy.count(), 1);
    const DecoderError decoderError = qvariant_cast<DecoderError>(decoderErrorSpy.first().at(0));
    QCOMPARE(decoderError.channel, Channel::Udp);
    QCOMPARE(decoderError.operation, QStringLiteral("decode"));
    QCOMPARE(packetSpy.first().at(1).toByteArray(), packet);
    const Endpoint source = qvariant_cast<Endpoint>(packetSpy.first().at(2));
    QCOMPARE(source.port, peer.localPort());
    QVERIFY(source.address == QHostAddress(QHostAddress::LocalHost));
}

void CommunicationTests::tcpDefaultDecoderAndReconnect() {
    QTcpServer probe;
    QVERIFY(probe.listen(QHostAddress::LocalHost, 0));
    const quint16 port = probe.serverPort();
    probe.close();
    ConnectControl control;
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy rawSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy packetSpy(&control, &ConnectControl::packetReceived);
    TcpConfig config;
    config.role = TcpRole::Server;
    config.listenAddress = QHostAddress::LocalHost;
    config.port = port;
    control.openTcp(config);
    QTRY_VERIFY(!stateSpy.isEmpty());
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Listening);
    QTcpSocket peer;
    peer.connectToHost(QHostAddress::LocalHost, port);
    QVERIFY(peer.waitForConnected(3000));
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) == ConnectionState::Connected);
    const QByteArray packet = QByteArray::fromHex("5aa50100000001000000");
    QCOMPARE(peer.write(packet.left(1)), qint64(1));
    QTRY_COMPARE(rawSpy.count(), 1);
    QCOMPARE(packetSpy.count(), 0);
    const QByteArray rest = packet.mid(1) + packet;
    QCOMPARE(peer.write(rest), qint64(rest.size()));
    QTRY_COMPARE(packetSpy.count(), 2);
    QCOMPARE(packetSpy.first().at(1).toByteArray(), packet);
    const int received = rawSpy.count();
    QCOMPARE(peer.write(packet.left(8)), qint64(8));
    QTRY_VERIFY(rawSpy.count() > received);
    peer.disconnectFromHost();
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) == ConnectionState::Listening);
    peer.connectToHost(QHostAddress::LocalHost, port);
    QVERIFY(peer.waitForConnected(3000));
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) == ConnectionState::Connected);
    QCOMPARE(peer.write(packet), qint64(packet.size()));
    QTRY_COMPARE(packetSpy.count(), 3);
    QCOMPARE(packetSpy.last().at(1).toByteArray(), packet);
}

void CommunicationTests::udpRoundTripAndDecoder() {
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 controlPort = probe.localPort();
    probe.close();

    QUdpSocket peer;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));

    std::atomic<bool> factoryOnWorker(false);
    QThread *mainThread = QCoreApplication::instance()->thread();
    ConnectControl control(nullptr, std::bind(createPassthroughDecoder,
                                             std::placeholders::_1,
                                             &factoryOnWorker, mainThread));
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy dataSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy packetSpy(&control, &ConnectControl::packetReceived);
    QSignalSpy errorSpy(&control, &ConnectControl::errorOccurred);
    UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = controlPort;
    control.openUdp(config);
    QTRY_VERIFY(stateSpy.count() > 0);
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Ready);
    QVERIFY(factoryOnWorker.load());

    Endpoint target;
    target.address = QHostAddress::LocalHost;
    target.port = peer.localPort();
    control.send(Channel::Udp, QByteArray("ping"), target);
    QTRY_VERIFY(peer.hasPendingDatagrams());
    QByteArray sent(16, Qt::Uninitialized);
    sent.resize(static_cast<int>(peer.readDatagram(sent.data(), sent.size())));
    QCOMPARE(sent, QByteArray("ping"));

    QCOMPARE(peer.writeDatagram("pong", 4, QHostAddress::LocalHost, controlPort), qint64(4));
    QTRY_VERIFY(dataSpy.count() > 0);
    QCOMPARE(qvariant_cast<Channel>(dataSpy.last().at(0)), Channel::Udp);
    QCOMPARE(dataSpy.last().at(1).toByteArray(), QByteArray("pong"));
    QTRY_VERIFY(packetSpy.count() > 0);
    QCOMPARE(packetSpy.last().at(1).toByteArray(), QByteArray("pong"));
    QCOMPARE(errorSpy.count(), 0);
}

void CommunicationTests::tcpClientRoundTrip() {
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    ConnectControl control;
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy dataSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy errorSpy(&control, &ConnectControl::errorOccurred);
    TcpConfig config;
    config.role = TcpRole::Client;
    config.host = QStringLiteral("127.0.0.1");
    config.port = server.serverPort();
    control.openTcp(config);
    QTRY_VERIFY(server.hasPendingConnections());
    QTcpSocket *peer = server.nextPendingConnection();
    QVERIFY(peer != nullptr);
    QTRY_VERIFY(stateSpy.count() >= 2);
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Connected);

    control.send(Channel::Tcp, QByteArray("ping"));
    QTRY_VERIFY(peer->bytesAvailable() >= 4);
    QCOMPARE(peer->readAll(), QByteArray("ping"));

    QCOMPARE(peer->write("pong", 4), qint64(4));
    QTRY_VERIFY(dataSpy.count() > 0);
    QCOMPARE(qvariant_cast<Channel>(dataSpy.last().at(0)), Channel::Tcp);
    QCOMPARE(dataSpy.last().at(1).toByteArray(), QByteArray("pong"));
    QCOMPARE(errorSpy.count(), 0);

    control.close(Channel::Tcp);
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) == ConnectionState::Closed);
}

void CommunicationTests::tcpServerRoundTrip() {
    QTcpServer probe;
    QVERIFY(probe.listen(QHostAddress::LocalHost, 0));
    const quint16 port = probe.serverPort();
    probe.close();

    ConnectControl control;
    QSignalSpy stateSpy(&control, &ConnectControl::stateChanged);
    QSignalSpy dataSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy errorSpy(&control, &ConnectControl::errorOccurred);
    TcpConfig config;
    config.role = TcpRole::Server;
    config.listenAddress = QHostAddress::LocalHost;
    config.port = port;
    control.openTcp(config);
    QTRY_VERIFY(stateSpy.count() > 0);
    QCOMPARE(qvariant_cast<ConnectionState>(stateSpy.last().at(1)), ConnectionState::Listening);

    QTcpSocket peer;
    peer.connectToHost(QHostAddress::LocalHost, port);
    QVERIFY(peer.waitForConnected(3000));
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) ==
                ConnectionState::Connected);

    QCOMPARE(peer.write("hello", 5), qint64(5));
    QTRY_VERIFY(dataSpy.count() > 0);
    QCOMPARE(dataSpy.last().at(1).toByteArray(), QByteArray("hello"));

    control.send(Channel::Tcp, QByteArray("world"));
    QTRY_VERIFY(peer.bytesAvailable() >= 5);
    QCOMPARE(peer.readAll(), QByteArray("world"));
    QCOMPARE(errorSpy.count(), 0);

    peer.disconnectFromHost();
    QTRY_VERIFY(qvariant_cast<ConnectionState>(stateSpy.last().at(1)) ==
                ConnectionState::Listening);
}

void CommunicationTests::invalidSerialConfiguration() {
    ConnectControl control;
    QSignalSpy errorSpy(&control, &ConnectControl::errorOccurred);
    control.openSerial(SerialConfig());
    QTRY_VERIFY(errorSpy.count() > 0);
    const CommunicationError error =
        qvariant_cast<CommunicationError>(errorSpy.last().at(0));
    QCOMPARE(error.channel, Channel::Serial);
    QCOMPARE(error.operation, QStringLiteral("open"));
}

void CommunicationTests::sendOnClosedSerialReportsError() {
    ConnectControl control;
    QSignalSpy dataSpy(&control, &ConnectControl::rawDataReceived);
    QSignalSpy errorSpy(&control, &ConnectControl::errorOccurred);
    control.send(Channel::Serial, QByteArray("ping"));
    QTRY_VERIFY(errorSpy.count() > 0);
    const CommunicationError error =
        qvariant_cast<CommunicationError>(errorSpy.last().at(0));
    QCOMPARE(error.channel, Channel::Serial);
    QCOMPARE(error.operation, QStringLiteral("send"));
    QCOMPARE(dataSpy.count(), 0);
}

QTEST_MAIN(CommunicationTests)
#include "communication_tests.moc"

