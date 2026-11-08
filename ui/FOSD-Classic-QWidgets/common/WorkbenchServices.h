#pragma once
#include "FOSD-Classic-QWidgets/common/UiTypes.h"
#include <QObject>
namespace fosd {
namespace ui {
namespace classic {
// Application-owned business port. The standalone UI preview may leave it unset.
class WorkbenchServices : public QObject {
    Q_OBJECT
public:
    explicit WorkbenchServices(QObject *parent = nullptr) : QObject(parent) {
    }
    virtual bool handleRequest(const UiRequest &request, UiResponse *response) = 0;
signals:
    void presentationChanged(fosd::ui::classic::PanelId panel, const QVariantMap &fields);
};
} // namespace classic
} // namespace ui
} // namespace fosd
