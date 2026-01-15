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
    if (m_renderer) {
        if (axe) {
            m_renderer->RemoveActor(axe);
            axe->Delete();
            axe = nullptr;
        }
        if (axesGridActor_XZ) {
            m_renderer->RemoveActor(axesGridActor_XZ);
        }
        if (axesGridActor_XY) {
            m_renderer->RemoveActor(axesGridActor_XY);
        }
        if (axesGridActor_YZ) {
            m_renderer->RemoveActor(axesGridActor_YZ);
        }
    }

    axesGridActor_XZ = nullptr;
    axesGridActor_XY = nullptr;
    axesGridActor_YZ = nullptr;

    m_renderer = nullptr;
    m_renderWindow = nullptr;
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

vtkSmartPointer<vtkActor> disp3dbackground::CreateGridActor(PlaneType type)
{
    double step = m_gridRange / m_gridNum;

    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();

    for (int i = -m_gridNum; i <= m_gridNum; ++i){
        double v = i * step;

        vtkSmartPointer<vtkLineSource> line1 = vtkSmartPointer<vtkLineSource>::New();
        vtkSmartPointer<vtkLineSource> line2 = vtkSmartPointer<vtkLineSource>::New();

        switch (type)
        {
        case PLANE_XZ: // Y = 0
            // 平行 X
            line1->SetPoint1(-m_gridRange, 0.0, v);
            line1->SetPoint2( m_gridRange, 0.0, v);
            // 平行 Z
            line2->SetPoint1(v, 0.0, -m_gridRange);
            line2->SetPoint2(v, 0.0,  m_gridRange);
            break;

        case PLANE_XY: // Z = 0
            // 平行 X
            line1->SetPoint1(-m_gridRange, v, 0.0);
            line1->SetPoint2( m_gridRange, v, 0.0);
            // 平行 Y
            line2->SetPoint1(v, -m_gridRange, 0.0);
            line2->SetPoint2(v,  m_gridRange, 0.0);
            break;

        case PLANE_YZ: // X = 0
            // 平行 Y
            line1->SetPoint1(0.0, -m_gridRange, v);
            line1->SetPoint2(0.0,  m_gridRange, v);
            // 平行 Z
            line2->SetPoint1(0.0, v, -m_gridRange);
            line2->SetPoint2(0.0, v,  m_gridRange);
            break;
        }

        appendFilter->AddInputConnection(line1->GetOutputPort());
        appendFilter->AddInputConnection(line2->GetOutputPort());
    }

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(appendFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(0.7, 0.7, 0.7);
    actor->GetProperty()->SetOpacity(0.5);
    actor->GetProperty()->SetLineWidth(1.0);

    return actor;
}


void disp3dbackground::AddAxesGrid()
{
    axesGridActor_XZ = CreateGridActor(PLANE_XZ);
    axesGridActor_XY = CreateGridActor(PLANE_XY);
    axesGridActor_YZ = CreateGridActor(PLANE_YZ);

    m_renderer->AddActor(axesGridActor_XZ);
    m_renderer->AddActor(axesGridActor_XY);
    m_renderer->AddActor(axesGridActor_YZ);

    axesGridActor_XZ->SetVisibility(false);
    axesGridActor_XY->SetVisibility(false);
    axesGridActor_YZ->SetVisibility(false);

    m_renderWindow->Render();
}

void disp3dbackground::SetAxesGridSize(int size)
{
    if (size <= 0)
            return;
    // remove old actors
    if (axesGridActor_XZ) m_renderer->RemoveActor(axesGridActor_XZ);
    if (axesGridActor_XY) m_renderer->RemoveActor(axesGridActor_XY);
    if (axesGridActor_YZ) m_renderer->RemoveActor(axesGridActor_YZ);

    m_gridRange = 3000 * size / 20;
    m_gridNum   = 100  * size / 20;

    axesGridActor_XZ = CreateGridActor(PLANE_XZ);
    axesGridActor_XY = CreateGridActor(PLANE_XY);
    axesGridActor_YZ = CreateGridActor(PLANE_YZ);

    m_renderer->AddActor(axesGridActor_XZ);
    m_renderer->AddActor(axesGridActor_XY);
    m_renderer->AddActor(axesGridActor_YZ);

    axesGridActor_XZ->SetVisibility(false);
    axesGridActor_XY->SetVisibility(false);
    axesGridActor_YZ->SetVisibility(false);

    m_renderWindow->Render();
}

void disp3dbackground::ShowGridXZ(bool flag)
{
    axesGridActor_XZ->SetVisibility(flag);
    m_renderWindow->Render();
}

void disp3dbackground::ShowGridXY(bool flag)
{
    axesGridActor_XY->SetVisibility(flag);
    m_renderWindow->Render();
}

void disp3dbackground::ShowGridYZ(bool flag)
{
    axesGridActor_YZ->SetVisibility(flag);
    m_renderWindow->Render();
}




