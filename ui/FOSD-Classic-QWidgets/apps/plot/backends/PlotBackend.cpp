#include "PlotBackend.h"
namespace fosd {
namespace ui {
namespace classic {
PlotBackend::PlotBackend(QObject *parent)
    : FeatureBackend({PanelId::Plot, PanelId::PlotSettings}, parent) {
}

void PlotBackend::handleRequest(const UiRequest &request) {
    reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
