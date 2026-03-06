#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

#include <cstdio>

#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 宿主自己也可以有一个主窗口（可选）
    MainWindow *w = new MainWindow();
    w->show();

    return a.exec();
}
