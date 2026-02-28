#include "trackingpointdatatable.h"
#include "ui_trackingpointdatatable.h"

#include <QTimer>

trackingpointdatatable* trackingpointdatatable::m_instance = nullptr;

trackingpointdatatable::trackingpointdatatable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trackingpointdatatable)
{
    ui->setupUi(this);

    InitWindow();
}

trackingpointdatatable::~trackingpointdatatable()
{
    delete ui;
    m_instance = nullptr;  // 销毁时清空单例指针
}

void trackingpointdatatable::InitWindow()
{
    setWindowTitle("追踪点变形量数据表");
    ui->tab_datatable->setColumnCount(10);
    ui->tab_datatable->setHorizontalHeaderLabels({"表格显示", "ID", "名称", "初始X(mm)", "初始Y(mm)", "初始Z(mm)", "变形ΔX(mm)", "变形ΔY(mm)", "变形ΔZ(mm)", ""});
    // 设置表格属性
    ui->tab_datatable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_datatable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tab_datatable->setAlternatingRowColors(true);
    ui->tab_datatable->horizontalHeader()->setStretchLastSection(true);
    // 设置列宽
    ui->tab_datatable->setColumnWidth(0, 80);   // 3D显示复选框
    ui->tab_datatable->setColumnWidth(1, 40);   // ID
    ui->tab_datatable->setColumnWidth(2, 60);  // 名称
    for (int i = 3; i < 9; i++){
        ui->tab_datatable->setColumnWidth(i, 100);  // 坐标和变形量列
    }

//   ui->tab_datatable->setColumnWidth(0, 80);   // 显示复选框列
//   ui->tab_datatable->setColumnWidth(1, 50);   // ID列
//   ui->tab_datatable->setColumnWidth(2, 100);  // 名称列
//   ui->tab_datatable->setColumnWidth(3, 70);   // X坐标列
//   ui->tab_datatable->setColumnWidth(4, 70);   // Y坐标列
//   ui->tab_datatable->setColumnWidth(5, 70);   // Z坐标列

    // 创建自动刷新定时器
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &trackingpointdatatable::RefreshTable);
}

