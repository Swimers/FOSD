#include "ModelBackend.h"
namespace fosd {
namespace ui {
namespace classic {
ModelBackend::ModelBackend(QObject *parent)
    : FeatureBackend({PanelId::ModelPanel, PanelId::KeyPointEdit, PanelId::KeyPointSelect,
                      PanelId::ColorPicker},
                     parent) {
}

void ModelBackend::handleRequest(const UiRequest &request) {
    if (!dispatch(request))
        reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
