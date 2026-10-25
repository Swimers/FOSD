#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for Plot, PlotSettings only.
class PlotBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit PlotBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
