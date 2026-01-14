#ifndef DISP3DINTERACTOR_H
#define DISP3DINTERACTOR_H

//重载交互方式

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTextMapper.h>
#include <vtkTextActor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <math.h>

#include "disp3dmodel.h"
#include "vtkVectorText.h"

typedef double(*DisplyNodeInfo)[4];

class disp3dinteractor: public vtkInteractorStyleTrackballCamera
{
public:
    disp3dinteractor();
    ~disp3dinteractor();

public:
    vtkTextActor* actor;
    vtkActor* actor_sphere;
    vtkVectorText* actor_text;

    QLabel * label0;
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;

public:
    bool Flag_Fellow;

    static disp3dinteractor* New(); //内存管理
    void GetDisplayModelPtr(disp3dmodel* ptr){m_disp3dmodel = ptr;}

    void setpickflag(bool flag);                //设置是否拾取
    void setGrid(vtkUnstructuredGrid* Grid);    //设置数据集
    vtkUnstructuredGrid* getGrid();             //获取数据
    DisplyNodeInfo GetPickPointInfo();          //获取拾取点信息

    virtual void OnLeftButtonDown();    //鼠标左键按下
    virtual void OnRightButtonDown();
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

<<<<<<< Updated upstream
public: //函数
    void RefreshText();
    void ClearPicker();
    void RefreshSelectedPoints();

public: //变量
    bool Flag_RightButtonPressed = false;
    bool Flag_Pick;

    bool Flag_CameraLock = 0;
    bool Flag_CameraFellow = 0;

    int LastX = 0, LastY = 0;
    double scale = 0.5; // 控制平移灵敏度，可调节

    QLabel * lab_KeyPointID;
    QLabel * lab_KeyPointRawPos;
    QLabel * lab_KeyPointDisplace;
    QLabel * lab_KeyPointNewPos;

    vtkTextActor* m_TextActor;
    vtkActor* m_SphereActor;

    vtkIdType OldID;
=======
    double * caculatePos(double x, double y, double z); //计算位置
    double caculatelength(double x1, double y1, double z1, double x2, double y2, double z2);//计算两点的距离
    bool findpointID(int number); //查找点的ID
    void refreshPickPointText();
>>>>>>> Stashed changes

private:
    double worldPoint[3];   //记录当前pick点的三维位置;更新使用
    vtkIdType OldID;

protected:
    disp3dinteractor *style;
    disp3dmodel * m_disp3dmodel;

    vtkTextMapper*textmapper;       //二维文本映射器
    vtkActor2D* textActor;          //显示位移文本的数据
    vtkUnstructuredGrid* Grid;      //显示模型的数据集
    bool pickflag;                  //拾取标记
    int * pickpointIDset = nullptr; //拾取点的ID
    unsigned long long pickernumber = 0;
    vtkActor **pointactor = nullptr;//拾取点的Actor数组


};

#endif // DISP3DINTERACTOR_H
