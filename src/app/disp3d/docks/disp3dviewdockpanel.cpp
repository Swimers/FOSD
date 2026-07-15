#include "disp3dviewdockpanel.h"
#include "ui_disp3dviewdockpanel.h"

#include <QDockWidget>

Disp3DViewDockPanel::Disp3DViewDockPanel(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Disp3DViewDockPanel),
      m_dock(new QDockWidget(tr("网格视图"), parent))
{
    ui->setupUi(this);

    m_dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dock->setWidget(this);

    connect(ui->cbox_AxisAble, &QCheckBox::clicked, this, &Disp3DViewDockPanel::axesVisibleChanged);
    connect(ui->hsd_AxisSize, &QSlider::valueChanged, this, &Disp3DViewDockPanel::axesSizeChanged);
    connect(ui->cbox_GridAble, &QCheckBox::clicked, this, &Disp3DViewDockPanel::gridVisibleChanged);
    connect(ui->hsd_GridSize, &QSlider::sliderReleased, this, [this]() {
        emit gridSizeChangeFinished(gridSize());
    });
    connect(ui->cbox_XOYGrid, &QCheckBox::clicked, this, &Disp3DViewDockPanel::xoyGridVisibleChanged);
    connect(ui->cbox_XOZGrid, &QCheckBox::clicked, this, &Disp3DViewDockPanel::xozGridVisibleChanged);
    connect(ui->cbox_ZOYGrid, &QCheckBox::clicked, this, &Disp3DViewDockPanel::zoyGridVisibleChanged);
    connect(ui->cbox_CareamLockAble, &QCheckBox::clicked, this, &Disp3DViewDockPanel::cameraLockChanged);
    connect(ui->cbox_FellowAble, &QCheckBox::clicked, this, &Disp3DViewDockPanel::cameraFollowChanged);
    connect(ui->pbtn_SwitchView, &QPushButton::clicked, this, [this]() {
        emit switchViewRequested(selectedViewIndex());
    });
}

Disp3DViewDockPanel::~Disp3DViewDockPanel()
{
    delete ui;
}

QString Disp3DViewDockPanel::dockId() const
{
    return Disp3DDockIds::View;
}

QDockWidget *Disp3DViewDockPanel::dockWidget() const
{
    return m_dock;
}

int Disp3DViewDockPanel::gridSize() const
{
    return ui->hsd_GridSize->value();
}

int Disp3DViewDockPanel::selectedViewIndex() const
{
    return ui->cbox_SelectView->currentIndex();
}

bool Disp3DViewDockPanel::xoyGridChecked() const
{
    return ui->cbox_XOYGrid->isChecked();
}

bool Disp3DViewDockPanel::xozGridChecked() const
{
    return ui->cbox_XOZGrid->isChecked();
}

bool Disp3DViewDockPanel::zoyGridChecked() const
{
    return ui->cbox_ZOYGrid->isChecked();
}

void Disp3DViewDockPanel::resetGridChecks()
{
    ui->cbox_XOYGrid->setChecked(false);
    ui->cbox_XOZGrid->setChecked(false);
    ui->cbox_ZOYGrid->setChecked(false);
    ui->cbox_GridAble->setChecked(false);
}
