#pragma once
#include "../../../common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
// Backend extension point for Connection, ConnectionDebug only.
class ConnectionBackend : public FeatureBackend {
    Q_OBJECT
public:
    explicit ConnectionBackend(QObject *parent = nullptr);

protected:
    void handleRequest(const UiRequest &request) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
