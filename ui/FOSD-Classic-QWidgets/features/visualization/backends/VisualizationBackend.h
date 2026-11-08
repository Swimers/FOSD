#pragma once
#include "FOSD-Classic-QWidgets/common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for Visualization, ScenePanel, InfoPanel only.
class VisualizationBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit VisualizationBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
