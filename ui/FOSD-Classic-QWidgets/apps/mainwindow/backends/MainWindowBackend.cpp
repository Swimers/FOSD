#include "MainWindowBackend.h"
namespace fosd {
namespace ui {
namespace classic {
MainWindowBackend::MainWindowBackend(QObject *parent) : FeatureBackend({PanelId::Main}, parent) {
}

void MainWindowBackend::handleRequest(const UiRequest &request) {
    reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
