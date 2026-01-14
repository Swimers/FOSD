#ifndef DISP3DFRESHMODELTHREAD_H
#define DISP3DFRESHMODELTHREAD_H

#include <QThread>

#include <disp3dmodel.h>
#include <IFE.h>

class disp3dfreshmodelthread: public QThread
{
public:
    explicit disp3dfreshmodelthread(QObject *parent);
    void startThread() {isPaused = false; start();}
    void pauseThread() {isPaused = true;}
    void GetPtr(disp3dmodel* externdisplaymodel,IFE* externife);

protected:
    void run() override;

private:
    disp3dmodel* m_disp3dmodel;
    IFE* ife;
    QLabel* label;

private:
    bool isPaused;
};

#endif // DISP3DFRESHMODELTHREAD_H
