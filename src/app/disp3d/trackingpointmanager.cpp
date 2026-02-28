#include "trackingpointmanager.h"
#include "ui_trackingpointmanager.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QCheckBox>

trackingpointmanager* trackingpointmanager::m_instance = nullptr;

trackingpointmanager::trackingpointmanager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trackingpointmanager)
{
    ui->setupUi(this);

    InitWindow();
}

trackingpointmanager::~trackingpointmanager()
{
    delete ui;
    m_instance = nullptr;  // 销毁时清空单例指针
}

trackingpointmanager *trackingpointmanager::instance(QWidget *parent)
{
    if (m_instance == nullptr) {
        m_instance = new trackingpointmanager(parent);
    }
    return m_instance;
}

void trackingpointmanager::InitWindow()
{
    setWindowTitle("追踪点管理器");

    ui->tab_pointtable->setColumnCount(7);  // 设置列数为6
    ui->tab_pointtable->setHorizontalHeaderLabels({"显示", "ID", "名称", "X (mm)", "Y (mm)", "Z (mm)", ""});  // 设置表头
    ui->tab_pointtable->setSelectionBehavior(QAbstractItemView::SelectRows);  // 整行选择模式
    ui->tab_pointtable->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 禁止编辑
    ui->tab_pointtable->horizontalHeader()->setStretchLastSection(true);      // 最后一列自动拉伸
    ui->tab_pointtable->setColumnWidth(0, 50);   // 显示复选框列
    ui->tab_pointtable->setColumnWidth(1, 50);   // ID列
    ui->tab_pointtable->setColumnWidth(2, 100);  // 名称列
    ui->tab_pointtable->setColumnWidth(3, 80);   // X坐标列
    ui->tab_pointtable->setColumnWidth(4, 80);   // Y坐标列
    ui->tab_pointtable->setColumnWidth(5, 80);   // Z坐标列
}

void trackingpointmanager::RefreshTable()
{
    if (!m_disp3dmodel->Flag_Actor) {
        return;
    }

    // 清空表格现有内容
    ui->tab_pointtable->setRowCount(0);

    // 遍历所有追踪点，逐行添加到表格
    for (int i = 0; i < m_trackingpoint->trackingPoints.size(); ++i) {
        TrackingPoint* tp = m_trackingpoint->trackingPoints[i];
        ui->tab_pointtable->insertRow(i);  // 插入新行
        UpdataTableRow(i, tp);          // 更新该行数据
    }

    // 更新追踪点计数标签
    ui->labelCount->setText(QString("共 %1 个追踪点").arg(m_trackingpoint->trackingPoints.size()));
}

void trackingpointmanager::UpdataTableRow(int row, TrackingPoint *tp)
{
    // ==================== 第0列：显示复选框 ====================
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(tp->visible);         // 设置复选框状态
    checkBox->setProperty("pointId", tp->id);  // 存储点ID到属性中
    connect(checkBox, &QCheckBox::stateChanged, [this, row](int state) {OnVisibilityChanged(row, state); });    // 连接复选框状态改变信号到槽函数
    QWidget* checkWidget = new QWidget();   // 创建容器widget使复选框居中显示
    QHBoxLayout* layout = new QHBoxLayout(checkWidget);

    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);   // 居中对齐
    layout->setContentsMargins(0, 0, 0, 0);  // 去除边距
    ui->tab_pointtable->setCellWidget(row, 0, checkWidget);

    ui->tab_pointtable->setItem(row, 1, new QTableWidgetItem(QString::number(tp->id)));
    ui->tab_pointtable->setItem(row, 2, new QTableWidgetItem(tp->name));
    ui->tab_pointtable->setItem(row, 3, new QTableWidgetItem(QString::number(tp->originalPos[0], 'f', 2)));
    ui->tab_pointtable->setItem(row, 4, new QTableWidgetItem(QString::number(tp->originalPos[1], 'f', 2)));
    ui->tab_pointtable->setItem(row, 5, new QTableWidgetItem(QString::number(tp->originalPos[2], 'f', 2)));
}

void trackingpointmanager::OnVisibilityChanged(int row, int state)
{
    if (!m_disp3dmodel->Flag_Actor)
        return;
    // 检查行号有效性
    if (row >= 0 && row < m_trackingpoint->trackingPoints.size()) {
        uint id = m_trackingpoint->trackingPoints[row]->id;                   // 获取追踪点ID
        m_trackingpoint->AbleTrackingPoint(id, state == Qt::Checked);  // 设置可见性
    }
}

void trackingpointmanager::on_pbtn_LoadFile_clicked()
{
    if(!m_disp3dmodel->Flag_Actor){
        QMessageBox::warning(this, "警告", "模型不存在");
        return;
    }

    QString filter = tr("CSV文件 (*.csv);;文本文件 (*.txt);;所有文件 (*.*)");
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择追踪点坐标文件"), "", filter);
    if (!filePath.isEmpty()) {

        if (m_trackingpoint->LoadTrackingPointsFromFile(filePath)) {
            RefreshTable();  // 刷新表格
            QMessageBox::information(this, "成功", QString("成功加载 %1 个追踪点").arg(m_trackingpoint->trackingPoints.size()));
        } else {
            QMessageBox::warning(this, "错误", "加载追踪点文件失败");
        }
    }
    else {
        QMessageBox::information(this, tr("提示"), tr("未选择任何文件！"));
    }
}

void trackingpointmanager::on_pbtn_ClearAll_clicked()
{
    if (!m_disp3dmodel->Flag_Actor)
        return;

    // 弹出确认对话框
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "确认", "确定要清空所有追踪点吗？", QMessageBox::Yes | QMessageBox::No);

    // 用户确认后执行清空操作
    if (reply == QMessageBox::Yes) {
        m_trackingpoint->ClearTrackingPoint();  // 清空所有追踪点
        RefreshTable();                         // 刷新表格
    }
}

void trackingpointmanager::on_pbtn_SelectAll_clicked()
{
    if (!m_disp3dmodel->Flag_Actor)
        return;
    m_trackingpoint->AbleAllTrackingPoint(true);  // 设置所有点可见
    RefreshTable();                               // 刷新表格更新复选框状态
}

void trackingpointmanager::on_pbtn_SelectNone_clicked()
{
    if (!m_disp3dmodel->Flag_Actor)
        return;
    m_trackingpoint->AbleAllTrackingPoint(false);  // 设置所有点隐藏
    RefreshTable();                               // 刷新表格更新复选框状态
}

void trackingpointmanager::on_pbtn_Refresh_clicked()
{
    RefreshTable();
    m_disp3dmodel->GetVTKRenderWindow()->Render();
}
