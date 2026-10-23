#pragma once
#include "../contracts/IWorkbenchBackend.h"
#include "../contracts/IWorkbenchView.h"
#include <QMap>
#include <QPointer>
namespace fosd { namespace widgets {
class PanelPresenter;
class WorkbenchPresenter final : public QObject {
    Q_OBJECT
public:
    explicit WorkbenchPresenter(IWorkbenchView *view, QObject *parent = nullptr);
    ~WorkbenchPresenter() override;
    void setBackend(IWorkbenchBackend *backend);
public slots:
    void execute(fosd::widgets::PanelId panel, const QString &action);
signals:
    void backendRequest(const fosd::widgets::UiRequest &request);
private slots:
    void route(const fosd::widgets::UiRequest &request);
    void complete(const fosd::widgets::UiResponse &response);
    void present(fosd::widgets::PanelId panel, const QVariantMap &fields);
    void showStatus(const QString &message);
    void backendDestroyed();
private:
    bool localAction(const UiRequest &request);
    IWorkbenchView *m_view;
    QMap<int, PanelPresenter *> m_presenters;
    QPointer<IWorkbenchBackend> m_backend;
};
} }
