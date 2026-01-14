#include "renderdermanager.h"


#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include <QDebug>
#include "vtkLight.h"
#include "vtkCamera.h"
RenderderManager::RenderderManager()
{

}

/*
 * @功能：窗体布局设置
 *       ----------------               ------------------              --------------------            --------------------
 *       |              |               |        |       |              |                  |            |         |         |
 *       |              |               |        |       |              |        3         |            |   3     |   4     |
 *       |              |               |        |       |              |                  |            |         |         |
 *       |       1      |               |   1    |   2   |              |------------------|            |-------------------|
 *       |              |               |        |       |              |         |        |            |         |         |
 *       |              |               |        |       |              |    1    |    2   |            |    1    |   2     |
 *       |              |               |        |       |              |         |        |            |         |         |
 *       ---------------                -----------------               --------------------            ---------------------
 * @参数：num -- 1，2，3，4
 * @返回：布局完成，则返回true，否则返回false
 */
bool RenderderManager::SetvtkRenderNumAndLayout(const int num)
{
    if(0<num&&num<=4)
    {
        vtkSmartPointer<vtkRendererCollection> renderers =
                renderWindow->GetRenderers();
        renderers->RemoveAllItems();
        switch(num){
        case 1:              //渲染器,最多四个小窗口，一般情况下只使用一个,可以用数组写，但我懒
            {
                vtkSmartPointer<vtkRenderer>renderer1= vtkSmartPointer<vtkRenderer>::New();
                //int i=0;
                renderWindow->AddRenderer(renderer1);
                renderer1->SetBackground(0.75,0.9,0.96);             //设置页面底部颜色值
                renderer1->SetViewport(0,0,1,1);                     //设置视窗位置
                break;
            }
        case 2:
            {
                vtkSmartPointer<vtkRenderer>renderer1= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer2= vtkSmartPointer<vtkRenderer>::New();
                //qDebug()<<"renderer1"<<renderer1<<"renderer2"<<renderer2;
                renderer1->SetBackground(0.75,0.9,0.96);             //设置页面底部颜色值
                renderer2->SetBackground(0.2, 0.4, 0.6);
                //renderer1->SetTexturedBackground(1);
                renderer1->SetViewport(0,0,0.5,1);
                renderer2->SetViewport(0.5,0,1,1);
                renderWindow->AddRenderer(renderer1);
                renderWindow->AddRenderer(renderer2);
                break;
            }
        case 3:
            {
                vtkSmartPointer<vtkRenderer>renderer1= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer2= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer3= vtkSmartPointer<vtkRenderer>::New();
                renderWindow->AddRenderer(renderer1);
                renderWindow->AddRenderer(renderer2);
                renderWindow->AddRenderer(renderer3);
                renderer1->SetBackground(0.75,0.9,0.96);             //设置页面底部颜色值
                renderer2->SetBackground(0.2, 0.4, 0.6);
                renderer3->SetBackground(0, 0, 1);
                renderer1->SetViewport(0,0,0.5,0.5);
                renderer2->SetViewport(0.5,0,1,0.5);
                renderer3->SetViewport(0,0.5,1,1);
                break;
            }
        case 4:
            {
                vtkSmartPointer<vtkRenderer>renderer1= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer2= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer3= vtkSmartPointer<vtkRenderer>::New();
                vtkSmartPointer<vtkRenderer>renderer4= vtkSmartPointer<vtkRenderer>::New();
                renderWindow->AddRenderer(renderer1);
                renderWindow->AddRenderer(renderer2);
                renderWindow->AddRenderer(renderer3);
                renderWindow->AddRenderer(renderer4);
                renderer1->SetBackground(0.75,0.9,0.96);             //设置页面底部颜色值
                renderer2->SetBackground(0.2, 0.4, 0.6);
                renderer3->SetBackground(0,0, 1);
                renderer4->SetBackground(0,1, 0);
                renderer1->SetViewport(0,0,0.5,0.5);
                renderer2->SetViewport(0.5,0,1,0.5);
                renderer3->SetViewport(0,0.5,0.5,1);
                renderer4->SetViewport(0.5,0.5,1,1);
                break;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}
/*
 * @功能：渲染器背景颜色设置
 * @参数：num -- 1，2，3，4且0<num<rendererNum
 *       r,g,b -- RGB颜色值[0,1]
 * @返回：布局完成，则返回true，否则返回false
 */
bool RenderderManager::SetvtkRenderBackColor(RendererIndex num, double r, double g, double b)
{
    vtkSmartPointer<vtkRendererCollection> renderers =
            renderWindow->GetRenderers();
    qDebug()<<renderers->GetItemAsObject(num)<<renderers->GetItemAsObject(num-1);
    vtkRenderer* renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(num-1));
    if(renderer)
    {
        renderer->SetBackground(r,g,b);
        renderer->SetGradientBackground(0);               //关闭渐变色背景设置
        renderWindow->Render();
        return true;
    }
    else
    {
        qDebug()<<"SetvtkRenderBackColor函数中"<<"renderer"<<num<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}
/*
 * @功能：渲染器背景颜色设置
 * @参数：num -- 1，2，3，4且0<num<rendererNum
 *       r,g,b -- RGB颜色值[0,1]
 * @返回：布局完成，则返回true，否则返回false
 */
bool RenderderManager::SetvtkRenderBackColor2(RendererIndex num, double r1, double g1, double b1, double r2, double g2, double b2)
{
    vtkSmartPointer<vtkRendererCollection> renderers =
            renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(num-1));

    if(renderer)
    {
        renderer->SetBackground(r1,g1,b1);             //设置页面底部颜色值
        renderer->SetBackground2(r2, g2, b2);          //设置页面顶部颜色值
        renderer->SetGradientBackground(1);            //开启渐变色背景设置
        return true;
    }
    else
    {
        qDebug()<<"SetvtkRenderBackColor2函数中"<<"renderer"<<num<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}

bool RenderderManager::AddLight(RendererIndex num)
{
    vtkSmartPointer<vtkRendererCollection> renderers =
            renderWindow->GetRenderers();
    vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::SafeDownCast(renderers->GetItemAsObject(num-1));
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetColor(0,1,0);
    light->SetPosition(20,80,0);
    light->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light);
    vtkSmartPointer<vtkLight> light2 = vtkSmartPointer<vtkLight>::New();
    light2->SetColor(1,0,0);
    light2->SetPosition(20,-80,0);
    light2->SetFocalPoint(renderer->GetActiveCamera()->GetFocalPoint());
    renderer->AddLight(light2);
    return 1;
}

void RenderderManager::test()
{


}
