#pragma once
#include "FOSD-Classic-QWidgets/application/IWorkbenchView.h"
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
#include <QMap>
#include <QTimer>
namespace fosd {
namespace ui {
namespace classic {
class WorkbenchPresenter final : public QObject {
    Q_OBJECT
  public:
    explicit WorkbenchPresenter(IWorkbenchView *view, application::WorkbenchModel *model = nullptr,
                                QObject *parent = nullptr);
    PanelPresenter *panelPresenter(PanelId panel) const {
        return m_presenters.value(int(panel));
    }
  public slots:
    void initialize();
    void refreshDisplays();

  private:
    void showStatus(const QString &);
    IWorkbenchView *m_view;
    QPointer<application::WorkbenchModel> m_model;
    QMap<int, PanelPresenter *> m_presenters;
    QTimer m_refreshTimer;
};
} // namespace classic
} // namespace ui
} // namespace fosd
