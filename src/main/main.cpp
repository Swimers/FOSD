#include "login.h"
#include <QApplication>
#include <QSurfaceFormat>

#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);  // 指定 OpenGL 版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);  // 抗锯齿
    QSurfaceFormat::setDefaultFormat(format);

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

//    VTK_MODULE_INIT(vtkRenderingOpenGL2);    // 必须有：OpenGL 渲染后端
//    VTK_MODULE_INIT(vtkInteractionStyle);    // 交互风格
//    VTK_MODULE_INIT(vtkRenderingFreeType);   // 文本渲染（若用到）

    loginwindow * m_loginwindow = loginwindow::getInstance();
    m_loginwindow->show();

    return a.exec();
}
