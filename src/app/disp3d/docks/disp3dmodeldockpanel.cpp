#include "disp3dmodeldockpanel.h"
#include "ui_disp3dmodeldockpanel.h"

#include <QDockWidget>

Disp3DModelDockPanel::Disp3DModelDockPanel(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Disp3DModelDockPanel),
      m_dock(new QDockWidget(tr("模型配置"), parent))
{
    ui->setupUi(this);

    m_dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dock->setWidget(this);

    connect(ui->pbtn_ModelLoad, &QPushButton::clicked, this, &Disp3DModelDockPanel::modelLoadRequested);
    connect(ui->pbtn_ModelClear, &QPushButton::clicked, this, &Disp3DModelDockPanel::modelClearRequested);
    connect(ui->pbtn_ModelMerge, &QPushButton::clicked, this, &Disp3DModelDockPanel::modelMergeRequested);
    connect(ui->pbtn_ModelExport, &QPushButton::clicked, this, &Disp3DModelDockPanel::modelExportRequested);
    connect(ui->pbtn_KeyPointEdit, &QPushButton::clicked, this, &Disp3DModelDockPanel::keyPointEditRequested);
    connect(ui->pbtn_KeyPointSelect, &QPushButton::clicked, this, &Disp3DModelDockPanel::keyPointSelectRequested);
    connect(ui->pbtn_ScalarClear, &QPushButton::clicked, this, &Disp3DModelDockPanel::scalarClearRequested);
    connect(ui->pbtn_SelectColor, &QPushButton::clicked, this, &Disp3DModelDockPanel::colorSelected);
    connect(ui->pbtn_ScalarSet, &QPushButton::clicked, this, [this]() {
        emit scalarSetRequested(ui->cbox_ScalarSelect->currentIndex());
    });
    connect(ui->cbox_FPS, &QComboBox::currentTextChanged, this, [this](const QString &) {
        emit fpsChanged(fps());
    });

    connect(ui->pbtn_Normalize, &QPushButton::toggled, this, [this](bool checked) {
        ui->pbtn_Normalize->setText(checked ? tr("归一化:开") : tr("归一化:关"));
        ui->le_MapRangeMin->setEnabled(!checked);
        ui->le_MapRangeMax->setEnabled(!checked);
        emit normalizeToggled(checked);
    });

    auto emitRangeChanged = [this]() {
        double lo = ui->le_MapRangeMin->text().toDouble();
        double hi = ui->le_MapRangeMax->text().toDouble();
        if (lo < hi) {
            emit mapRangeChanged(lo, hi);
        }
    };
    connect(ui->le_MapRangeMin, &QLineEdit::editingFinished, this, emitRangeChanged);
    connect(ui->le_MapRangeMax, &QLineEdit::editingFinished, this, emitRangeChanged);
}

Disp3DModelDockPanel::~Disp3DModelDockPanel()
{
    delete ui;
}

QString Disp3DModelDockPanel::dockId() const
{
    return Disp3DDockIds::Model;
}

QDockWidget *Disp3DModelDockPanel::dockWidget() const
{
    return m_dock;
}

int Disp3DModelDockPanel::fps() const
{
    const QString text = ui->cbox_FPS->currentText();
    return text.left(text.indexOf('H')).toInt();
}

int Disp3DModelDockPanel::scalarIndex() const
{
    return ui->cbox_ScalarSelect->currentIndex();
}

bool Disp3DModelDockPanel::isNormalized() const
{
    return ui->pbtn_Normalize->isChecked();
}

double Disp3DModelDockPanel::mapRangeMin() const
{
    return ui->le_MapRangeMin->text().toDouble();
}

double Disp3DModelDockPanel::mapRangeMax() const
{
    return ui->le_MapRangeMax->text().toDouble();
}
