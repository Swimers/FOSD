#ifndef RENDERDERMANAGER_H
#define RENDERDERMANAGER_H


#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
class RenderderManager
{
public:
    RenderderManager();

    enum RendererIndex{
        rendererIndex1 = 1,
        rendererIndex2 = 2,
        rendererIndex3 = 3,
        rendererIndex4 = 4
    };

private:
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();    //视窗
public:
    vtkRenderWindow* GetvtkRenderWindow()       //得到窗口指针
    {
        return renderWindow;
    }

    bool SetvtkRenderNumAndLayout(const int num);        //设置vtkRender窗体的个数和布局
    bool SetvtkRenderBackColor(RendererIndex num,double r,double g,double b);   //设置背景颜色
    bool SetvtkRenderBackColor2(RendererIndex num,double r1,double g1,double b1,double r2,double g2,double b2);     //设置渐变类型背景颜色
    bool AddLight(RendererIndex num);           //没写完、懒得写，应该没啥用，添加光源
    bool ReadDataAndCreateActor();
    void test();


};

#endif // RENDERDERMANAGER_H
