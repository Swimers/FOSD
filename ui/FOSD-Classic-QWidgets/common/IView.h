#pragma once
#include <QVariantMap>
#include "FOSD-Classic-QWidgets/common/ViewEvents.h"

namespace fosd {
namespace ui {
namespace classic {

class IView {
public:
    virtual ~IView() = default;

    virtual ViewEvents *events() const = 0;
    virtual QVariantMap values() const = 0;
    virtual void applyFields(const QVariantMap &fields) = 0;
    virtual void clearSensitiveFields() = 0;
    virtual void showStatus(const QString &message) = 0;
};

} // namespace classic
} // namespace ui
} // namespace fosd
