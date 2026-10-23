#pragma once
#include "UiTypes.h"
#include <QObject>

namespace fosd { namespace widgets {
// Implement an application-layer adapter later. It owns any Control references;
// Views/Presenters never include communication, calculation or VTK headers.
// Connect requests to execute using a queued connection on the adapter's thread.
class IWorkbenchBackend : public QObject {
    Q_OBJECT
public:
    explicit IWorkbenchBackend(QObject *parent = nullptr) : QObject(parent) {}
    ~IWorkbenchBackend() override = default;
public slots:
    virtual void execute(const fosd::widgets::UiRequest &request) = 0;
signals:
    void completed(const fosd::widgets::UiResponse &response);
    void presentationChanged(fosd::widgets::PanelId panel, const QVariantMap &fields);
};
} }
