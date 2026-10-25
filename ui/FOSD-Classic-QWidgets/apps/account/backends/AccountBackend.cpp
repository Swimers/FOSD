#include "AccountBackend.h"
namespace fosd {
namespace ui {
namespace classic {
AccountBackend::AccountBackend(QObject *parent)
    : FeatureBackend({PanelId::Login, PanelId::Registration, PanelId::Recovery, PanelId::Account},
                     parent) {
}

void AccountBackend::handleRequest(const UiRequest &request) {
    reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
