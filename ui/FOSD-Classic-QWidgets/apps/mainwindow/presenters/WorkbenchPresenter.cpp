#include "WorkbenchPresenter.h"
#include "MainWindowPresenter.h"
#include "../../connection/presenters/ConnectionPresenter.h"
#include "../../plot/presenters/PlotPresenter.h"
#include "../../visualization/presenters/VisualizationPresenter.h"
#include "../../model/presenters/ModelPresenter.h"
#include "../../tracking/presenters/TrackingPresenter.h"
#include "../../account/presenters/AccountPresenter.h"
namespace fosd { namespace ui { namespace classic {
WorkbenchPresenter::WorkbenchPresenter(IWorkbenchView *view, QObject *parent) : QObject(parent), m_view(view) {
    for (int index = int(PanelId::Main); index <= int(PanelId::Account); ++index) {
        const auto panel = PanelId(index); auto *panelView = view->panelView(panel);
        if (!panelView) continue;
        PanelPresenter *presenter = nullptr;
        switch (featureForPanel(panel)) {
        case FeatureId::MainWindow: presenter = new MainWindowPresenter(panel, panelView, view, this); break;
        case FeatureId::Connection: presenter = new ConnectionPresenter(panel, panelView, view, this); break;
        case FeatureId::Plot: presenter = new PlotPresenter(panel, panelView, view, this); break;
        case FeatureId::Visualization: presenter = new VisualizationPresenter(panel, panelView, view, this); break;
        case FeatureId::Model: presenter = new ModelPresenter(panel, panelView, view, this); break;
        case FeatureId::Tracking: presenter = new TrackingPresenter(panel, panelView, view, this); break;
        case FeatureId::Account: presenter = new AccountPresenter(panel, panelView, view, this); break;
        }
        m_presenters.insert(index, presenter);
        connect(presenter, &PanelPresenter::requestReady, this, &WorkbenchPresenter::backendRequest);
        connect(presenter, &PanelPresenter::statusChanged, this, &WorkbenchPresenter::showStatus);
    }
    showStatus(tr("经典界面已就绪，后端适配器尚未接入"));
}
void WorkbenchPresenter::setBackends(WorkbenchBackend *backends) {
    if (m_backends == backends) return;
    if (m_backends) disconnect(m_backends, nullptr, this, nullptr);
    m_backends = backends;
    if (backends) {
        connect(backends, &WorkbenchBackend::backendChanged, this, &WorkbenchPresenter::refreshBackend);
        connect(backends, &QObject::destroyed, this, &WorkbenchPresenter::registryDestroyed);
    }
    for (int feature = int(FeatureId::MainWindow); feature <= int(FeatureId::Account); ++feature)
        refreshBackend(FeatureId(feature));
}
void WorkbenchPresenter::refreshBackend(FeatureId feature) {
    for (auto it = m_presenters.constBegin(); it != m_presenters.constEnd(); ++it)
        if (featureForPanel(PanelId(it.key())) == feature)
            it.value()->setBackend(m_backends ? m_backends->backend(feature) : nullptr);
}
void WorkbenchPresenter::registryDestroyed() {
    if (m_backends) return;
    for (auto *presenter : m_presenters) presenter->setBackend(nullptr);
    showStatus(tr("后端适配器已断开"));
}
void WorkbenchPresenter::execute(PanelId panel, const QString &action) {
    if (auto *presenter = m_presenters.value(int(panel), nullptr)) presenter->execute(action);
}
void WorkbenchPresenter::showStatus(const QString &message) {
    if (auto *view = m_view->panelView(PanelId::Main)) view->showStatus(message);
}
} } }
