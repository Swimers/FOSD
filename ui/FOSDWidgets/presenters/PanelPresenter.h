#pragma once
#include "../contracts/UiTypes.h"
#include "../contracts/IPanelView.h"
#include <QObject>
#include <QHash>
namespace fosd { namespace widgets {
// One presenter instance per form. The view contract also permits fake views
// in tests; no generated UI class or backend implementation is referenced.
class PanelPresenter final : public QObject {
    Q_OBJECT
public:
    PanelPresenter(PanelId panel, IPanelView *view, QObject *parent = nullptr);
    void finishLocal(quint64 id);
    void cancelPending();
public slots:
    void execute(const QString &action);
    void complete(const fosd::widgets::UiResponse &response);
signals:
    void requestReady(const fosd::widgets::UiRequest &request);
    void statusChanged(const QString &message);
private:
    PanelId m_panel;
    IPanelView *m_view; // Owned by the composition root; outlives this presenter.
    QHash<quint64, QString> m_pending;
};
} }
