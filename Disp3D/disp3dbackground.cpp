#include "disp3dbackground.h"

#include <QDebug>
#include <QObject>

#include <vtkRenderWindow.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>
#include <vtkArrowSource.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>

disp3dbackground::disp3dbackground(vtkRenderer *renderer, vtkSmartPointer<vtkRenderWindow> renderWindow)
{
    m_renderer =  renderer;
    m_renderWindow = renderWindow;
}

disp3dbackground::~disp3dbackground()
{

}

bool disp3dbackground::SetVtkRenderBackColor(double r, double g, double b)
{
    if(m_renderer){
        m_renderer->SetBackground(r,g,b);
        m_renderer->SetGradientBackground(0);               //关闭渐变色背景设置
        m_renderWindow->Render();
        return true;
    }
    else{
        qDebug()<<"SetvtkRenderBackColor函数中"<<"renderer"<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}

bool disp3dbackground::SetVtkRenderBackGradient(double r1, double g1, double b1, double r2, double g2, double b2)
{
    if(m_renderer){
        m_renderer->SetBackground(r1,g1,b1);             //设置页面底部颜色值
        m_renderer->SetBackground2(r2, g2, b2);          //设置页面顶部颜色值
        m_renderer->SetGradientBackground(1);            //开启渐变色背景设置
        m_renderWindow->Render();
        return true;
    }
    else{
        qDebug()<<"SetvtkRenderBackColor2函数中"<<"renderer"<<"未设置,导致设置颜色失败"<<"\n";
        return false;
    }
}

void disp3dbackground::AddAxesActor()
{
    this->axe = vtkAxesActor::New();
    axe->SetAxisLabels(1);      //设置lable是否可见
    axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    axe->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);
    axe->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);
    axe->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axe->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axe->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    //axe->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->set(70);
    axe->SetTotalLength(1,1,1);
    axe->SetTipTypeToCone();        // 默认就是 Cone
    axe->SetNormalizedShaftLength(0.95, 0.95, 0.95);  // 轴部分占比
    axe->SetNormalizedTipLength(0.05, 0.05, 0.05);    // 箭头部分占比

    axe->SetVisibility(false);           //true为可见，false为不可见
    m_renderer->AddActor(axe);
}

void disp3dbackground::ShowAxes(bool flag)
{
    axe->SetVisibility(flag);           //true为可见，false为不可见
    m_renderWindow->Render();
}

void disp3dbackground::SetAxesLength(int size)
{
    double normalized = size / 100.0;
    double scaleFactor = 10000.0;
    double actualLength = scaleFactor * pow(normalized, 2.0);
    axe->SetTotalLength(actualLength, actualLength, actualLength);
    if (m_renderWindow) {
        m_renderWindow->Render();
    }
}

void disp3dbackground::AddAxesGrid()
{
    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();
    double step = m_gridRange / m_gridNum;
    for (int i = -m_gridNum; i <= m_gridNum; ++i)
    {
        double z = i * step;
        vtkSmartPointer<vtkLineSource> lineX = vtkSmartPointer<vtkLineSource>::New();
        lineX->SetPoint1(-m_gridRange, 0.0, z);
        lineX->SetPoint2(m_gridRange, 0.0, z);
        appendFilter->AddInputConnection(lineX->GetOutputPort());
    }
    for (int i = -m_gridNum; i <= m_gridNum; ++i)
    {
        double x = i * step;
        vtkSmartPointer<vtkLineSource> lineZ = vtkSmartPointer<vtkLineSource>::New();
        lineZ->SetPoint1(x, 0.0, -m_gridRange);
        lineZ->SetPoint2(x, 0.0, m_gridRange);
        appendFilter->AddInputConnection(lineZ->GetOutputPort());
    }
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(appendFilter->GetOutputPort());
    axesGridActor->SetMapper(mapper);
    axesGridActor->GetProperty()->SetColor(0.7, 0.7, 0.7);  // 灰色线条
    axesGridActor->GetProperty()->SetOpacity(0.5);          // 半透明
    axesGridActor->GetProperty()->SetLineWidth(1.0);
    m_renderer->AddActor(axesGridActor);
    axesGridActor->SetVisibility(false);
    m_renderWindow->Render();
}

void disp3dbackground::ShowAxesGrid(bool flag)
{
    axesGridActor->SetVisibility(flag);
    m_renderWindow->Render();
}

void disp3dbackground::SetAxesGridSize(int size)
{
    if (size <= 0)
            return;
    if (axesGridActor){
        m_renderer->RemoveActor(axesGridActor);
    }
    m_gridRange = 3000 * size /20;
    m_gridNum = 100 * size /20;
    double step = m_gridRange / m_gridNum;
    m_gridNum = static_cast<int>(m_gridRange / step);
    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();
    for (int i = -m_gridNum; i <= m_gridNum; ++i){
        double z = i * step;
        vtkSmartPointer<vtkLineSource> lineX = vtkSmartPointer<vtkLineSource>::New();
        lineX->SetPoint1(-m_gridRange, 0.0, z);
        lineX->SetPoint2(m_gridRange, 0.0, z);
        appendFilter->AddInputConnection(lineX->GetOutputPort());
    }
    for (int i = -m_gridNum; i <= m_gridNum; ++i){
        double x = i * step;
        vtkSmartPointer<vtkLineSource> lineZ = vtkSmartPointer<vtkLineSource>::New();
        lineZ->SetPoint1(x, 0.0, -m_gridRange);
        lineZ->SetPoint2(x, 0.0, m_gridRange);
        appendFilter->AddInputConnection(lineZ->GetOutputPort());
    }
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(appendFilter->GetOutputPort());
    axesGridActor = vtkSmartPointer<vtkActor>::New();
    axesGridActor->SetMapper(mapper);
    axesGridActor->GetProperty()->SetColor(0.7, 0.7, 0.7);
    axesGridActor->GetProperty()->SetOpacity(0.5);
    axesGridActor->GetProperty()->SetLineWidth(1.0);
    m_renderer->AddActor(axesGridActor);
    m_renderWindow->Render();
}



