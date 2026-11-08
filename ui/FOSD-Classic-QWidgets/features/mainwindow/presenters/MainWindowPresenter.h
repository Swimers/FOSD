#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class MainWindowPresenter final : public PanelPresenter {
    Q_OBJECT
  public:
    MainWindowPresenter(PanelId, IView *, IWorkbenchView *host = nullptr,
                        QObject *parent = nullptr);

  protected:
    bool localAction(const QString &, const QVariantMap &) override;
};
} // namespace classic
} // namespace ui
} // namespace fosd
