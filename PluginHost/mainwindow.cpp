#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!loadPlugin("FOSD_ProPlugin.dll")) {
        QMessageBox::critical(nullptr, "PluginHost", "Failed to load plugin DLL or symbols.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadPlugin(const char *dllName)
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

void MainWindow::unloadPlugin()
{
    if (g_plugin) {
        FreeLibrary(g_plugin);
        g_plugin = nullptr;
    }
}

void MainWindow::on_pushButton_clicked()
{
    g_start();
    g_init();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (g_stop) g_stop();
    if (g_shutdown) g_shutdown();
    unloadPlugin();
}
