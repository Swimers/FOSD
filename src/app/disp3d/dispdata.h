#ifndef DISPDATA_H
#define DISPDATA_H

#include <QDialog>
#include <Timer.h>

#include <analyzepackets.h>
#include <rainbowife.h>

namespace Ui {
class dispdata;
}

class dispdata : public QDialog
{
    Q_OBJECT

public:
    explicit dispdata(QWidget *parent = nullptr);
    ~dispdata();

public:
    void refreshTable();
    void startAutoRefresh(int intervalMs);
    void stopAutoRefresh();

    // 弃用
    void GetPackPtr(analyzepackets *ptr){m_dataParser = ptr;}
    // 从IFE获取数据
    void GetRainbowIFE(rainbowIFE *ptr){m_rainbowIFE = ptr;}

private slots:
    void on_checkBox_toggled(bool checked);

private:
    QTimer* m_refreshTimer = nullptr;  // 自动刷新定时器

    float strain_array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    float temperature_array[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    float freedom_array[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

private:
    Ui::dispdata *ui;

    analyzepackets *m_dataParser = nullptr;
    rainbowIFE *m_rainbowIFE = nullptr;
};

#endif // DISPDATA_H
