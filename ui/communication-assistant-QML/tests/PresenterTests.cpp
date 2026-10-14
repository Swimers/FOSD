#include "communication-assistant-QML/CommunicationPresenter.h"
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QtTest>

using fosd::assistant::CommunicationPresenter;

class PresenterTests final : public QObject {
    Q_OBJECT
private slots:
    void validation();
    void udpRawRoundTrip();
    void tcpClientRoundTrip();
    void tcpServerRoundTrip();
    void transportFailure();
    void boundedHistory();
};

namespace {
quint16 availableTcpPort() {
    QTcpServer reservation;
    if (!reservation.listen(QHostAddress::LocalHost, 0))
        return 0;
    return reservation.serverPort();
}
quint16 availableUdpPort() {
    QUdpSocket reservation;
    if (!reservation.bind(QHostAddress(QHostAddress::LocalHost), 0))
        return 0;
    return reservation.localPort();
}
QVariantMap udpSettings(quint16 port) {
    QVariantMap settings;
    settings.insert(QStringLiteral("bindAddress"), QStringLiteral("127.0.0.1"));
    settings.insert(QStringLiteral("localPort"), QString::number(port));
    return settings;
}
QVariantMap tcpSettings(quint16 port, bool server) {
    QVariantMap settings;
    settings.insert(QStringLiteral("host"), QStringLiteral("127.0.0.1"));
    settings.insert(QStringLiteral("port"), QString::number(port));
    settings.insert(QStringLiteral("tcpRole"), server ? 1 : 0);
    return settings;
}
}

void PresenterTests::validation() {
    CommunicationPresenter presenter;
    QVERIFY(!presenter.sendData(QStringLiteral("a"), false, false, QString(), QString()));
    QVERIFY(!presenter.lastError().isEmpty());
    presenter.setSelectedChannel(99);
    QCOMPARE(presenter.selectedChannel(), 0);
    QVERIFY(!presenter.openConnection(QVariantMap()));
    presenter.setSelectedChannel(1);
    QVERIFY(!presenter.openConnection(udpSettings(0)));
    QVariantMap bad = udpSettings(9000);
    bad[QStringLiteral("localPort")] = QStringLiteral("65536");
    QVERIFY(!presenter.openConnection(bad));
    bad = udpSettings(9000);
    bad[QStringLiteral("bindAddress")] = QStringLiteral("invalid");
    QVERIFY(!presenter.openConnection(bad));
    QVERIFY(!presenter.isOpen());
    presenter.setSelectedChannel(2);
    bad = tcpSettings(9000, true);
    bad[QStringLiteral("host")] = QStringLiteral("not-an-ip");
    QVERIFY(!presenter.openConnection(bad));
}

void PresenterTests::udpRawRoundTrip() {
    CommunicationPresenter presenter;
    QUdpSocket peer;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), 0));
    const quint16 localPort = availableUdpPort();
    QVERIFY(localPort != 0);
    presenter.setSelectedChannel(1);
    QVERIFY(presenter.openConnection(udpSettings(localPort)));
    QTRY_VERIFY(presenter.canSend());
    QSignalSpy received(&presenter, &CommunicationPresenter::dataReceived);
    // Arbitrary bytes: no header/terminator, embedded NUL, invalid protocol data.
    const QByteArray raw = QByteArray::fromHex("00ffa55a010d0200");
    QCOMPARE(peer.writeDatagram(raw, QHostAddress::LocalHost, localPort), qint64(raw.size()));
    QTRY_COMPARE(received.count(), 1);
    QCOMPARE(received.at(0).at(0).toInt(), 1);
    QCOMPARE(received.at(0).at(1).toByteArray(), raw);
    QCOMPARE(presenter.receivedBytes(), qulonglong(raw.size()));
    QVERIFY(presenter.activityLog().contains(QStringLiteral("00 FF A5 5A 01 0D 02 00")));
    presenter.setHexDisplay(false);
    QVERIFY(presenter.activityLog().contains(QStringLiteral("\\0")));
    const QString remotePort = QString::number(peer.localPort());
    QVERIFY(!presenter.sendData(QStringLiteral("0xAA"), true, false, "127.0.0.1", remotePort));
    QVERIFY(!presenter.sendData(QStringLiteral("ABC"), true, false, "127.0.0.1", remotePort));
    QVERIFY(!presenter.sendData(QString(), false, false, "127.0.0.1", remotePort));
    QVERIFY(!presenter.sendData("AA", true, false, "bad", remotePort));
    QCOMPARE(presenter.submittedBytes(), qulonglong(0));
    QVERIFY(presenter.sendData(QStringLiteral("00 a5\nFF"), true, true, "127.0.0.1", remotePort));
    QTRY_VERIFY(peer.hasPendingDatagrams());
    QByteArray sent(64, '\0');
    sent.resize(int(peer.readDatagram(sent.data(), sent.size())));
    QCOMPARE(sent, QByteArray::fromHex("00a5ff0d0a"));
    QCOMPARE(presenter.submittedBytes(), qulonglong(sent.size()));
    // Switching the selected view keeps the previous channel connected.
    presenter.setSelectedChannel(0);
    QVERIFY(!presenter.isOpen());
    presenter.setSelectedChannel(1);
    QVERIFY(presenter.canSend());
    presenter.clearLog();
    QVERIFY(presenter.activityLog().isEmpty());
    QVERIFY(presenter.receivedBytes() > 0);
    presenter.resetCounters();
    QCOMPARE(presenter.receivedBytes(), qulonglong(0));
    QCOMPARE(presenter.submittedBytes(), qulonglong(0));
    presenter.closeConnection();
    QTRY_VERIFY(!presenter.isOpen());
}

