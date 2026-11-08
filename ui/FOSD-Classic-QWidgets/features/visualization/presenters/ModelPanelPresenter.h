#pragma once
#include "FOSD-Classic-QWidgets/common/PanelPresenter.h"
namespace fosd {
namespace ui {
namespace classic {
class ModelPanelPresenter final : public PanelPresenter {
    Q_OBJECT
  public:
    ModelPanelPresenter(PanelId, IView *, IWorkbenchView *host = nullptr,
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
    bool markerAction(const QString &, const QVariantMap &, QString &, QVariantMap &);
    void publishMarkers();
    void publishDisplayedPoints();
    void publishScalar();
    void presentScalar(const QVariantMap &fields);
    bool m_originalKeyCoordinates = true;
    QVariantMap m_lastScalar;
};
} // namespace classic
} // namespace ui
} // namespace fosd
