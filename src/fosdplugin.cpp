#include <cstdio>
#include <QFile>
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>

#include <vtkOutputWindow.h>
#include <vtkAutoInit.h>

#include "rainbowmainwindow.h"   // 你的主界面
// #include "uiinterface.h"      // 如果你也需要 MIF::registerBaseDlgs 之类

// 如果你项目需要 VTK_MODULE_INIT，建议放在库里任意 cpp 顶层即可
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

static RainbowMainWindow* g_mainWindow = nullptr;
static bool g_initedOnce = false;

static void initOnce()
{
    if (g_initedOnce) return;
    g_initedOnce = true;

    // 1) OpenGL 格式：你参考代码用的是 QVTKOpenGLNativeWidget::defaultFormat()
    //    这是 VTK+Qt 场景更稳的做法
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    // 2) VTK 警告窗口屏蔽
    vtkOutputWindow::SetGlobalWarningDisplay(0);

    // 3) QSS：注意这是全局样式，若宿主也设置样式可能冲突（但你要的话可以保留）
    if (qApp) {
        QFile qssFile("./res/QSS/MacOS.qss");
        if (qssFile.open(QFile::ReadOnly)) {
            qApp->setStyleSheet(qssFile.readAll());
            qssFile.close();
        }
    }
}

extern "C" Q_DECL_EXPORT void dllStartPlugin(void)
{
    std::printf("[RainbowPlugin] dllStartPlugin\n");

    // 宿主必须已经创建 QApplication（插件不负责）
    if (!qApp) {
        std::printf("[RainbowPlugin] ERROR: qApp is null. Host must create QApplication before loading plugin.\n");
        return;
    }

    initOnce();

    // 创建并显示界面
    if (!g_mainWindow) {
        g_mainWindow = new RainbowMainWindow();
        g_mainWindow->show(); // 如果你的插件是嵌入式UI，通常这里不 show，而是交给宿主嵌入
    }
}

extern "C" Q_DECL_EXPORT void dllInitialisePlugin(void)
{
    std::printf("[RainbowPlugin] dllInitialisePlugin\n");
    // 放“启动后”的初始化逻辑：注册对话框、注册菜单、绑定信号槽、订阅数据等
    // 例如：
    // MIF::registerBaseDlgs(g_mainWindow);
}

extern "C" Q_DECL_EXPORT void dllStopPlugin(void)
{
    std::printf("[RainbowPlugin] dllStopPlugin\n");

    // 停止业务（线程/监听/定时器/订阅）要先停，再销毁 UI
    if (g_mainWindow) {
        g_mainWindow->close();
        delete g_mainWindow;
        g_mainWindow = nullptr;
    }
}

extern "C" Q_DECL_EXPORT void dllShutdownPlugin(void)
{
    std::printf("[RainbowPlugin] dllShutdownPlugin\n");
    // 进程退出前或插件彻底卸载前的最后清理
    // 通常这里不再碰 Qt UI（因为可能事件循环都要结束了）
}
