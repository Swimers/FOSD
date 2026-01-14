#ifndef MODELCALCULATIONTHREAD_H
#define MODELCALCULATIONTHREAD_H

#include "IFE.h"
#include "displaymodel.h"
#include <QThread>
#include <QLabel>

class ModelCalculationThread: public QThread
{
public:
    explicit ModelCalculationThread(QObject *parent);

// 启动线程
    void startThread() {
        isPaused = false;
        start();
    }

    // 暂停线程
    void pauseThread() {
        isPaused = true;
    }


    void GetPtr(displayModel* externdisplaymodel,IFE* externife,QLabel* externlabel)
    {
        displaymodel=externdisplaymodel;
        ife=externife;
        this->label = externlabel;
    }


protected:
    void run() override;

private:
    displayModel* displaymodel;
    IFE* ife;
    QLabel* label;
private:
    bool isPaused;
};

#endif // MODELCALCULATIONTHREAD_H
