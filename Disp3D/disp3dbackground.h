#ifndef DISP3DBACKGROUND_H
#define DISP3DBACKGROUND_H

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkAxesActor.h>

class disp3dbackground
{
public:
    disp3dbackground(vtkRenderer* renderer,vtkSmartPointer<vtkRenderWindow> renderWindow);
    ~disp3dbackground();

public:
    bool SetVtkRenderBackColor(double r,double g,double b);                                         //设置背景颜色
    bool SetVtkRenderBackGradient(double r1,double g1,double b1,double r2,double g2,double b2);     //设置渐变类型背景颜色

    void AddAxesActor();
    void ShowAxes(bool flag);
    void SetAxesLength(int size);

    void AddAxesGrid();
    void ShowAxesGrid(bool flag);
    void SetAxesGridSize(int size);

private:
    vtkRenderer* m_renderer = nullptr;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow = nullptr;    //声明窗口指针

    vtkAxesActor* axe = nullptr;
    vtkSmartPointer<vtkActor> axesGridActor = vtkSmartPointer<vtkActor>::New(); //网格指针
    double m_gridRange = 3000.0;
    int m_gridNum = 100;
};

#endif // DISP3DBACKGROUND_H