void PresenterTests::tcpClientRoundTrip() {
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));
    CommunicationPresenter presenter;
    presenter.setSelectedChannel(2);
    QVERIFY(presenter.openConnection(tcpSettings(server.serverPort(), false)));
    QTRY_VERIFY(server.hasPendingConnections());
    QScopedPointer<QTcpSocket> socket(server.nextPendingConnection());
    QTRY_VERIFY(presenter.canSend());
    QSignalSpy received(&presenter, &CommunicationPresenter::dataReceived);
    const QByteArray raw = QByteArray::fromHex("01ff000d");
    QCOMPARE(socket->write(raw), qint64(raw.size()));
    QTRY_COMPARE(presenter.receivedBytes(), qulonglong(raw.size()));
    QByteArray assembled;
    for (const QList<QVariant> &event : received)
        assembled.append(event.at(1).toByteArray());
    QCOMPARE(assembled, raw);
    const QString text = QString::fromUtf8("通信测试");
    QVERIFY(presenter.sendData(text, false, false, QString(), QString()));
    QTRY_VERIFY(socket->bytesAvailable() >= text.toUtf8().size());
    QCOMPARE(socket->readAll(), text.toUtf8());
    presenter.closeConnection();
    QTRY_VERIFY(socket->state() == QAbstractSocket::UnconnectedState);
}

void PresenterTests::tcpServerRoundTrip() {
    CommunicationPresenter presenter;
    const quint16 port = availableTcpPort();
    QVERIFY(port != 0);
    presenter.setSelectedChannel(2);
    QVERIFY(presenter.openConnection(tcpSettings(port, true)));
    QTRY_VERIFY(presenter.statusText().contains(QString::fromUtf8("监听")));
    QVERIFY(!presenter.canSend());
    QTcpSocket peer;
    peer.connectToHost(QHostAddress::LocalHost, port);
    QTRY_VERIFY(presenter.canSend());
    const QByteArray raw("no framing\0\r", 12);
    QCOMPARE(peer.write(raw), qint64(raw.size()));
    QTRY_COMPARE(presenter.receivedBytes(), qulonglong(raw.size()));
    QVERIFY(presenter.sendData("FF 00", true, false, QString(), QString()));
    QTRY_VERIFY(peer.bytesAvailable() >= 2);
    QCOMPARE(peer.readAll(), QByteArray::fromHex("ff00"));
    peer.disconnectFromHost();
    QTRY_VERIFY(presenter.statusText().contains(QString::fromUtf8("监听")));
    QVERIFY(!presenter.canSend());
    presenter.closeConnection();
    QVERIFY(!presenter.isOpen());
}

void PresenterTests::transportFailure() {
    CommunicationPresenter presenter;
    QUdpSocket occupied;
    QVERIFY(occupied.bind(QHostAddress(QHostAddress::LocalHost), 0, QUdpSocket::DontShareAddress));
    presenter.setSelectedChannel(1);
    QVERIFY(presenter.openConnection(udpSettings(occupied.localPort())));
    QTRY_VERIFY(!presenter.lastError().isEmpty());
    QTRY_VERIFY(!presenter.isOpen());
    QVERIFY(presenter.activityLog().contains(QString::fromUtf8("错误")));
    // Also covers the serial open failure path without requiring real hardware.
    presenter.setSelectedChannel(0);
    QVariantMap serial;
    serial["portName"] = QStringLiteral("FOSD_NONEXISTENT_SERIAL_PORT");
    serial["baudRate"] = QStringLiteral("115200");
    QVERIFY(presenter.openConnection(serial));
    QTRY_VERIFY(!presenter.isOpen());
    QVERIFY(!presenter.lastError().isEmpty());
}

void PresenterTests::boundedHistory() {
    CommunicationPresenter presenter;
    for (int i = 0; i < 350; ++i)
        QVERIFY(!presenter.openConnection(QVariantMap()));
    QCOMPARE(presenter.activityLog().count(QChar('\n')), 299);
}

QTEST_GUILESS_MAIN(PresenterTests)
#include "PresenterTests.moc"
