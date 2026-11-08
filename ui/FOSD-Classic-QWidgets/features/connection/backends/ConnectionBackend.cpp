#include "FOSD-Classic-QWidgets/features/connection/backends/ConnectionBackend.h"
namespace fosd {
namespace ui {
namespace classic {
ConnectionBackend::ConnectionBackend(QObject *parent)
    : FeatureBackend({PanelId::Connection, PanelId::ChannelCalibration}, parent) {
}

void ConnectionBackend::handleRequest(const UiRequest &request) {
    if (!dispatch(request))
        reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
