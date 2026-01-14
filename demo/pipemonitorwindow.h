#ifndef PIPEMONITORWINDOW_H
#define PIPEMONITORWINDOW_H

#include <QWidget>

class ReceDateThread;
namespace Ui {
class PipeMonitorWindow;
}

class PipeMonitorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PipeMonitorWindow(QWidget *parent = nullptr);
    ~PipeMonitorWindow();
    void GetReceDateThreadPtr(ReceDateThread* thread_temp){thread = thread_temp;}

private slots:
    void on_btnClose_clicked();
    void on_btnState_clicked();

private:
    Ui::PipeMonitorWindow *ui;
    ReceDateThread* thread=nullptr;
    bool isRunning = false;
};

#endif // PIPEMONITORWINDOW_H
