#include "TrackingBackend.h"
namespace fosd { namespace ui { namespace classic {
TrackingBackend::TrackingBackend(QObject *parent) : FeatureBackend({PanelId::HardwareData, PanelId::TrackingManager, PanelId::TrackingData}, parent) {}
void TrackingBackend::handleRequest(const UiRequest &request) { reportUnavailable(request); }
} } }
