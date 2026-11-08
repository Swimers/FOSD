#pragma once
#include <QVariantMap>
class QCustomPlot;
class QPlainTextEdit;
class QWidget;
namespace fosd {
namespace ui {
namespace classic {
void renderSpectrum(QCustomPlot *plot, const QVariantMap &data, QWidget *owner);
void appendTrafficEntries(QPlainTextEdit *edit, const QVariantMap &data);
} // namespace classic
} // namespace ui
} // namespace fosd
