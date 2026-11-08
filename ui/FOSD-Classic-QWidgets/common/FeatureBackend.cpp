#include "FOSD-Classic-QWidgets/common/FeatureBackend.h"
namespace fosd {
namespace ui {
namespace classic {
FeatureBackend::FeatureBackend(const QVector<PanelId> &panels, QObject *parent)
    : IBackend(parent), m_panels(panels) {
}

bool FeatureBackend::supports(PanelId panel) const {
    return m_panels.contains(panel);
}

void FeatureBackend::execute(const UiRequest &request) {
    if (!supports(request.panel)) {
        UiResponse response;
        response.id = request.id;
        response.panel = request.panel;
        response.message = tr("该请求不属于此功能 Backend");
        emit completed(response);
        return;
    }
    handleRequest(request);
}

void FeatureBackend::reportUnavailable(const UiRequest &request) {
    UiResponse response;
    response.id = request.id;
    response.panel = request.panel;
    response.message = tr("%1 的业务适配尚未实现").arg(request.action);
    emit completed(response);
}

void FeatureBackend::setServices(WorkbenchServices *services) {
    if (m_services == services)
        return;
    if (m_services)
        disconnect(m_services, nullptr, this, nullptr);
    m_services = services;
    if (services)
        connect(services, &WorkbenchServices::presentationChanged, this, &FeatureBackend::publish);
}

void FeatureBackend::publish(PanelId panel, const QVariantMap &fields) {
    if (supports(panel))
        emit presentationChanged(panel, fields);
}

bool FeatureBackend::dispatch(const UiRequest &request) {
    if (!m_services)
        return false;
    UiResponse response;
    response.id = request.id;
    response.panel = request.panel;
    if (!m_services->handleRequest(request, &response))
        return false;
    response.id = request.id;
    response.panel = request.panel;
    emit completed(response);
    return true;
}
} // namespace classic
} // namespace ui
} // namespace fosd
