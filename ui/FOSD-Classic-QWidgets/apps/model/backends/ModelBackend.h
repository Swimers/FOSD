#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for ModelPanel, KeyPointEdit, KeyPointSelect, ColorPicker only.
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
