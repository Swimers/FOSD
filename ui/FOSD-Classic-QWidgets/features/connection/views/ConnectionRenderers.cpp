#include "FOSD-Classic-QWidgets/features/connection/views/ConnectionRenderers.h"
#include "FOSD-Classic-QWidgets/controls/qcustomplot/qcustomplot.h"
#include <QPlainTextEdit>
#include <QTextBlock>
#include <QTextCursor>
#include <QScrollBar>
#include <QToolTip>

namespace fosd {
namespace ui {
namespace classic {
void renderSpectrum(QCustomPlot *plot, const QVariantMap &data, QWidget *owner) {
    if (data.contains("savePath")) {
        const auto p = data["savePath"].toString();
        const bool ok =
            p.endsWith(".jpg", Qt::CaseInsensitive)
                ? plot->saveJpg(p)
                : p.endsWith(".bmp", Qt::CaseInsensitive) ? plot->saveBmp(p) : plot->savePng(p);
        QToolTip::showText(owner->mapToGlobal(QPoint(20, 20)),
                           ok ? QObject::tr("图片已保存") : QObject::tr("图片保存失败"), owner);
        return;
    }

    if (data.contains("series")) {
        const auto allSeries = data["series"].toList();
        while (plot->graphCount() > allSeries.size())
            plot->removeGraph(plot->graphCount() - 1);
        int index = 0;
        for (const auto &v : allSeries) {
            const auto series = v.toMap();
            QVector<double> x, y;
            for (const auto &sample : series["samples"].toList()) {
                const auto p = sample.toList();
                if (p.size() == 2) {
                    x.append(p[0].toDouble());
                    y.append(p[1].toDouble());
                }
            }
            auto *g = index < plot->graphCount() ? plot->graph(index) : plot->addGraph();
            const bool dark = plot->property("spectrumDarkTheme").toBool();
            g->setName(series["name"].toString());
            g->setPen(
                QPen(QColor::fromHsv((index * 47) % 360, dark ? 160 : 210, dark ? 245 : 175), 1.6));
            g->setData(x, y);
            g->setSelectable(QCP::stSingleData);
            g->selectionDecorator()->setScatterStyle(
                QCPScatterStyle(QCPScatterStyle::ssCircle, QColor("#f39c12"), QColor("#f39c12"), 8),
                QCPScatterStyle::spAll);
            ++index;
        }
        plot->xAxis->setLabel(QObject::tr("时间 (s)"));
        plot->yAxis->setLabel(QObject::tr("波长 (nm)"));
        plot->legend->setVisible(true);
        if (!plot->property("spectrumAutoRange").isValid() ||
            plot->property("spectrumAutoRange").toBool())
            plot->rescaleAxes();
        plot->replot(QCustomPlot::rpQueuedReplot);
        return;
    }
}
void appendTrafficEntries(QPlainTextEdit *edit, const QVariantMap &data) {
    const bool follow = edit->verticalScrollBar()->value() == edit->verticalScrollBar()->maximum();
    if (data.value("clear").toBool())
        edit->clear();
    QTextCursor cursor(edit->document());
    cursor.movePosition(QTextCursor::End);
    const bool dark = edit->property("trafficDarkTheme").toBool();
    for (const auto &value : data.value("entries").toList()) {
        const auto row = value.toMap();
        const bool outgoing = row.value("outgoing").toBool();
        if (!edit->document()->isEmpty())
            cursor.insertBlock();
        QTextCharFormat format;
        format.setForeground(
            QColor(outgoing ? (dark ? "#73b7ff" : "#1d4ed8") : (dark ? "#63df91" : "#15803d")));
        // A line separator keeps both visual lines in one block, so the
        // document limit and theme recoloring always operate on whole frames.
        cursor.insertText(row.value("text").toString().replace('\n', QChar(0x2028)), format);
        auto block = cursor.block();
        block.setUserState(outgoing ? 2 : 1);
    }
    if (follow)
        edit->verticalScrollBar()->setValue(edit->verticalScrollBar()->maximum());
}
} // namespace classic
} // namespace ui
} // namespace fosd
