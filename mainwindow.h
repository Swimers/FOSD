#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <disp3dwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    disp3dwindow* m_disp3dwindow = nullptr;
};

#endif // MAINWINDOW_H
