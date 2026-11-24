#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class HardwareDataPresenter final : public PanelPresenter {
    Q_OBJECT
  public:
    HardwareDataPresenter(PanelId p, IView *v, IWorkbenchView *host = nullptr,
                          QObject *parent = nullptr)
        : PanelPresenter(p, v, host, parent) {
    }
    void poll() override;
    void initialize() override;

  protected:
    bool handleAction(const QString &, const QVariantMap &, QString &, QVariantMap &) override;

  private:
    bool m_refresh = true;
    core::CalibratedDataPtr m_presented;
};
} // namespace classic
} // namespace ui
} // namespace fosd
