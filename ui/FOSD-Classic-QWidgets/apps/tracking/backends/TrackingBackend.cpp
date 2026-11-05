#include "TrackingBackend.h"
namespace fosd {
namespace ui {
namespace classic {
TrackingBackend::TrackingBackend(QObject *parent)
    : FeatureBackend({PanelId::HardwareData},
                     parent) {
}

void TrackingBackend::handleRequest(const UiRequest &request) {
    if (!dispatch(request))
        reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
