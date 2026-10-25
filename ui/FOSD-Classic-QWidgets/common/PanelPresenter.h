#pragma once
#include "IView.h"
#include "IBackend.h"
#include <QHash>
#include <QPointer>
namespace fosd { namespace ui { namespace classic {
class IWorkbenchView;
// Shared request lifecycle only; feature-specific actions belong in subclasses.
class PanelPresenter : public QObject {
    Q_OBJECT
public:
    PanelPresenter(PanelId panel, IView *view, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
    void setBackend(IBackend *backend);
    void cancelPending();
public slots:
    void execute(const QString &action);
    void complete(const fosd::ui::classic::UiResponse &response);
signals:
    void requestReady(const fosd::ui::classic::UiRequest &request);
    void statusChanged(const QString &message);
protected:
    virtual bool localAction(const UiRequest &request);
    virtual bool prepareRequest(UiRequest &request);
    virtual bool isSensitiveAction(const QString &action) const;
    bool showPanel(PanelId panel);
    PanelId m_panel;
    IView *m_view;
    IWorkbenchView *m_host;
private slots:
    void actionTriggered(fosd::ui::classic::PanelId panel, const QString &action);
    void receiveResponse(const fosd::ui::classic::UiResponse &response);
    void present(fosd::ui::classic::PanelId panel, const QVariantMap &fields);
    void backendDestroyed();
private:
    QPointer<IBackend> m_backend;
    QHash<quint64, QString> m_pending;
};
} } }
