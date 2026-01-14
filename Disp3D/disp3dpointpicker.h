#ifndef DISP3DPOINTPICKER_H
#define DISP3DPOINTPICKER_H

#include "vtkVectorText.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkTextMapper.h"
#include "vtkTextActor.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "math.h"

#include "disp3dmodel.h"

typedef double(*DisplyNodeInfo)[4];

class disp3dpointpicker:public vtkInteractorStyleTrackballCamera
{
protected:
    disp3dpointpicker();
    ~disp3dpointpicker() override;

public:
    static disp3dpointpicker* New();
    vtkTypeMacro(disp3dpointpicker, vtkInteractorStyleTrackballCamera)
    void GetDisplayModelPtr(disp3dmodel* ptr){displaymodel = ptr;}

protected:
    disp3dpointpicker *style;
    bool pickflag;              //拾取标记

private:
    double    worldPoint[3];   //记录当前pick点的三维位置;更新使用
    vtkIdType OldID;

public:
    void refreshPickPointText();

public:
    disp3dmodel  * displaymodel;
    vtkTextActor * actor;
    vtkActor     * actor_sphere;

    QLabel * label0;
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;

};

#endif // DISP3DPOINTPICKER_H
