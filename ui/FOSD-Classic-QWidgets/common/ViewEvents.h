#pragma once
#include "UiTypes.h"
#include <QObject>
namespace fosd { namespace ui { namespace classic {
// Typed QObject event source exposed by each IView. GUI thread only.
class ViewEvents : public QObject {
    Q_OBJECT
public:
    explicit ViewEvents(QObject *parent = nullptr) : QObject(parent) {}
signals:
    void actionTriggered(fosd::ui::classic::PanelId panel, const QString &action);
};
} } }
