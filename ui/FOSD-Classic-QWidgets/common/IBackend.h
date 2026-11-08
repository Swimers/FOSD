#pragma once
#include "FOSD-Classic-QWidgets/common/UiTypes.h"
#include <QObject>
namespace fosd {
namespace ui {
namespace classic {
// Feature adapter contract. Values cross threads; widgets and Controls do not.
class IBackend : public QObject {
    Q_OBJECT
public:
    explicit IBackend(QObject *parent = nullptr) : QObject(parent) {
    }
    virtual bool supports(PanelId panel) const = 0;
public slots:
    virtual void execute(const fosd::ui::classic::UiRequest &request) = 0;
signals:
    void completed(const fosd::ui::classic::UiResponse &response);
    void presentationChanged(fosd::ui::classic::PanelId panel, const QVariantMap &fields);
};
} // namespace classic
} // namespace ui
} // namespace fosd
