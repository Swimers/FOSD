#ifndef PICKER_H
#define PICKER_H

//重载交互方式

#include"vtkInteractorStyleTrackballCamera.h"
#include"vtkTextMapper.h"
#include"vtkTextActor.h"
#include"vtkUnstructuredGrid.h"
#include"vtkDataSetMapper.h"
#include"vtkActor.h"
#include"math.h"
#include "displaymodel.h"
#include "vtkVectorText.h"

typedef double(*DisplyNodeInfo)[4];

class PointPickerInteractorStyle:public vtkInteractorStyleTrackballCamera
{
protected:
    vtkTextMapper*textmapper; //二维文本映射器
    vtkActor2D* textActor; //显示位移文本的数据
    vtkUnstructuredGrid* Grid; //显示模型的数据集
    PointPickerInteractorStyle *style;
    bool pickflag;  //拾取标记

public:
    vtkTextActor* actor;
    vtkActor* actor_sphere;
    vtkVectorText* actor_text;
    void setGrid(vtkUnstructuredGrid* Grid); //设置数据集
    vtkUnstructuredGrid* getGrid(); //获取数据
    void setpickflag(bool flag); //设置是否拾取
    displayModel* displaymodel;
    void GetDisplayModelPtr(displayModel* dd)
    {
        displaymodel = dd;
    }
    QLabel * label0;
    QLabel * label1;
    QLabel * label2;
    QLabel * label3;
private:
    double worldPoint[3];   //记录当前pick点的三维位置;更新使用
    vtkIdType OldID;
public:
    static PointPickerInteractorStyle* New();
    PointPickerInteractorStyle() //初始化类的成员变量
    {
        textmapper = nullptr;
        textActor = nullptr;
        pickflag = false;
        this->actor = vtkTextActor::New();
        this->actor_sphere = vtkActor::New();
        this->actor_text = vtkVectorText::New();
    }
    ~ PointPickerInteractorStyle()//释放类的成员指针成员
    {
        if (textmapper)
        textmapper->Delete();
        if (textActor)
        textActor -> Delete();
        if (actor_sphere)
        actor_sphere->Delete();

        this->actor->Delete();
    }
/*
    displayModel* dis;
    displayModel* getptr(displayModel* di)
    {
        dis = di;
    }
*/
    virtual void OnLeftButtonDown();//鼠标左键按下
    virtual void OnRightButtonDown();
    virtual void OnMouseMove();//鼠标移动事件
    virtual void OnKeyUp(); //键盘键弹起
    virtual void OnKeyDown();//键盘键按下
    virtual void Rotate();//鼠标旋转事件
    virtual void Spin();
    virtual void Pan();
    virtual void Dolly();
    virtual void OnMouseWheelForward();//鼠标中键向前滚动
    virtual void OnMouseWheelBackward();//鼠标中键向后滚动
    //virtual void OnMouse
    double * caculatePos(double x, double y, double z); //计算位置
    double caculatelength(double x1, double y1, double z1, double x2, double y2, double z2);//计算两点的距离
    bool findpointID(int number); //查找点的ID
    void refreshPickPointText();
    DisplyNodeInfo GetPickPointInfo();//获取拾取点信息
};

#endif // PICKER_H
