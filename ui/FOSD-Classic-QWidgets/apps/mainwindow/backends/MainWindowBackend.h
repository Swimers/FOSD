#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for Main only.
class MainWindowBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit MainWindowBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
