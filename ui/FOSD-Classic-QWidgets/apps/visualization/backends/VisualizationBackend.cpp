#include "VisualizationBackend.h"
namespace fosd {
namespace ui {
namespace classic {
VisualizationBackend::VisualizationBackend(QObject *parent)
    : FeatureBackend({PanelId::Visualization, PanelId::ScenePanel, PanelId::InfoPanel}, parent) {
}

void VisualizationBackend::handleRequest(const UiRequest &request) {
    reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
