#ifndef CONNECTDEBUG_H
#define CONNECTDEBUG_H

#include <QWidget>

class ReceDateThread;
namespace Ui {
class connectdebug;
}

class connectdebug : public QWidget
{
    Q_OBJECT

public:
    explicit connectdebug(QWidget *parent = nullptr);
    ~connectdebug();

    void GetReceDateThreadPtr(ReceDateThread* thread_temp){thread = thread_temp;}

private slots:
    void on_pbtn_ConnectDebugStart_clicked();
    void on_pbtn_ConnectDebugClose_clicked();

private:
    Ui::connectdebug *ui;

    ReceDateThread* thread = nullptr;
    bool isRunning = false;
};

#endif // connectdebug_H
