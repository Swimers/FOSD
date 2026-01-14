<<<<<<< Updated upstream
#include "login.h"
#include <QApplication>
#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>
=======
﻿//#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include "logwidget.h"
#include <vtkOutputWindow.h>
#include "config.h"
#pragma execution_character_set("utf-8")
>>>>>>> Stashed changes


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
<<<<<<< Updated upstream
    //MainWindow w;
    //w.show();
=======

>>>>>>> Stashed changes
    //屏蔽vtkOutputWindow窗口及其发出的警告
    vtkOutputWindow::SetGlobalWarningDisplay(0); //加入

    //加载qss文件
//    QFile qssFile("./QSS/MacOS.qss");//1.
//    if(qssFile.open(QFile::ReadOnly)){//2.
//        a.setStyleSheet(qssFile.readAll());//3.
//    }
//    qssFile.close();//4.

<<<<<<< Updated upstream
    VTK_MODULE_INIT(vtkRenderingOpenGL2);    // 必须有：OpenGL 渲染后端
    VTK_MODULE_INIT(vtkInteractionStyle);    // 交互风格
    VTK_MODULE_INIT(vtkRenderingFreeType);   // 文本渲染（若用到）

    loginwindow * m_loginwindow = loginwindow::getInstance();
    m_loginwindow->show();
=======
    // 初始化共享资源
    if(!Config::getInstance().loadFromFile()){
        Config::getInstance().saveToFile();
    }

    LogWidget* loginWidget = LogWidget::instance();
    loginWidget->show();

    QFile file("numbers.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件!";
        return 1;
    }

    QTextStream out(&file);
    for (int i = 1; i <= 86570; ++i) {
        out << i;
        if (i != 86570) {
            out << '\t'; // 使用Tab字符分隔
        }
    }
    file.close();
>>>>>>> Stashed changes

    return a.exec();
}
