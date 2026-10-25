#include "ConnectionBackend.h"
namespace fosd {
namespace ui {
namespace classic {
ConnectionBackend::ConnectionBackend(QObject *parent)
    : FeatureBackend({PanelId::Connection, PanelId::ConnectionDebug}, parent) {
}

void ConnectionBackend::handleRequest(const UiRequest &request) {
    reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
