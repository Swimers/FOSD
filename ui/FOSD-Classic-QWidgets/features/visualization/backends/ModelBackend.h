#pragma once
#include "FOSD-Classic-QWidgets/common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for the retained model and key-point panels.
class ModelBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit ModelBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
