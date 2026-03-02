#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

#include <windows.h>
#include <cstdio>

using PluginFn = void (*)();

static HMODULE g_plugin = nullptr;
static PluginFn g_start = nullptr;
static PluginFn g_init  = nullptr;
static PluginFn g_stop  = nullptr;
static PluginFn g_shutdown = nullptr;

static bool loadPlugin(const char* dllName)
{
    g_plugin = LoadLibraryA(dllName);
    if (!g_plugin) {
        std::printf("[Host] LoadLibrary failed: %s\n", dllName);
        return false;
    }

    g_start    = (PluginFn)GetProcAddress(g_plugin, "dllStartPlugin");
    g_init     = (PluginFn)GetProcAddress(g_plugin, "dllInitialisePlugin");
    g_stop     = (PluginFn)GetProcAddress(g_plugin, "dllStopPlugin");
    g_shutdown = (PluginFn)GetProcAddress(g_plugin, "dllShutdownPlugin");

    if (!g_start || !g_init || !g_stop || !g_shutdown) {
        std::printf("[Host] GetProcAddress failed. "
                    "start=%p init=%p stop=%p shutdown=%p\n",
                    (void*)g_start, (void*)g_init, (void*)g_stop, (void*)g_shutdown);
        return false;
    }

    std::printf("[Host] Plugin loaded OK\n");
    return true;
}

static void unloadPlugin()
{
    if (g_plugin) {
        FreeLibrary(g_plugin);
        g_plugin = nullptr;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 宿主自己也可以有一个主窗口（可选）
    QMainWindow hostWindow;
    hostWindow.setWindowTitle("PluginHost");
    hostWindow.resize(900, 600);
    hostWindow.show();

    // 关键：加载插件（确保插件 dll 在 exe 同目录）
    if (!loadPlugin("FOSD_ProPlugin.dll")) {
        QMessageBox::critical(nullptr, "PluginHost", "Failed to load plugin DLL or symbols.");
        return 1;
    }

    // 调用生命周期
    g_start();
    g_init();

    // 退出前清理：用 Qt 的 aboutToQuit 最稳
    QObject::connect(&a, &QCoreApplication::aboutToQuit, [](){
        if (g_stop) g_stop();
        if (g_shutdown) g_shutdown();
        unloadPlugin();
    });

    return a.exec();
}
