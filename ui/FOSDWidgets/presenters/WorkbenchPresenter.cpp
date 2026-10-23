#include "WorkbenchPresenter.h"
#include "PanelPresenter.h"
namespace fosd { namespace widgets {
WorkbenchPresenter::WorkbenchPresenter(IWorkbenchView *view, QObject *parent) : QObject(parent), m_view(view) {
    qRegisterMetaType<PanelId>("fosd::widgets::PanelId");
    qRegisterMetaType<UiRequest>("fosd::widgets::UiRequest");
    qRegisterMetaType<UiResponse>("fosd::widgets::UiResponse");
    for (int index = int(PanelId::Main); index <= int(PanelId::Account); ++index) {
        auto *panelView = view->panelView(PanelId(index));
        if (!panelView) continue;
        auto *presenter = new PanelPresenter(PanelId(index), panelView, this);
        m_presenters.insert(index, presenter);
        connect(presenter, &PanelPresenter::requestReady, this, &WorkbenchPresenter::route);
        connect(presenter, &PanelPresenter::statusChanged, this, &WorkbenchPresenter::showStatus);
    }
    showStatus(tr("界面已就绪，后端适配器尚未接入"));
}
WorkbenchPresenter::~WorkbenchPresenter() = default;
void WorkbenchPresenter::setBackend(IWorkbenchBackend *backend) {
    if (m_backend == backend) return;
    if (m_backend) { disconnect(this, nullptr, m_backend, nullptr); disconnect(m_backend, nullptr, this, nullptr); }
    for (auto *presenter : m_presenters) presenter->cancelPending();
    m_backend = backend;
    if (backend) {
        connect(this, &WorkbenchPresenter::backendRequest, backend, &IWorkbenchBackend::execute, Qt::QueuedConnection);
        connect(backend, &IWorkbenchBackend::completed, this, &WorkbenchPresenter::complete, Qt::QueuedConnection);
        connect(backend, &IWorkbenchBackend::presentationChanged, this, &WorkbenchPresenter::present, Qt::QueuedConnection);
        connect(backend, &QObject::destroyed, this, &WorkbenchPresenter::backendDestroyed);
    }
    showStatus(backend ? tr("后端适配器已接入") : tr("后端适配器尚未接入"));
}
void WorkbenchPresenter::execute(PanelId panel, const QString &action) {
    if (auto *presenter = m_presenters.value(int(panel), nullptr)) presenter->execute(action);
}
bool WorkbenchPresenter::localAction(const UiRequest &request) {
    const auto panel = request.panel; const auto &action = request.action;
    if (action == "pbtn_Minimize" || action == "btn_Min") {
        m_view->windowAction(panel, WindowAction::Minimize); return true;
    }
    if (action == "pbtn_Full") { m_view->windowAction(panel, WindowAction::ToggleFullScreen); return true; }
    if (action == "pbtn_Maximize" || action == "btn_Close" || action == "pbtn_Exit" ||
        action == "pbtn_ConnectDebugClose" || action == "rejected" || action == "pbtn_SenerCancel") {
        m_view->closePanel(panel); return true;
    }
    if (panel == PanelId::Main) {
        const QStringList navigation{"pbtn_JieTiaoYi", "pbtn_2DPlot", "pbtn_3DPlot"};
        if (navigation.contains(action)) { m_view->selectPage(navigation.indexOf(action)); return true; }
        if (action == "switch_Style") { m_view->applyTheme(request.fields.value("switch_Style").toBool()); return true; }
        if (action == "pbtn_TurnOff") { m_view->showPanel(PanelId::Login); return true; }
    }
    if (panel == PanelId::RainbowMain && action == "pbtn_TurnOff") { m_view->closePanel(panel); return true; }
    if ((panel == PanelId::Login && action == "btn_Clear") || action == "pbtn_RegisterClear" || action == "pbtn_ForgetClear") {
        m_view->panelView(panel)->clearInputs(); return true;
    }
    if (panel == PanelId::Connection) {
        const QMap<QString, QString> clear{{"pbtn_SerialClear", "ptedit_SerialRec"},
            {"pbtn_UdpClear", "ptedit_UdpRec"}, {"pbtn_TcpClear", "ptedit_TcpRec"}};
        if (clear.contains(action)) {
            QVariantMap fields; fields[clear[action]] = QString(); m_view->panelView(panel)->applyFields(fields); return true;
        }
        if (action == "pbtn_SelectAllCH") {
            QVariantMap fields;
            for (int index = 1; index <= 16; ++index) fields[QStringLiteral("ckbox_CH%1").arg(index)] = true;
            m_view->panelView(panel)->applyFields(fields); return true;
        }
    }
    // Navigation is frontend behavior; operational actions continue to backend.
    struct Navigation { PanelId source; const char *action; PanelId target; };
    static const Navigation navigation[] = {
        {PanelId::Connection, "pbtn_Diagnosis", PanelId::ConnectionDebug},
        {PanelId::Plot, "pbtn_ParaRevise", PanelId::PlotSettings},
        {PanelId::ModelPanel, "pbtn_KeyPointEdit", PanelId::KeyPointEdit},
        {PanelId::ModelPanel, "pbtn_KeyPointSelect", PanelId::KeyPointSelect},
        {PanelId::ModelPanel, "pbtn_SelectColor", PanelId::ColorPicker},
        {PanelId::KeyPointSelect, "pbtn_EditPoint", PanelId::KeyPointEdit},
        {PanelId::Visualization, "actionModelDock", PanelId::ModelPanel},
        {PanelId::Visualization, "actionViewDock", PanelId::ScenePanel},
        {PanelId::Visualization, "actionInforDock", PanelId::InfoPanel},
        {PanelId::Visualization, "actionTrackingPoints", PanelId::TrackingManager},
        {PanelId::Visualization, "actionTrackingPointData", PanelId::TrackingData},
        {PanelId::Visualization, "actionBackData", PanelId::HardwareData},
        {PanelId::Login, "btn_Register", PanelId::Registration},
        {PanelId::Login, "btn_Forget", PanelId::Recovery}
    };
    for (const auto &entry : navigation)
        if (entry.source == panel && action == entry.action) { m_view->showPanel(entry.target); return true; }
    return false;
}
void WorkbenchPresenter::route(const UiRequest &request) {
    auto *presenter = m_presenters.value(int(request.panel));
    if (localAction(request)) { presenter->finishLocal(request.id); return; }
    UiRequest forwarded = request;
    const bool modelInput = request.panel == PanelId::ModelPanel &&
        (request.action == "pbtn_ModelLoad" || request.action == "pbtn_ModelMerge");
    const bool modelOutput = request.panel == PanelId::ModelPanel && request.action == "pbtn_ModelExport";
    const bool pointsInput = request.panel == PanelId::TrackingManager && request.action == "pbtn_LoadFile";
    if (m_backend && (modelInput || modelOutput || pointsInput)) {
        const auto path = m_view->chooseFile(modelOutput, pointsInput ? tr("导入追踪点") : tr("模型文件"),
            pointsInput ? tr("数据文件 (*.csv *.txt);;所有文件 (*)") : tr("STL 模型 (*.stl);;所有文件 (*)"));
        if (path.isEmpty()) { presenter->finishLocal(request.id); return; }
        forwarded.fields["filePath"] = path;
    }
    if (!m_backend) {
        UiResponse response; response.id = request.id; response.panel = request.panel;
        response.message = tr("该操作的后端适配器尚未接入"); presenter->complete(response);
        return;
    }
    emit backendRequest(forwarded);
}
void WorkbenchPresenter::complete(const UiResponse &response) {
    if (!m_backend || sender() != m_backend.data()) return;
    if (auto *presenter = m_presenters.value(int(response.panel), nullptr)) presenter->complete(response);
}
void WorkbenchPresenter::present(PanelId panel, const QVariantMap &fields) {
    if (!m_backend || sender() != m_backend.data()) return;
    if (auto *view = m_view->panelView(panel)) view->applyFields(fields);
}
void WorkbenchPresenter::showStatus(const QString &message) {
    if (auto *view = m_view->panelView(PanelId::Main)) view->showStatus(message);
}
void WorkbenchPresenter::backendDestroyed() {
    for (auto *presenter : m_presenters) presenter->cancelPending();
    showStatus(tr("后端适配器已断开"));
}
} }
