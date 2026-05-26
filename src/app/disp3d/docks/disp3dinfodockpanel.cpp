#include "disp3dinfodockpanel.h"
#include "ui_disp3dinfodockpanel.h"

#include <QDockWidget>

Disp3DInfoDockPanel::Disp3DInfoDockPanel(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Disp3DInfoDockPanel),
      m_dock(new QDockWidget(tr("可视化参数"), parent))
{
    ui->setupUi(this);

    m_dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    m_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dock->setWidget(this);
}

Disp3DInfoDockPanel::~Disp3DInfoDockPanel()
{
    delete ui;
}

QString Disp3DInfoDockPanel::dockId() const
{
    return Disp3DDockIds::Information;
}

QDockWidget *Disp3DInfoDockPanel::dockWidget() const
{
    return m_dock;
}

bool Disp3DInfoDockPanel::defaultVisible() const
{
    return false;
}

void Disp3DInfoDockPanel::setModelInfo(uint pointCount, uint polyCount, uint keyPointCount)
{
    ui->lab_NumPoints->setText(QString::number(pointCount));
    ui->lab_NumPolys->setText(QString::number(polyCount));
    ui->lab_NumKeyPoints->setText(QString::number(keyPointCount));
}

void Disp3DInfoDockPanel::setMaxDeformation(const QString &value)
{
    ui->lab_MaxDeformation->setText(value);
}

void Disp3DInfoDockPanel::clearModelInfo()
{
    ui->lab_NumPoints->clear();
    ui->lab_NumPolys->clear();
    ui->lab_NumKeyPoints->clear();
    ui->lab_MaxDeformation->clear();
}
