#pragma once
#include "../../../common/IBackend.h"
#include <QMap>
#include <QPointer>
namespace fosd {
namespace ui {
namespace classic {
// Composition/registry only. Each feature backend owns its own future logic.
// Create/destroy the registry on the GUI thread; injected adapters may be hosted elsewhere.
class WorkbenchBackend final : public QObject {
    Q_OBJECT
public:
    explicit WorkbenchBackend(QObject *parent = nullptr);
    IBackend *backend(FeatureId feature) const;
    void setBackend(FeatureId feature, IBackend *backend);
signals:
    void backendChanged(fosd::ui::classic::FeatureId feature);
private slots:
    void adapterDestroyed();

private:
    QMap<int, QPointer<IBackend>> m_backends;
};
} // namespace classic
} // namespace ui
} // namespace fosd
