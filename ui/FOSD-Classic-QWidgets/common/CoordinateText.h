#pragma once
#include <QString>
#include <QStringList>
#include <array>
#include <cmath>

namespace fosd { namespace ui { namespace classic {
inline QString selectedPointStatus(const QString &id = QString(),
                                   const std::array<double,3> &original = {},
                                   const std::array<double,3> &display = {}) {
    const bool valid = !id.isEmpty();
    auto coordinates = [valid](const std::array<double,3> &p) {
        QStringList fields;
        for (double value : p) {
            // Suppress rounded negative zero. Large values keep the reserved
            // width using scientific notation with two decimal places.
            const double rounded = std::abs(value) < 0.005 ? 0.0 : value;
            QString text = valid ? QString::number(rounded, 'f', 2) : "--";
            if (text.size() > 12)
                text = QString::number(rounded, 'e', 2);
            fields.append(text.rightJustified(12));
        }
        return "(" + fields.join(", ") + ")";
    };
    return QString::fromUtf8("SelectedPoint ID: %1 | 原始位置: %2 | 变形位置: %3 (mm) | %4")
        .arg((valid ? id : QString("--")).rightJustified(20),
             coordinates(original), coordinates(display),
             valid ? QString::fromUtf8("已选择") : QString::fromUtf8("未选择"));
}
}}}
