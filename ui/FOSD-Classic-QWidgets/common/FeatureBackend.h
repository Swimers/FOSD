#pragma once
#include "IBackend.h"
#include <QVector>
namespace fosd { namespace ui { namespace classic {
class FeatureBackend : public IBackend {
    Q_OBJECT
public:
    FeatureBackend(const QVector<PanelId> &panels, QObject *parent = nullptr);
    bool supports(PanelId panel) const override;
public slots:
    void execute(const fosd::ui::classic::UiRequest &request) override;
protected:
    virtual void handleRequest(const UiRequest &request) = 0;
    void reportUnavailable(const UiRequest &request);
private:
    QVector<PanelId> m_panels;
};
} } }
