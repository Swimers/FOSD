#ifndef POINTTABLE_H
#define POINTTABLE_H

#include <QWidget>
#include "config.h"
#include "IFE.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QTimer>


namespace Ui {
class PointTable;
}

class PointTable : public QWidget
{
    Q_OBJECT

public:
    static PointTable* instance(QWidget* parent = nullptr,IFE* ife = nullptr);

    // 排序方式
    enum SortMode{
        ordinal = 0,
        coordinate,
        disp
    };

private slots:
    void on_btnok_clicked();
    void on_btnstop_clicked();


    void on_comboBox_currentIndexChanged(int index);

private:
    explicit PointTable(QWidget *parent = nullptr, IFE* ife = nullptr);
    ~PointTable();

    void initializeTableHeader();
    void updateTable();



private:
    static PointTable* s_instance;  // 静态成员
    Ui::PointTable *ui;
    IFE* ife;
    QStandardItemModel* tableModel;
    QTimer* updateTimer;
    bool isRunning;
    SortMode currentSortMode;
};

#endif // POINTTABLE_H
