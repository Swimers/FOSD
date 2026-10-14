#include "CommunicationAssistantWidgets/CommunicationWindow.h"
#include "CommunicationAssistant/CommunicationPresenter.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QtTest>

using namespace fosd::assistant;
class WidgetsTests final : public QObject {
    Q_OBJECT
private slots:
    void udpViewRoundTrip();
    void tcpViewServer();
    void serialFailureAndValidation();
};

void WidgetsTests::udpViewRoundTrip() {
    CommunicationPresenter presenter;
    CommunicationWindow window(presenter);
    window.show();
    QComboBox *channel = window.findChild<QComboBox *>("channel");
    QPushButton *connection = window.findChild<QPushButton *>("connectionButton");
    QPushButton *send = window.findChild<QPushButton *>("sendButton");
    QVERIFY(channel && connection && send);
    QVERIFY(!send->isEnabled());
    QUdpSocket peer, reservation;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), 0));
    QVERIFY(reservation.bind(QHostAddress(QHostAddress::LocalHost), 0));
    const quint16 port = reservation.localPort();
    reservation.close();
    channel->setCurrentIndex(1);
    QCOMPARE(presenter.selectedChannel(), 1);
    window.findChild<QLineEdit *>("udpBind")->setText("127.0.0.1");
    QLineEdit *localPort = window.findChild<QLineEdit *>("udpLocalPort");
    localPort->setText(QString::number(port));
    window.findChild<QLineEdit *>("udpPeerPort")->setText(QString::number(peer.localPort()));
    connection->click();
    QTRY_VERIFY(presenter.canSend());
    QVERIFY(send->isEnabled());
    QVERIFY(!localPort->isEnabled());
    QPlainTextEdit *sendText = window.findChild<QPlainTextEdit *>("sendText");
    window.findChild<QCheckBox *>("hexSend")->setChecked(true);
    window.findChild<QCheckBox *>("appendCrLf")->setChecked(true);
    sendText->setPlainText("00 A5 FF");
    send->click();
    QTRY_VERIFY(peer.hasPendingDatagrams());
    QByteArray bytes(64, '\0');
    bytes.resize(int(peer.readDatagram(bytes.data(), bytes.size())));
    QCOMPARE(bytes, QByteArray::fromHex("00a5ff0d0a"));
    const QByteArray raw = QByteArray::fromHex("00ff0da55a");
    QCOMPARE(peer.writeDatagram(raw, QHostAddress::LocalHost, port), qint64(raw.size()));
    QTRY_COMPARE(presenter.receivedBytes(), qulonglong(raw.size()));
    QPlainTextEdit *log = window.findChild<QPlainTextEdit *>("activityLog");
    QVERIFY(log->isReadOnly());
    QVERIFY(log->toPlainText().contains("00 FF 0D A5 5A"));
    window.findChild<QCheckBox *>("hexDisplay")->setChecked(false);
    QVERIFY(log->toPlainText().contains("\\0"));
    channel->setCurrentIndex(0);
    QVERIFY(!send->isEnabled());
    channel->setCurrentIndex(1);
    QVERIFY(send->isEnabled());
    window.findChild<QPushButton *>("clearLog")->click();
    QVERIFY(log->toPlainText().isEmpty());
    QCOMPARE(presenter.receivedBytes(), qulonglong(raw.size()));
    window.findChild<QPushButton *>("resetCounters")->click();
    QCOMPARE(presenter.receivedBytes(), qulonglong(0));
    QCOMPARE(presenter.submittedBytes(), qulonglong(0));
    connection->click();
    QTRY_VERIFY(!presenter.isOpen());
    QVERIFY(localPort->isEnabled());
    channel->setCurrentIndex(0);
    QTest::qWait(100);
    QVERIFY(window.grab().save(QCoreApplication::applicationDirPath() + "/assistant-widgets-preview.png"));
}
void WidgetsTests::tcpViewServer() {
    CommunicationPresenter presenter;
    CommunicationWindow window(presenter);
    QTcpServer reservation;
    QVERIFY(reservation.listen(QHostAddress::LocalHost, 0));
    const quint16 port = reservation.serverPort();
    reservation.close();
    window.findChild<QComboBox *>("channel")->setCurrentIndex(2);
    window.findChild<QComboBox *>("tcpRole")->setCurrentIndex(1);
    window.findChild<QLineEdit *>("tcpPort")->setText(QString::number(port));
    QPushButton *connection = window.findChild<QPushButton *>("connectionButton");
    QPushButton *send = window.findChild<QPushButton *>("sendButton");
    connection->click();
    QTRY_VERIFY(presenter.statusText().contains(QString::fromUtf8("监听")));
    QVERIFY(!send->isEnabled());
    QTcpSocket peer;
    peer.connectToHost(QHostAddress::LocalHost, port);
    QTRY_VERIFY(send->isEnabled());
    window.findChild<QPlainTextEdit *>("sendText")->setPlainText(QString::fromUtf8("测试"));
    send->click();
    const QByteArray text = QString::fromUtf8("测试").toUtf8();
    QTRY_VERIFY(peer.bytesAvailable() >= text.size());
    QCOMPARE(peer.readAll(), text);
    QCOMPARE(peer.write("raw\0", 4), qint64(4));
    QTRY_COMPARE(presenter.receivedBytes(), qulonglong(4));
    peer.disconnectFromHost();
    QTRY_VERIFY(!send->isEnabled());
    QVERIFY(presenter.statusText().contains(QString::fromUtf8("监听")));
    connection->click();
    QVERIFY(!presenter.isOpen());
}
void WidgetsTests::serialFailureAndValidation() {
    CommunicationPresenter presenter;
    CommunicationWindow window(presenter);
    QComboBox *port = window.findChild<QComboBox *>("serialPort");
    port->setEditText("FOSD_NONEXISTENT_SERIAL_PORT");
    window.findChild<QComboBox *>("baudRate")->setEditText("invalid");
    QPushButton *connection = window.findChild<QPushButton *>("connectionButton");
    connection->click();
    QVERIFY(!presenter.isOpen());
    QLabel *error = window.findChild<QLabel *>("errorMessage");
    QCOMPARE(error->text(), presenter.lastError());
    QVERIFY(!error->text().isEmpty());
    window.findChild<QComboBox *>("baudRate")->setEditText("115200");
    connection->click();
    QTRY_VERIFY(!presenter.isOpen());
    QVERIFY(port->isEnabled());
    QCOMPARE(error->text(), presenter.lastError());
}
QTEST_MAIN(WidgetsTests)
#include "WidgetsTests.moc"
