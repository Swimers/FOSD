#include "communication-assistant-QML/CommunicationPresenter.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlError>
#include <QQuickItem>
#include <QQuickRenderControl>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QSignalSpy>
#include <QUdpSocket>
#include <QtTest>

Q_DECLARE_METATYPE(QList<QQmlError>)

class QmlTests final : public QObject {
    Q_OBJECT
private slots:
    void viewLoadsAndSwitchesChannels();
};

void QmlTests::viewLoadsAndSwitchesChannels() {
    QQuickStyle::setStyle(QStringLiteral("Material"));
    qRegisterMetaType<QList<QQmlError>>();
    QQuickRenderControl renderControl;
    QQuickWindow renderWindow(&renderControl);
    fosd::assistant::CommunicationPresenter presenter;
    QQmlApplicationEngine engine;
    QSignalSpy warnings(&engine, &QQmlEngine::warnings);
    engine.rootContext()->setContextProperty(QStringLiteral("presenter"), &presenter);
    engine.load(QUrl(QStringLiteral("qrc:/communication-assistant-QML/Main.qml")));
    QCOMPARE(engine.rootObjects().size(), 1);
    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
    QVERIFY(window);
    QObject *serial = window->findChild<QObject *>(QStringLiteral("serialSettings"));
    QObject *udp = window->findChild<QObject *>(QStringLiteral("udpSettings"));
    QObject *tcp = window->findChild<QObject *>(QStringLiteral("tcpSettings"));
    QObject *send = window->findChild<QObject *>(QStringLiteral("sendButton"));
    QVERIFY(serial && udp && tcp && send);
    QVERIFY(serial->property("visible").toBool());
    QVERIFY(!udp->property("visible").toBool());
    QVERIFY(!send->property("enabled").toBool());
    presenter.setSelectedChannel(1);
    QVERIFY(udp->property("visible").toBool());
    QVERIFY(!serial->property("visible").toBool());
    // Exercise View -> Presenter -> real UDP I/O, including QML form mapping.
    QUdpSocket peer;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), 0));
    QUdpSocket reservation;
    QVERIFY(reservation.bind(QHostAddress(QHostAddress::LocalHost), 0));
    const quint16 localPort = reservation.localPort();
    reservation.close();
    QObject *bind = window->findChild<QObject *>(QStringLiteral("udpBind"));
    QObject *port = window->findChild<QObject *>(QStringLiteral("udpLocalPort"));
    QObject *peerPort = window->findChild<QObject *>(QStringLiteral("udpPeerPort"));
    QObject *sendText = window->findChild<QObject *>(QStringLiteral("sendText"));
    QObject *connection = window->findChild<QObject *>(QStringLiteral("connectionButton"));
    QVERIFY(bind && port && peerPort && sendText && connection);
    bind->setProperty("text", QStringLiteral("127.0.0.1"));
    port->setProperty("text", QString::number(localPort));
    peerPort->setProperty("text", QString::number(peer.localPort()));
    QVERIFY(QMetaObject::invokeMethod(connection, "clicked"));
    QTRY_VERIFY(presenter.canSend());
    QVERIFY(send->property("enabled").toBool());
    sendText->setProperty("text", QStringLiteral("raw UI test"));
    QVERIFY(QMetaObject::invokeMethod(send, "clicked"));
    QTRY_VERIFY(peer.hasPendingDatagrams());
    QByteArray bytes(64, '\0');
    bytes.resize(int(peer.readDatagram(bytes.data(), bytes.size())));
    QCOMPARE(bytes, QByteArray("raw UI test"));
    QCOMPARE(peer.writeDatagram("reply", QHostAddress::LocalHost, localPort), qint64(5));
    QTRY_COMPARE(presenter.receivedBytes(), qulonglong(5));
    QVERIFY(QMetaObject::invokeMethod(connection, "clicked"));
    QTRY_VERIFY(!presenter.isOpen());
    presenter.setSelectedChannel(2);
    QVERIFY(tcp->property("visible").toBool());
    QVERIFY(!udp->property("visible").toBool());
    presenter.setSelectedChannel(0);
    // Qt 5.12's offscreen platform cannot grab a native software window.
    // Render the real view content through a software render control instead.
    window->hide();
    renderWindow.setGeometry(0, 0, window->width(), window->height());
    renderWindow.setColor(window->color());
    window->contentItem()->setParentItem(renderWindow.contentItem());
    renderControl.initialize(nullptr);
    renderControl.polishItems();
    QVERIFY(sendText->property("width").toDouble() > 500);
    QVERIFY(sendText->property("height").toDouble() > 60);
    renderControl.sync();
    renderControl.render();
    const QImage image = renderControl.grab();
    QVERIFY(!image.isNull());
    QVERIFY(image.save(QCoreApplication::applicationDirPath() +
                       QStringLiteral("/assistant-QML-preview.png")));
    renderControl.invalidate();
    QCOMPARE(warnings.count(), 0);
}

QTEST_MAIN(QmlTests)
#include "QmlTests.moc"
