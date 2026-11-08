#include "FOSD-Classic-QWidgets/features/connection/views/connectwindow.h"
#include "FOSD-Classic-QWidgets/common/FormBinding.h"
#include "FOSD-Classic-QWidgets/common/UiResources.h"
#include "ui_connectwindow.h"
#include "FOSD-Classic-QWidgets/features/connection/views/ConnectionRenderers.h"
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMouseEvent>
#include <QTextBlock>
#include <QTextCursor>
#include "FOSD-Classic-QWidgets/third_party/qcustomplot/qcustomplot.h"
namespace fosd {
namespace ui {
namespace classic {
ConnectionView::ConnectionView(QWidget *parent) : QWidget(parent), m_ui(new Ui::connectwindow) {
    initializeFosdUiResources();
    m_ui->setupUi(this);
    m_ui->ptedit_UdpRec->setMaximumBlockCount(200);
    m_ui->ptedit_UdpRec->setUndoRedoEnabled(false);
    m_ui->ptedit_UdpRec->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_ui->cbox_UdpDisplayMode->setToolTip(tr("有效包显示接收时间、协议、序号和记录数；此选项仅切换未识别包的预览格式"));
    initializeSpectrum();
    m_binding = new FormBinding(PanelId::Connection, this, this);
    for (auto *edit : findChildren<QLineEdit *>()) {
        if (edit->objectName().startsWith("edit_SerialCommand"))
            connect(edit, &QLineEdit::editingFinished, this, [this, edit]() {
                emit m_binding->actionTriggered(PanelId::Connection, edit->objectName());
            });
    }
    connect(m_binding, &ViewEvents::actionTriggered, this, &ConnectionView::relayControl);
}

ConnectionView::~ConnectionView() = default;
ViewEvents *ConnectionView::events() const {
    return m_binding;
}

QVariantMap ConnectionView::values() const {
    return m_binding->values();
}

void ConnectionView::applyFields(const QVariantMap &fields) {
    auto ordinary=fields;
    if(fields.contains("cplot_Spectrum")){renderSpectrum(m_ui->cplot_Spectrum,fields.value("cplot_Spectrum").toMap(),this);ordinary.remove("cplot_Spectrum");}
    for(auto *log:{m_ui->ptedit_SerialRec,m_ui->ptedit_UdpRec,m_ui->ptedit_TcpRec}){
        const auto value=fields.value(log->objectName());
        if(value.type()==QVariant::Map&&value.toMap().contains("entries")){appendTrafficEntries(log,value.toMap());ordinary.remove(log->objectName());}
    }
    m_binding->applyFields(ordinary);
    if (fields.contains("_spectrumReset"))
        resetSpectrumView();
    if (fields.contains("cplot_Spectrum"))
        refreshSpectrumSelection();
    m_ui->pbtn_SaveDynamic->setText(m_ui->pbtn_SaveDynamic->isChecked() ? tr("停止保存") : tr("开始保存"));
}

void ConnectionView::initializeSpectrum() {
    auto *plot = m_ui->cplot_Spectrum;
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    plot->setProperty("spectrumAutoRange", true);
    plot->xAxis->setLabel(tr("时间 (s)"));
    plot->yAxis->setLabel(tr("波长 (nm)"));
    plot->setSelectionTolerance(10);
    connect(m_ui->ckbox_SpectrumAutoRange, &QCheckBox::toggled, this, [this, plot](bool enabled) {
        plot->setProperty("spectrumAutoRange", enabled);
        if (enabled) {
            plot->rescaleAxes();
            plot->replot(QCustomPlot::rpQueuedReplot);
        }
    });
    connect(plot, &QCustomPlot::mousePress, this, [this](QMouseEvent *event) {
        if (event->button() == Qt::LeftButton)
            m_ui->ckbox_SpectrumAutoRange->setChecked(false);
    });
    connect(plot, &QCustomPlot::mouseWheel, this, [this](QWheelEvent *) {
        m_ui->ckbox_SpectrumAutoRange->setChecked(false);
    });
    connect(plot, &QCustomPlot::mouseDoubleClick, this, [this](QMouseEvent *) { resetSpectrumView(); });
    connect(plot, &QCustomPlot::plottableClick, this,
        [this](QCPAbstractPlottable *plottable, int index, QMouseEvent *) {
            auto *graph = qobject_cast<QCPGraph *>(plottable);
            if (!graph || index < 0 || index >= graph->dataCount())
                return;
            m_selectedCurve = graph->name();
            m_selectedTime = graph->data()->at(index)->key;
            m_hasSpectrumSelection = true;
            refreshSpectrumSelection();
        });
    setSpectrumTheme(false);
}

void ConnectionView::resetSpectrumView() {
    m_ui->ckbox_SpectrumAutoRange->setChecked(true);
    m_ui->cplot_Spectrum->rescaleAxes();
    m_ui->cplot_Spectrum->replot(QCustomPlot::rpQueuedReplot);
}

void ConnectionView::refreshSpectrumSelection() {
    if (!m_hasSpectrumSelection)
        return;
    auto *plot = m_ui->cplot_Spectrum;
    bool found = false;
    for (int i = 0; i < plot->graphCount(); ++i) {
        auto *graph = plot->graph(i);
        graph->setSelection(QCPDataSelection());
        if (graph->name() != m_selectedCurve)
            continue;
        const auto point = graph->data()->findBegin(m_selectedTime, false);
        if (point == graph->data()->constEnd() || !qFuzzyCompare(point->key + 1.0, m_selectedTime + 1.0))
            continue;
        const int index = int(point - graph->data()->constBegin());
        graph->setSelection(QCPDataSelection(QCPDataRange(index, index + 1)));
        m_ui->lab_SpectrumPointValue->setText(tr("%1  |  时间 %2 s  |  波长 %3 nm")
            .arg(m_selectedCurve).arg(point->key, 0, 'f', 3).arg(point->value, 0, 'f', 6));
        found = true;
    }
    if (!found) {
        m_hasSpectrumSelection = false;
        m_ui->lab_SpectrumPointValue->setText(tr("所选数据点已移出显示窗口"));
    }
    plot->replot(QCustomPlot::rpQueuedReplot);
}

void ConnectionView::setSpectrumTheme(bool dark) {
    for (auto *log : {m_ui->ptedit_SerialRec, m_ui->ptedit_UdpRec, m_ui->ptedit_TcpRec}) {
        log->setProperty("trafficDarkTheme", dark);
        for (auto block = log->document()->begin(); block.isValid(); block = block.next()) {
            if (block.userState() != 1 && block.userState() != 2)
                continue;
            QTextCursor cursor(block);
            cursor.select(QTextCursor::BlockUnderCursor);
            QTextCharFormat format;
            format.setForeground(QColor(block.userState() == 2 ? (dark ? "#73b7ff" : "#1d4ed8")
                                                               : (dark ? "#63df91" : "#15803d")));
            cursor.mergeCharFormat(format);
        }
    }
    auto *plot = m_ui->cplot_Spectrum;
    plot->setProperty("spectrumDarkTheme", dark);
    const QColor background = dark ? QColor("#20252b") : QColor("#ffffff");
    const QColor foreground = dark ? QColor("#e4e8ed") : QColor("#28323c");
    const QColor grid = dark ? QColor("#454d58") : QColor("#dbe0e5");
    plot->setBackground(QBrush(background));
    plot->axisRect()->setBackground(QBrush(background));
    for (auto *axis : {plot->xAxis, plot->yAxis, plot->xAxis2, plot->yAxis2}) {
        axis->setBasePen(QPen(foreground));
        axis->setTickPen(QPen(foreground));
        axis->setSubTickPen(QPen(foreground));
        axis->setTickLabelColor(foreground);
        axis->setLabelColor(foreground);
        axis->grid()->setPen(QPen(grid, 1, Qt::DotLine));
        axis->grid()->setSubGridPen(QPen(grid.darker(110), 1, Qt::DotLine));
        axis->grid()->setZeroLinePen(QPen(grid));
    }
    plot->legend->setBrush(QBrush(background));
    plot->legend->setBorderPen(QPen(grid));
    plot->legend->setTextColor(foreground);
    for (int i = 0; i < plot->graphCount(); ++i)
        plot->graph(i)->setPen(QPen(QColor::fromHsv((i * 47) % 360, dark ? 160 : 210, dark ? 245 : 175), 1.6));
    plot->replot(QCustomPlot::rpQueuedReplot);
}

void ConnectionView::clearSensitiveFields() {
    m_binding->clearSensitiveFields();
}

void ConnectionView::clearInputs() {
    m_binding->clearInputs();
}

void ConnectionView::showStatus(const QString &message) {
    m_binding->showStatus(message);
}

void ConnectionView::relayControl(PanelId panel, const QString &action) {
    if (panel != PanelId::Connection)
        return;
    if (action == "cbox_CommuteChoose") {
        emit cbox_CommuteChooseRequested(values());
        return;
    }
    if (action == "cbox_LocakIP") {
        emit cbox_LocakIPRequested(values());
        return;
    }
    if (action == "cbox_SerialBaudChoose") {
        emit cbox_SerialBaudChooseRequested(values());
        return;
    }
    if (action == "cbox_SerialNameChoose") {
        emit cbox_SerialNameChooseRequested(values());
        return;
    }
    if (action == "ckbox_CH1") {
        emit ckbox_CH1Requested(values());
        return;
    }
    if (action == "ckbox_CH10") {
        emit ckbox_CH10Requested(values());
        return;
    }
    if (action == "ckbox_CH11") {
        emit ckbox_CH11Requested(values());
        return;
    }
    if (action == "ckbox_CH12") {
        emit ckbox_CH12Requested(values());
        return;
    }
    if (action == "ckbox_CH13") {
        emit ckbox_CH13Requested(values());
        return;
    }
    if (action == "ckbox_CH14") {
        emit ckbox_CH14Requested(values());
        return;
    }
    if (action == "ckbox_CH15") {
        emit ckbox_CH15Requested(values());
        return;
    }
    if (action == "ckbox_CH16") {
        emit ckbox_CH16Requested(values());
        return;
    }
    if (action == "ckbox_CH2") {
        emit ckbox_CH2Requested(values());
        return;
    }
    if (action == "ckbox_CH3") {
        emit ckbox_CH3Requested(values());
        return;
    }
    if (action == "ckbox_CH4") {
        emit ckbox_CH4Requested(values());
        return;
    }
    if (action == "ckbox_CH5") {
        emit ckbox_CH5Requested(values());
        return;
    }
    if (action == "ckbox_CH6") {
        emit ckbox_CH6Requested(values());
        return;
    }
    if (action == "ckbox_CH7") {
        emit ckbox_CH7Requested(values());
        return;
    }
    if (action == "ckbox_CH8") {
        emit ckbox_CH8Requested(values());
        return;
    }
    if (action == "ckbox_CH9") {
        emit ckbox_CH9Requested(values());
        return;
    }
    if (action == "pbtn_OpenNet") {
        emit pbtn_OpenNetRequested(values());
        return;
    }
    if (action == "pbtn_OpenSerial") {
        emit pbtn_OpenSerialRequested(values());
        return;
    }
    if (action == "pbtn_ParaRest") {
        emit pbtn_ParaRestRequested(values());
        return;
    }
    if (action == "pbtn_ParaSet") {
        emit pbtn_ParaSetRequested(values());
        return;
    }
    if (action == "pbtn_RefreshSerial") {
        emit pbtn_RefreshSerialRequested(values());
        return;
    }
    if (action == "pbtn_SaveDynamic") {
        emit pbtn_SaveDynamicRequested(values());
        return;
    }
    if (action == "pbtn_SaveSpectrum") {
        emit pbtn_SaveSpectrumRequested(values());
        return;
    }
    if (action == "pbtn_SelectAllCH") {
        emit pbtn_SelectAllCHRequested(values());
        return;
    }
    if (action == "pbtn_SelectCH") {
        emit pbtn_SelectCHRequested(values());
        return;
    }
    if (action == "pbtn_SerialClear") {
        emit pbtn_SerialClearRequested(values());
        return;
    }
    if (action == "pbtn_SerialSend") {
        emit pbtn_SerialSendRequested(values());
        return;
    }
    if (action == "pbtn_TcpClear") {
        emit pbtn_TcpClearRequested(values());
        return;
    }
    if (action == "pbtn_TcpSend") {
        emit pbtn_TcpSendRequested(values());
        return;
    }
    if (action == "pbtn_UdpClear") {
        emit pbtn_UdpClearRequested(values());
        return;
    }
    if (action == "pbtn_UdpSend") {
        emit pbtn_UdpSendRequested(values());
        return;
    }
    if (action == "pbtn_WaveLengthReset") {
        emit pbtn_WaveLengthResetRequested(values());
        return;
    }
}
} // namespace classic
} // namespace ui
} // namespace fosd
