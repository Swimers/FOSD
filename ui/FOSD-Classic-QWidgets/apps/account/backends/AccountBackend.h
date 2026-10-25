#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for Login, Registration, Recovery, Account only.
class AccountBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit AccountBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
