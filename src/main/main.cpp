#include "login.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char *argv[])
{
    //OpenGL congif
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);  // 指定 OpenGL 版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);  // 抗锯齿
    QSurfaceFormat::setDefaultFormat(format);

    VTK_MODULE_INIT(vtkRenderingOpenGL2);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);

    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //屏蔽vtkOutputWindow窗口及其发出的警告
    vtkOutputWindow::SetGlobalWarningDisplay(0); //加入

    //加载qss文件
//    QFile qssFile("./QSS/MacOS.qss");//1.
//    if(qssFile.open(QFile::ReadOnly)){//2.
//        a.setStyleSheet(qssFile.readAll());//3.
//    }
//    qssFile.close();//4.

    loginwindow * m_loginwindow = loginwindow::getInstance();
    m_loginwindow->show();

    return a.exec();
}
