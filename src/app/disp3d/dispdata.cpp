#include "dispdata.h"
#include "ui_dispdata.h"

#include <QTimer>
#include <global.h>

#include <global.h>

dispdata::dispdata(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dispdata)
{
    ui->setupUi(this);
    setWindowTitle("温度应变显示");

    // ==================== 表格初始化 ====================
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"编号", "应变", "温度","自由度"});

    // 设置表格属性
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    // 设置列宽
    ui->tableWidget->setColumnWidth(0, 60);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 120);

    // ==================== 初始化 12 行 ====================
    ui->tableWidget->setRowCount(16);

    for (int row = 0; row < 16; ++row)
    {
        // 编号
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(row + 1));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 0, idItem);

        // 应变
        QTableWidgetItem *strainItem = new QTableWidgetItem("0.000000");
        strainItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 1, strainItem);

        // 温度
        QTableWidgetItem *tempItem = new QTableWidgetItem("0.00");
        tempItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 2, tempItem);

        // 温度
        QTableWidgetItem *freedomItem = new QTableWidgetItem("0.0000");
        freedomItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, 3, freedomItem);
    }

    // 创建自动刷新定时器
    m_refreshTimer = new QTimer(this);
    connect(m_refreshTimer, &QTimer::timeout, this, &dispdata::refreshTable);
}

dispdata::~dispdata()
{
    delete ui;
}

void dispdata::refreshTable()
{
    QMutexLocker locker(&mutexStrain);
    for (int i = 0; i < 12; ++i){
        strain_array[i] = m_rainbowIFE->all_data.strian[i];
        temperature_array[i] = m_rainbowIFE->all_data.temper[i];
    }
    for (int i = 0; i < 16; ++i){
        freedom_array[i] = m_rainbowIFE->all_data.free[i];
    }

    for (int i = 0; i < 12; ++i){
        ui->tableWidget->item(i, 1)->setText(QString::number(strain_array[i], 'f', 6));
        ui->tableWidget->item(i, 2)->setText(QString::number(temperature_array[i], 'f', 2));
    }
    for (int i = 0; i < 16; ++i){
        ui->tableWidget->item(i, 3)->setText(QString::number(freedom_array[i], 'f', 4));
    }
}

void dispdata::startAutoRefresh(int intervalMs) {
    if (m_refreshTimer) {
        m_refreshTimer->start(intervalMs);
    }
}

void dispdata::stopAutoRefresh() {
    if (m_refreshTimer) {
        m_refreshTimer->stop();
    }
}

void dispdata::on_checkBox_toggled(bool checked)
{
    if (checked) {
        startAutoRefresh(100);  // 100ms刷新一次
    } else {
        stopAutoRefresh();
    }
}
