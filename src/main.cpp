#include "login.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>

#include <rainbowmainwindow.h>

int main(int argc, char *argv[])
{
    //OpenGL初始化
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);  // 指定 OpenGL 版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);  // 抗锯齿
    QSurfaceFormat::setDefaultFormat(format);

    //VTK相关初始化
    VTK_MODULE_INIT(vtkRenderingOpenGL2);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);
    vtkOutputWindow::SetGlobalWarningDisplay(0); //屏蔽vtkOutputWindow窗口及其发出的警告

    QApplication a(argc, argv);

    //加载qss文件
    QFile qssFile("./data/QSS/MaterialDark-B.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
    }
    qssFile.close();

    //直接启动主窗口
    RainbowMainWindow * w = new RainbowMainWindow();
    w->show();
    //loginwindow * m_loginwindow = loginwindow::getInstance();
    //m_loginwindow->show();

    return a.exec();
}
