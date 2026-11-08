#pragma once
#include "FOSD-Classic-QWidgets/common/IView.h"
#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include <QPointer>
namespace fosd {
namespace ui {
namespace classic {
class IWorkbenchView;
class PanelPresenter : public QObject {
    Q_OBJECT
  public:
    PanelPresenter(PanelId, IView *, IWorkbenchView *host = nullptr, QObject *parent = nullptr);
    void setModel(fosd::application::WorkbenchModel *model);
    virtual void initialize();
    virtual void refresh() {
    }
    virtual void poll() {
    }
  public slots:
    void execute(const QString &action);
  signals:
    void statusChanged(const QString &message);
    void actionFinished(const QString &action, bool success, const QString &message);

  protected:
    virtual bool localAction(const QString &, const QVariantMap &);
    virtual bool prepareAction(const QString &, QVariantMap &) {
        return true;
    }
    virtual bool handleAction(const QString &, const QVariantMap &, QString &, QVariantMap &) {
        return false;
    }
    virtual void bindModel() {
    }
    void present(PanelId, const QVariantMap &);
    void presentOwn(PanelId p, const QVariantMap &f) {
        if (p == m_panel)
            m_view->applyFields(f);
    }
    bool showPanel(PanelId);
    static QString text(const QVariantMap &fields, const QString &name);
    static int index(const QVariantMap &fields, const QString &name);
    PanelId m_panel;
    IView *m_view;
    IWorkbenchView *m_host;
    QPointer<fosd::application::WorkbenchModel> m_model;
};
} // namespace classic
} // namespace ui
} // namespace fosd
