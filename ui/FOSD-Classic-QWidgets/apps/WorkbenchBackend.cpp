#include "WorkbenchBackend.h"
#include "../common/FeatureBackend.h"
#include "account/backends/AccountBackend.h"
#include "connection/backends/ConnectionBackend.h"
#include "mainwindow/backends/MainWindowBackend.h"
#include "model/backends/ModelBackend.h"
#include "plot/backends/PlotBackend.h"
#include "tracking/backends/TrackingBackend.h"
#include "visualization/backends/VisualizationBackend.h"
namespace fosd {
namespace ui {
namespace classic {
WorkbenchBackend::WorkbenchBackend(QObject *parent) : QObject(parent) {
    setBackend(FeatureId::MainWindow, new MainWindowBackend(this));
    setBackend(FeatureId::Connection, new ConnectionBackend(this));
    setBackend(FeatureId::Plot, new PlotBackend(this));
    setBackend(FeatureId::Visualization, new VisualizationBackend(this));
    setBackend(FeatureId::Model, new ModelBackend(this));
    setBackend(FeatureId::Tracking, new TrackingBackend(this));
    setBackend(FeatureId::Account, new AccountBackend(this));
}

IBackend *WorkbenchBackend::backend(FeatureId feature) const {
    return m_backends.value(int(feature)).data();
}

void WorkbenchBackend::setBackend(FeatureId feature, IBackend *backend) {
    auto *previous = this->backend(feature);
    if (previous == backend)
        return;
    m_backends[int(feature)] = backend;
    if (auto *featureBackend = qobject_cast<FeatureBackend *>(backend))
        featureBackend->setServices(m_services);
    if (previous && !m_backends.values().contains(QPointer<IBackend>(previous)))
        disconnect(previous, &QObject::destroyed, this, &WorkbenchBackend::adapterDestroyed);
    if (backend)
        connect(backend, &QObject::destroyed, this, &WorkbenchBackend::adapterDestroyed,
                Qt::UniqueConnection);
    emit backendChanged(feature);
}

void WorkbenchBackend::setServices(WorkbenchServices *services) {
    m_services = services;
    for (auto backend : m_backends)
        if (auto *feature = qobject_cast<FeatureBackend *>(backend.data()))
            feature->setServices(services);
}

void WorkbenchBackend::adapterDestroyed() {
    for (auto it = m_backends.constBegin(); it != m_backends.constEnd(); ++it)
        if (!it.value())
            emit backendChanged(FeatureId(it.key()));
}
} // namespace classic
} // namespace ui
} // namespace fosd
