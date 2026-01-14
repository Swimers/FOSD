#ifndef PARAREVISEWINDOW_H
#define PARAREVISEWINDOW_H

#include <QMainWindow>
#include "IFE.h"


class IFE;
namespace Ui {
class ParaReviseWindow;
}

class ParaReviseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParaReviseWindow(QWidget *parent = nullptr);
    ~ParaReviseWindow();

    void GetIFEPtr(IFE* ife_temp){ife = ife_temp;}
    void loadConfig();  //导入配置
    void saveConfig();  //保存配置
    void init();



private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::ParaReviseWindow *ui;
    IFE* ife;

};

#endif // PARAREVISEWINDOW_H