void trackingpointdatatable::InitializeTableRows()
{
    if (!m_disp3dmodel->Flag_Actor) {
        return;
    }

    int rowCount = m_trackingpoint->trackingPoints.size();

    for (int i = 0; i < rowCount; ++i) {
        TrackingPoint* tp = m_trackingpoint->trackingPoints[i];

        // 第0列：表格显示复选框（控制该行是否在表格中显示）
        QCheckBox* rowCheckBox = new QCheckBox();
        bool isChecked = rowVisibility.contains(i) ? rowVisibility[i] : true;
        rowCheckBox->setChecked(isChecked);
        rowVisibility[i] = isChecked;

        // 连接复选框信号
        connect(rowCheckBox, &QCheckBox::stateChanged, [this, i](int state) {
            rowVisibility[i] = (state == Qt::Checked);
        });

        // 创建容器widget使复选框居中显示
        QWidget* checkWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(checkWidget);
        layout->addWidget(rowCheckBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->tab_datatable->setCellWidget(i, 0, checkWidget);

        // 创建所有表格项（初始化时创建一次）
        ui->tab_datatable->setItem(i, 1, new QTableWidgetItem());  // ID
        ui->tab_datatable->setItem(i, 2, new QTableWidgetItem());  // 名称
        ui->tab_datatable->setItem(i, 3, new QTableWidgetItem());  // 初始X
        ui->tab_datatable->setItem(i, 4, new QTableWidgetItem());  // 初始Y
        ui->tab_datatable->setItem(i, 5, new QTableWidgetItem());  // 初始Z

        // 变形量列（带绿色背景）
        QTableWidgetItem* dispX = new QTableWidgetItem();
        QTableWidgetItem* dispY = new QTableWidgetItem();
        QTableWidgetItem* dispZ = new QTableWidgetItem();
        dispX->setBackground(QColor(200, 255, 200));
        dispY->setBackground(QColor(200, 255, 200));
        dispZ->setBackground(QColor(200, 255, 200));
        ui->tab_datatable->setItem(i, 6, dispX);
        ui->tab_datatable->setItem(i, 7, dispY);
        ui->tab_datatable->setItem(i, 8, dispZ);
    }
}

void trackingpointdatatable::StartAutoRefresh(int intervalMs)
{
    if (m_refreshTimer) {
        m_refreshTimer->start(intervalMs);
    }
}

void trackingpointdatatable::StopAutoRefresh()
{
    if (m_refreshTimer) {
        m_refreshTimer->stop();
    }
}

void trackingpointdatatable::RefreshTable()
{
    if (!m_disp3dmodel->Flag_Actor) {
        return;
    }

    int rowCount = m_trackingpoint->trackingPoints.size();

    // 只在行数变化时才重新初始化表格
    if (ui->tab_datatable->rowCount() != rowCount) {
        ui->tab_datatable->setRowCount(rowCount);
        InitializeTableRows();  // 创建所有控件
    }

    // 只更新数据内容，不创建新对象（性能优化的关键）
    for (int i = 0; i < rowCount; ++i) {
        TrackingPoint* tp = m_trackingpoint->trackingPoints[i];
        ui->tab_datatable->item(i, 1)->setText(QString::number(tp->id));
        ui->tab_datatable->item(i, 2)->setText(tp->name);
        ui->tab_datatable->item(i, 3)->setText(QString::number(tp->originalPos[0], 'f', 3));
        ui->tab_datatable->item(i, 4)->setText(QString::number(tp->originalPos[1], 'f', 3));
        ui->tab_datatable->item(i, 5)->setText(QString::number(tp->originalPos[2], 'f', 3));
        ui->tab_datatable->item(i, 6)->setText(QString::number(tp->currentDisp[0], 'f', 3));
        ui->tab_datatable->item(i, 7)->setText(QString::number(tp->currentDisp[1], 'f', 3));
        ui->tab_datatable->item(i, 8)->setText(QString::number(tp->currentDisp[2], 'f', 3));
    }

    // 更新统计信息
    ui->lab_Count->setText(QString("共 %1 个追踪点").arg(rowCount));
}

trackingpointdatatable *trackingpointdatatable::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new trackingpointdatatable(parent);
    }
    return m_instance;
}


void trackingpointdatatable::on_cbox_AutoRefresh_toggled(bool checked)
{
    if (checked) {
        StartAutoRefresh(100);  // 100ms刷新一次
    } else {
        StopAutoRefresh();
    }
}

void trackingpointdatatable::on_pbtn_ShowAll_clicked()
{
    for (int i = 0; i < ui->tab_datatable->rowCount(); ++i) {
        QWidget* widget = ui->tab_datatable->cellWidget(i, 0);
        if (widget) {
            QCheckBox* checkBox = widget->findChild<QCheckBox*>();
            if (checkBox) {
                checkBox->setChecked(true);
            }
        }
    }
}

void trackingpointdatatable::on_pbtn_HideAll_clicked()
{
    for (int i = 0; i < ui->tab_datatable->rowCount(); ++i) {
        QWidget* widget = ui->tab_datatable->cellWidget(i, 0);
        if (widget) {
            QCheckBox* checkBox = widget->findChild<QCheckBox*>();
            if (checkBox) {
                checkBox->setChecked(false);
            }
        }
    }
}

void trackingpointdatatable::on_pbtn_ShowSelected_clicked()
{
    for (int i = 0; i < ui->tab_datatable->rowCount(); ++i) {
        bool shouldShow = rowVisibility.value(i, true);
        ui->tab_datatable->setRowHidden(i, !shouldShow);
    }
}

void trackingpointdatatable::on_pbtn_ShowAllRows_clicked()
{
    for (int i = 0; i < ui->tab_datatable->rowCount(); ++i) {
        ui->tab_datatable->setRowHidden(i, false);
    }
}

void trackingpointdatatable::on_pbtn_Refresh_clicked()
{
    RefreshTable();
}
