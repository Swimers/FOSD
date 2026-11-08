#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class VisualizationPresenter final : public PanelPresenter {
    Q_OBJECT
  public:
    VisualizationPresenter(PanelId, IView *, IWorkbenchView *host = nullptr,
                           QObject *parent = nullptr);
    void initialize() override;
    void refresh() override;
    void poll() override;

  protected:
    bool localAction(const QString &, const QVariantMap &) override;
    bool prepareAction(const QString &, QVariantMap &) override;
    bool handleAction(const QString &, const QVariantMap &, QString &, QVariantMap &) override;
    void bindModel() override;

  private:
    void publishAppearance();
    void selectionChanged(const visualization::PickedPointInfo &);
    quint64 m_presentedResult = 0;
};
} // namespace classic
} // namespace ui
} // namespace fosd
