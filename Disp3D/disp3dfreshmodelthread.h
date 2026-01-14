#ifndef DISP3DFRESHMODELTHREAD_H
#define DISP3DFRESHMODELTHREAD_H

#include "IFE.h"
#include "disp3dmodel.h"
#include <QThread>
#include <QLabel>

class disp3dfreshmodelthread: public QThread
{
public:
    explicit disp3dfreshmodelthread(QObject *parent);
    void startThread() {isPaused = false;start();}
    void pauseThread() {isPaused = true;}
    void GetPtr(disp3dmodel* externdisplaymodel,IFE* externife,QLabel* externlabel);

    void normalizeScalarArray(vtkSmartPointer<vtkDoubleArray> arr);

protected:
    void run() override;

private:
    disp3dmodel* displaymodel;
    IFE* ife;
    QLabel* label;

private:
    bool isPaused;
};

#endif // DISP3DFRESHMODELTHREAD_H
