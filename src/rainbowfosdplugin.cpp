#include "rainbowmainwindow.h"
//#include <QApplication>
#include <QFile>
#include <vtkOutputWindow.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QSurfaceFormat>
#include "data.h"

#include <stdio.h>
#include "uiinterface.h"

DemoData *data = new DemoData();

extern "C" void dllStartPlugin(void){
    printf("FOSD_XD extern \"C\" void dllStartPlugin(void)\n");

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());  // 关键步骤：必须在创建任何QVTKOpenGLNativeWidget之前设置OpenGL表面格式
    vtkOutputWindow::SetGlobalWarningDisplay(0);                                //屏蔽vtkOutputWindow窗口及其发出的警告

    printf("PowerDev extern \"C\" void dllStartPlugin(void)\n");

    RainbowMainWindow* p_mainwindow = new RainbowMainWindow();
    MIF::registerBaseDlgs(p_mainwindow);

    data->listen();

    QObject::connect(data, &DemoData::signal_data, p_mainwindow->m_rainbowIFE, &rainbowIFE::getDemoData); //此处加入插件的启动时的执行代码, 插件加载时，首先执行启动
}

extern "C" void dllInitialisePlugin(void){
    printf("FOSD_XD extern \"C\" void dllInitialisePlugin(void)\n");    //此处加入插件的初始化时的执行代码, 插件启动完成后，运行初始化
}

extern "C" void dllStopPlugin(void){
    printf("FOSD_XD extern \"C\" void dllStopPlugin(void)\n");
    delete data;
}

extern "C" void dllShutdownPlugin(void){
    printf("FOSD_XD extern \"C\" void dllShutdownPlugin(void)\n");      //此处加入插件的关闭时的执行代码
}
