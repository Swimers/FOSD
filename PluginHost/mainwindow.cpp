#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QMessageBox>

#include <dlfcn.h>
#include <cstdio>

using PluginFn = void (*)();

static void* g_plugin = nullptr;
static PluginFn g_start = nullptr;
static PluginFn g_init  = nullptr;
static PluginFn g_stop  = nullptr;
static PluginFn g_shutdown = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!loadPlugin("./libFOSD_ProPlugin.so")) {
        QMessageBox::critical(this, "PluginHost", "Failed to load plugin SO or symbols.");
    }
}

MainWindow::~MainWindow()
{
    unloadPlugin();
    delete ui;
}

bool MainWindow::loadPlugin(const char *soName)
{
    g_plugin = dlopen(soName, RTLD_NOW);
    if (!g_plugin) {
        std::printf("[Host] dlopen failed: %s\n", dlerror());
        return false;
    }

    dlerror(); // 清空旧错误

    g_start    = reinterpret_cast<PluginFn>(dlsym(g_plugin, "dllStartPlugin"));
    g_init     = reinterpret_cast<PluginFn>(dlsym(g_plugin, "dllInitialisePlugin"));
    g_stop     = reinterpret_cast<PluginFn>(dlsym(g_plugin, "dllStopPlugin"));
    g_shutdown = reinterpret_cast<PluginFn>(dlsym(g_plugin, "dllShutdownPlugin"));

    const char* err = dlerror();
    if (err) {
        std::printf("[Host] dlsym failed: %s\n", err);
        dlclose(g_plugin);
        g_plugin = nullptr;
        return false;
    }

    std::printf("[Host] Plugin loaded OK\n");
    return true;
}

void MainWindow::unloadPlugin()
{
    if (g_plugin) {
        dlclose(g_plugin);
        g_plugin = nullptr;
        g_start = nullptr;
        g_init = nullptr;
        g_stop = nullptr;
        g_shutdown = nullptr;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (g_start) g_start();
    if (g_init)  g_init();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (g_stop) g_stop();
    if (g_shutdown) g_shutdown();
    unloadPlugin();
}
