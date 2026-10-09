#include "login.h"
#include "oldmainwindow.h"

#include <QApplication>
#include <QDir>
#include <QSurfaceFormat>

#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>
#include <QVTKOpenGLNativeWidget.h>

#include <rainbowmainwindow.h>

int main(int argc, char *argv[])
{
#ifdef FOSD_RK3588_ARM_LINUX
    if (qEnvironmentVariableIsEmpty("XDG_RUNTIME_DIR"))
        qputenv("XDG_RUNTIME_DIR", "/run");
    if (qEnvironmentVariableIsEmpty("WAYLAND_DISPLAY"))
        qputenv("WAYLAND_DISPLAY", "wayland-0");
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "wayland");
    const QString dataRoot = qEnvironmentVariable(
        "FOSD_DATA_ROOT", QStringLiteral("/root/Qt_Project/Qt_Test_Compilation"));
    if (QDir(dataRoot).exists())
        QDir::setCurrent(dataRoot);
#endif

    //OpenGL初始化
#ifdef FOSD_RK3588_ARM_LINUX
    QSurfaceFormat format = QVTKOpenGLNativeWidget::defaultFormat();
    format.setSamples(0);
#else
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 2);  // 指定 OpenGL 版本
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSamples(4);  // 抗锯齿
#endif
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

    //注册蒲城项目界面
    //RainbowMainWindow * w = new RainbowMainWindow();
    //w->show();

    //注册登录界面
    //loginwindow * m_loginwindow = loginwindow::getInstance();
    //m_loginwindow->show();

    //直接注册老主界面
    OldMainWindow *  m_MainWindow = new OldMainWindow("1");
    m_MainWindow->showFullScreen();

    return a.exec();
}
