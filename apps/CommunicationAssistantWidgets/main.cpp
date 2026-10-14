#include "communication-assistant-QML/CommunicationPresenter.h"
#include "CommunicationAssistantWidgets/CommunicationWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("CommunicationAssistantWidgets"));
    app.setOrganizationName(QStringLiteral("FOSD"));
    app.setStyle(QStringLiteral("Fusion"));
    fosd::assistant::CommunicationPresenter presenter;
    fosd::assistant::CommunicationWindow window(presenter);
    window.show();
    return app.exec();
}
