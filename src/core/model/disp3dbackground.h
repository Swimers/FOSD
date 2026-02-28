#ifndef DISP3DBACKGROUND_H
#define DISP3DBACKGROUND_H

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>

enum PlaneType { PLANE_XZ, PLANE_XY, PLANE_YZ };

class disp3dbackground
{
public:
    disp3dbackground(vtkSmartPointer<vtkRenderer> renderer,vtkSmartPointer<vtkRenderWindow> renderWindow);
    ~disp3dbackground();

public:
    bool SetVtkRenderBackColor(double r,double g,double b);                                         //设置背景颜色
    bool SetVtkRenderBackGradient(double r1,double g1,double b1,double r2,double g2,double b2);     //设置渐变类型背景颜色

    void AddAxesActor();
    void ShowAxes(bool flag);
    void SetAxesLength(int size);

    vtkSmartPointer<vtkActor> CreateGridActor(PlaneType type);
    void AddAxesGrid();
    void SetAxesGridSize(int size);

    void ShowGridXZ(bool);
    void ShowGridXY(bool);
    void ShowGridYZ(bool);

private:
    vtkSmartPointer<vtkRenderer> m_renderer = nullptr;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow = nullptr;    //声明窗口指针

    vtkAxesActor* axe = nullptr;

    vtkSmartPointer<vtkActor> axesGridActor_XZ;  // 原 Y=0 平面
    vtkSmartPointer<vtkActor> axesGridActor_XY;  // Z=0 平面
    vtkSmartPointer<vtkActor> axesGridActor_YZ;  // X=0 平面

    double m_gridRange = 3000.0;
    int m_gridNum = 100;
};

#endif // DISP3DBACKGROUND_H
