#include "CommunicationAssistant/CommunicationPresenter.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("CommunicationAssistant"));
    app.setOrganizationName(QStringLiteral("FOSD"));
    QQuickStyle::setStyle(QStringLiteral("Material"));
    fosd::assistant::CommunicationPresenter presenter;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("presenter"), &presenter);
    engine.load(QUrl(QStringLiteral("qrc:/communication-assistant/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return 1;
    return app.exec();
}
