#include "FOSD-Classic-QWidgets/application/WorkbenchPresenter.h"
#include "FOSD-Classic-QWidgets/features/mainwindow/presenters/MainWindowPresenter.h"
#include "FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.h"
#include "FOSD-Classic-QWidgets/features/visualization/presenters/VisualizationPresenter.h"
#include "FOSD-Classic-QWidgets/features/visualization/presenters/ModelPanelPresenter.h"
#include "FOSD-Classic-QWidgets/features/data/presenters/HardwareDataPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
WorkbenchPresenter::WorkbenchPresenter(IWorkbenchView *view, application::WorkbenchModel *model,
                                       QObject *parent)
    : QObject(parent), m_view(view), m_model(model) {
    const PanelId panels[] = {
        PanelId::Main,          PanelId::Connection,   PanelId::ChannelCalibration,
        PanelId::Visualization, PanelId::ModelPanel,   PanelId::ScenePanel,
        PanelId::InfoPanel,     PanelId::KeyPointEdit, PanelId::KeyPointSelect,
        PanelId::HardwareData};
    for (auto panel : panels) {
        auto *v = view->panelView(panel);
        if (!v)
            continue;
        PanelPresenter *p = nullptr;
        switch (panel) {
        case PanelId::Main:
            p = new MainWindowPresenter(panel, v, view, this);
            break;
        case PanelId::Connection:
        case PanelId::ChannelCalibration:
            p = new ConnectionPresenter(panel, v, view, this);
            break;
        case PanelId::Visualization:
        case PanelId::ScenePanel:
        case PanelId::InfoPanel:
            p = new VisualizationPresenter(panel, v, view, this);
            break;
        case PanelId::ModelPanel:
        case PanelId::KeyPointEdit:
        case PanelId::KeyPointSelect:
            p = new ModelPanelPresenter(panel, v, view, this);
            break;
        case PanelId::HardwareData:
            p = new HardwareDataPresenter(panel, v, view, this);
            break;
        }
        m_presenters.insert(int(panel), p);
        p->setModel(model);
        connect(p, &PanelPresenter::statusChanged, this, &WorkbenchPresenter::showStatus);
    }
    if (model) {
        connect(model, &application::WorkbenchModel::messageChanged, this,
                &WorkbenchPresenter::showStatus);
        connect(model, &application::WorkbenchModel::stateChanged, this,
                [this] { m_view->setModelAvailable(m_model && m_model->state().hasModel); });
    }
    m_view->setModelAvailable(model && model->state().hasModel);
    m_refreshTimer.setInterval(100);
    connect(&m_refreshTimer, &QTimer::timeout, this, &WorkbenchPresenter::refreshDisplays);
    m_refreshTimer.start();
}
void WorkbenchPresenter::initialize() {
    for (auto *p : m_presenters)
        p->initialize();
    if (m_model)
        m_model->initialize();
}
void WorkbenchPresenter::execute(PanelId p, const QString &action) {
    if (auto *presenter = m_presenters.value(int(p)))
        presenter->execute(action);
}
void WorkbenchPresenter::refreshDisplays() {
    for (auto *p : m_presenters)
        p->poll();
}
void WorkbenchPresenter::showStatus(const QString &s) {
    if (auto *view = m_view->panelView(PanelId::Main))
        view->showStatus(s);
}
} // namespace classic
} // namespace ui
} // namespace fosd
