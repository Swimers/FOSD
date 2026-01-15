#ifndef DISP2DPARA_H
#define DISP2DPARA_H

#include <QWidget>
#include "IFE.h"

class IFE;

namespace Ui {
class disp2dpara;
}

class disp2dpara : public QWidget
{
    Q_OBJECT

public:
    explicit disp2dpara(QWidget *parent = nullptr);
    ~disp2dpara();

    void GetIFEPtr(IFE* ife_temp){ife = ife_temp;}
    void loadConfig();  //导入配置
    void saveConfig();  //保存配置
    void init();

private slots:
    void on_pbtn_SenerSet_clicked();
    void on_pbtn_SenerCancel_clicked();

signals:
    void DataSeted();

private:
    Ui::disp2dpara *ui;
    IFE* ife;
};

#endif // DISP2DPARA_H
