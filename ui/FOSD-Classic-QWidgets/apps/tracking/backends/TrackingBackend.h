#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Hardware calibration data adapter.
class TrackingBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit TrackingBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
