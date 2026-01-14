#include "pointtable.h"
#include "ui_pointtable.h"
#include <qDebug>
#pragma execution_character_set("utf-8")

struct PointRow {
    int id;
    double x0, y0, z0;     // 初始坐标
    double dx, dy, dz;     // 变形值
};

PointTable* PointTable::s_instance = nullptr;

PointTable::PointTable(QWidget* parent, IFE* ife)
    : QWidget(parent), ui(new Ui::PointTable), ife(ife), tableModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    this->setWindowTitle("关键点数据表");


    // 初始化表格模型
    ui->tableView->setModel(tableModel);
    initializeTableHeader();

    // 定时器
    updateTimer = new QTimer(this);
    isRunning = false;

    connect(updateTimer, &QTimer::timeout, this, &PointTable::updateTable);

}

PointTable::~PointTable()
{
    delete ui;
}

void PointTable::initializeTableHeader()
{
    tableModel->clear();
    QStringList headers = {"编号", "x初始坐标", "y初始坐标", "z初始坐标",
                           "x变形", "y变形", "z变形"};
    tableModel->setHorizontalHeaderLabels(headers);

    // 表头样式设置（字体、对齐）
    QFont headerFont("Microsoft YaHei", 10, QFont::Bold);
    for (int i = 0; i < headers.size(); ++i) {
        QStandardItem* item = new QStandardItem(headers[i]);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFont(headerFont);
        tableModel->setHorizontalHeaderItem(i, item);
    }

    // 设置表格属性
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 平均分列宽
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);       // 表头居中
    ui->tableView->verticalHeader()->setVisible(false);                             // 隐藏行号
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);            // 整行选中
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);             // 禁止编辑

    // 表格整体字体美化
    ui->tableView->setFont(QFont("Microsoft YaHei", 9));
}

PointTable* PointTable::instance(QWidget* parent, IFE* ife)
{
    if (!s_instance)
        s_instance = new PointTable(parent, ife);
    return s_instance;
}

void PointTable::on_btnok_clicked()
{
    if (!isRunning) {
        isRunning = true;
        updateTimer->start(200);  // 每 500ms 更新一次表格，可根据需要调整
        ui->btnok->setText("运行中...");
        ui->btnok->setEnabled(false);  // 禁止重复点击
    }
}

void PointTable::on_btnstop_clicked()
{
    if (isRunning) {
        updateTimer->stop();
        isRunning = false;
        ui->btnok->setText("开始");
        ui->btnok->setEnabled(true);
    }
}



void PointTable::updateTable()
{
    tableModel->removeRows(0, tableModel->rowCount());

    std::vector<PointRow> rows;

    auto flagPoints = Config::getInstance().getFlagPoint1();
    for (auto it = flagPoints.begin(); it != flagPoints.end(); ++it){
        int id = it->first;
        const Point3f& pt0 = it->second;

        // 变形值由 ife 计算，假设接口如下：
        double Globalbeampointdisplace[3];
        ife->N_shapefunction(pt0.x,pt0.y,pt0.z,ife->node_u,Globalbeampointdisplace);
        //qDebug()<<Globalbeampointdisplace[0]<<Globalbeampointdisplace[1]<<Globalbeampointdisplace[2];
        rows.push_back(PointRow{id,
                                pt0.x*1000, pt0.y*1000, pt0.z*1000,
                                Globalbeampointdisplace[0]*1000,
                                Globalbeampointdisplace[1]*1000,
                                Globalbeampointdisplace[2]*1000});
    }
    if(currentSortMode == coordinate){
        std::sort(rows.begin(), rows.end(), [](const PointRow& a, const PointRow& b) {
                    if (a.x0 != b.x0) return a.x0 < b.x0;
                    if (a.y0 != b.y0) return a.y0 < b.y0;
                    return a.z0 < b.z0;
                });
    }
    else if(currentSortMode == ordinal){
        std::sort(rows.begin(), rows.end(), [](const PointRow& a, const PointRow& b) {
                    return a.id < b.id;
                });
    }
    else if(currentSortMode == disp){
        std::sort(rows.begin(), rows.end(), [](const PointRow& a, const PointRow& b) {
                    if (a.dz != b.dz) return a.dz < b.dz;
                    if (a.dx != b.dx) return a.dx < b.dx;
                    return a.dy < b.dy;
                });
    }

    // 插入表格
    QFont font("Microsoft YaHei", 9);
    for (const PointRow& row : rows)
    {
        QList<QStandardItem*> items;
        auto makeItem = [&](QString text) {
            QStandardItem* item = new QStandardItem(text);
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
            return item;
        };

        items << makeItem(QString::number(row.id))
              << makeItem(QString::number(row.x0, 'f', 3))
              << makeItem(QString::number(row.y0, 'f', 3))
              << makeItem(QString::number(row.z0, 'f', 3))
              << makeItem(QString::number(row.dx, 'f', 3))
              << makeItem(QString::number(row.dy, 'f', 3))
              << makeItem(QString::number(row.dz, 'f', 3));

        tableModel->appendRow(items);
    }
}

void PointTable::on_comboBox_currentIndexChanged(int index)
{
    //qDebug()<<index;
    currentSortMode = static_cast<SortMode>(index);
    updateTable();    // 重新排序并刷新表格
}
