#include "FOSD-Classic-QWidgets/features/mainwindow/backends/MainWindowBackend.h"
namespace fosd {
namespace ui {
namespace classic {
MainWindowBackend::MainWindowBackend(QObject *parent) : FeatureBackend({PanelId::Main}, parent) {
}

void MainWindowBackend::handleRequest(const UiRequest &request) {
    if (!dispatch(request))
        reportUnavailable(request);
}
} // namespace classic
} // namespace ui
} // namespace fosd
