#pragma once
#include "IBackend.h"
#include <QVector>
#include <QPointer>
#include "WorkbenchServices.h"
namespace fosd {
namespace ui {
namespace classic {
class FeatureBackend : public IBackend {
    Q_OBJECT
public:
    FeatureBackend(const QVector<PanelId> &panels, QObject *parent = nullptr);
    bool supports(PanelId panel) const override;
    void setServices(WorkbenchServices *services);
public slots:
    void execute(const fosd::ui::classic::UiRequest &request) override;

protected:
    virtual void handleRequest(const UiRequest &request) = 0;
    void reportUnavailable(const UiRequest &request);
    bool dispatch(const UiRequest &request);

private slots:
    void publish(fosd::ui::classic::PanelId panel, const QVariantMap &fields);

private:
    QVector<PanelId> m_panels;
    QPointer<WorkbenchServices> m_services;
};
} // namespace classic
} // namespace ui
} // namespace fosd
