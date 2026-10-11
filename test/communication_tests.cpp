#include "communication/ConnectControl.h"

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QUdpSocket>
#include <QtTest>
#include <atomic>

using namespace fosd::communication;

class PassthroughDecoder final : public IPacketDecoder {
public:
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &) override {
        return QVector<QByteArray>() << bytes;
    }
};

class CommunicationTests : public QObject {
    Q_OBJECT

private slots:
    void udpRoundTripAndDecoder();
    void tcpClientRoundTrip();
    void tcpServerRoundTrip();
    void invalidSerialConfiguration();
    void sendOnClosedSerialReportsError();
};

void CommunicationTests::udpRoundTripAndDecoder() {
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 controlPort = probe.localPort();
    probe.close();

    QUdpSocket peer;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));

    std::atomic<bool> factoryOnWorker(false);
    QThread *mainThread = QCoreApplication::instance()->thread();
    ConnectControl control(nullptr, [&factoryOnWorker, mainThread](Channel channel)
                           -> std::unique_ptr<IPacketDecoder> {
        factoryOnWorker.store(QThread::currentThread() != mainThread);
        if (channel == Channel::Udp)
            return std::unique_ptr<IPacketDecoder>(new PassthroughDecoder);
        return std::unique_ptr<IPacketDecoder>();
    });
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

