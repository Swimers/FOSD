#pragma once
#include "IWorkbenchView.h"
#include "WorkbenchBackend.h"
#include <QMap>
namespace fosd {
namespace ui {
namespace classic {
class PanelPresenter;
// Binds feature presenters to views/adapters; no control-name business switch.
class WorkbenchPresenter final : public QObject {
    Q_OBJECT
public:
    explicit WorkbenchPresenter(IWorkbenchView *view, QObject *parent = nullptr);
    void setBackends(WorkbenchBackend *backends);
public slots:
    void execute(fosd::ui::classic::PanelId panel, const QString &action);
signals:
    void backendRequest(const fosd::ui::classic::UiRequest &request);
private slots:
    void refreshBackend(fosd::ui::classic::FeatureId feature);
    void registryDestroyed();
    void showStatus(const QString &message);

private:
    IWorkbenchView *m_view;
    QMap<int, PanelPresenter *> m_presenters;
    QPointer<WorkbenchBackend> m_backends;
};
} // namespace classic
} // namespace ui
} // namespace fosd
