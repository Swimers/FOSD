#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd { namespace ui { namespace classic {
// Backend extension point for HardwareData, TrackingManager, TrackingData only.
class TrackingBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit TrackingBackend(QObject *parent = nullptr);
protected:
    void handleRequest(const UiRequest &request) override;
};
} } }
