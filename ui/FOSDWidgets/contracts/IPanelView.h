#pragma once
#include <QVariantMap>

namespace fosd { namespace widgets {
class IPanelView {
public:
    virtual ~IPanelView() = default;
    virtual QVariantMap values() const = 0;
    virtual void applyFields(const QVariantMap &fields) = 0;
    virtual void clearSensitiveFields() = 0;
    virtual void clearInputs() = 0;
    virtual void showStatus(const QString &message) = 0;
};
} }
