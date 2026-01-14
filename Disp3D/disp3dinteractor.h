#ifndef DISP3DINTERACTOR_H
#define DISP3DINTERACTOR_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTextMapper.h>
#include <vtkTextActor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkInteractorObserver.h>
#include <math.h>

#include "disp3dmodel.h"
#include "vtkVectorText.h"

class disp3dinteractor: public vtkInteractorStyleTrackballCamera
{
public:
    disp3dinteractor();
    ~disp3dinteractor();

    static disp3dinteractor* New(); //内存管理
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}

public: //重定向交互功能
    virtual void OnLeftButtonDown();    //鼠标左键按下
    virtual void OnRightButtonDown();   //鼠标右键按下
    virtual void OnRightButtonUp();     //鼠标右键释放
    virtual void OnMouseMove();         //鼠标移动事件

    virtual void OnKeyUp();             //键盘键弹起
    virtual void OnKeyDown();           //键盘键按下
    virtual void Rotate();              //鼠标旋转事件
    virtual void Spin();                //鼠标惯性旋转
    virtual void Pan();                 //相机平移
    virtual void Dolly();               //相机拉伸
    virtual void OnMouseWheelForward(); //鼠标中键向前滚动
    virtual void OnMouseWheelBackward();//鼠标中键向后滚动
    virtual void OnKeyPress();          //
    virtual void OnKeyRelease();        //

public: //函数
    void RefreshText();
    void ClearPicker();
    void RefreshSelectedPoints();

public: //变量
    bool Flag_RightButtonPressed = false;
    bool Flag_Pick;

    int LastX = 0, LastY = 0;
    double scale = 0.5; // 控制平移灵敏度，可调节

    QLabel * lab_KeyPointID;
    QLabel * lab_KeyPointRawPos;
    QLabel * lab_KeyPointDisplace;
    QLabel * lab_KeyPointNewPos;

    vtkTextActor* m_TextActor;
    vtkActor* m_SphereActor;

    vtkIdType OldID;

private:
    disp3dinteractor* style;

    disp3dmodel* m_disp3dmodel;

};

#endif // DISP3DINTERACTOR_H
