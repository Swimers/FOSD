//#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include "logwidget.h"
#include <vtkOutputWindow.h>
#pragma execution_character_set("utf-8")


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //屏蔽vtkOutputWindow窗口及其发出的警告
    vtkOutputWindow::SetGlobalWarningDisplay(0); //加入

    //加载qss文件
    QFile qssFile("./QSS/MacOS.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
    }
    qssFile.close();//4.

    LogWidget* loginWidget = LogWidget::instance();
    loginWidget->show();


    return a.exec();
}
