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
#include <vtkPlaneSource.h>
#include <vtkJPEGReader.h>
#include <vtkPNGReader.h>
#include <vtkTexture.h>
#include <vtkCamera.h>
#include <vtkSphereSource.h>
#include <vtkTextureMapToSphere.h>

disp3dbackground::disp3dbackground(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkRenderWindow> renderWindow)
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
    m_renderWindow->Render();
    axe->SetVisibility(flag);           //true为可见，false为不可见
    SetAxesLength(50);
    m_renderWindow->Render();
}

void disp3dbackground::SetAxesLength(int size)
{
    double normalized = size / 100.0;
    double scaleFactor = 50000.0;
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
    actor->GetProperty()->SetOpacity(0.1);
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

void disp3dbackground::CreateWorld()
{
    vtkActor* ground = CreateGroundWithTexture(200);
    m_renderer->AddActor(ground);
    vtkActor* sky = CreateSkyDome(700);
    m_renderer->AddActor(sky);

    vtkCamera* camera = m_renderer->GetActiveCamera();
    double focalPoint[3] = {0.0, 0.0, 0.0};
    double viewDir[3] = {0.0, -1.0, 0.3};  // 带一点仰角更像飞行视角
    double len = std::sqrt(viewDir[0]*viewDir[0] +
                           viewDir[1]*viewDir[1] +
                           viewDir[2]*viewDir[2]);
    viewDir[0] /= len;
    viewDir[1] /= len;
    viewDir[2] /= len;
    double distance = 20000.0;  // 根据你的飞机尺度调整（100~500常用）
    double position[3];
    position[0] = focalPoint[0] - viewDir[0] * distance;
    position[1] = focalPoint[1] - viewDir[1] * distance;
    position[2] = focalPoint[2] - viewDir[2] * distance;
    camera->SetPosition(position);       // 动态位置（非固定）
    m_renderWindow->Render();
}

vtkActor *disp3dbackground::CreateGround(double modelSize)
{
    vtkPlaneSource* plane = vtkPlaneSource::New();

    double scale = modelSize * 50.0;

    plane->SetOrigin(-scale, -scale, 0);
    plane->SetPoint1(scale, -scale, 0);
    plane->SetPoint2(-scale, scale, 0);
    plane->Update();

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(plane->GetOutputPort());

    vtkActor* groundActor = vtkActor::New();
    groundActor->SetMapper(mapper);
    groundActor->GetProperty()->SetColor(0.3, 0.6, 0.3); // 草绿色
    groundActor->GetProperty()->SetAmbient(0.8); // 防止太暗
    groundActor->GetProperty()->LightingOff();   // 更像背景地面
    groundActor->SetPosition(0, -modelSize * 30.0, -modelSize * 2.0);

    return groundActor;
}

vtkActor *disp3dbackground::CreateSkyPlane(double modelSize)
{
    vtkPlaneSource* plane = vtkPlaneSource::New();

    double scale = modelSize * 80.0;

    plane->SetOrigin(-scale, 0, -scale);
    plane->SetPoint1(scale, 0, -scale);
    plane->SetPoint2(-scale, 0, scale);
    plane->Update();

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(plane->GetOutputPort());
    vtkActor* skyActor = vtkActor::New();
    skyActor->SetMapper(mapper);
    skyActor->GetProperty()->SetColor(0.6, 0.85, 1.0);
    skyActor->GetProperty()->LightingOff();
    skyActor->GetProperty()->SetAmbient(1.0);
    skyActor->SetPosition(0, modelSize * 50.0, 0);

    return skyActor;
}

vtkActor *disp3dbackground::CreateGroundWithTexture(double modelSize)
{
    vtkPlaneSource* plane = vtkPlaneSource::New();

    double scale = modelSize * 5000.0;
    double groundY = -modelSize * 500; // 地面位于 Y 负方向远处

    // ===== 关键修改：固定Y，变化X和Z（XOZ平面）=====
    plane->SetOrigin(-scale, groundY, -scale);   // 左下
    plane->SetPoint1(scale,  groundY, -scale);   // 右下（X方向）
    plane->SetPoint2(-scale, groundY,  scale);   // 左上（Z方向）
    plane->Update();

    // ===== 读取贴图 =====
    vtkJPEGReader* reader = vtkJPEGReader::New();
    reader->SetFileName("./data/background/ground.jpg");  // 草地/跑道图片
    reader->Update();

    vtkTexture* texture = vtkTexture::New();
    texture->SetInputConnection(reader->GetOutputPort());

    texture->InterpolateOn();     // 平滑
    texture->RepeatOn();          // 允许重复贴图（关键）

    // ===== 映射器 =====
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(plane->GetOutputPort());

    vtkActor* groundActor = vtkActor::New();
    groundActor->SetMapper(mapper);
    groundActor->SetTexture(texture);

    // 关闭光照，避免地面发黑
    groundActor->GetProperty()->LightingOff();

    // 放到 Y 负方向远处
    groundActor->SetPosition(0, -modelSize * 40.0, -modelSize * 2.0);

    return groundActor;
}

vtkActor *disp3dbackground::CreateSkyPlaneWithTexture(double modelSize)
{
    vtkPlaneSource* plane = vtkPlaneSource::New();

    double scale = modelSize * 5000.0;
    double groundY = modelSize * 500; // 地面位于 Y 负方向远处

    // ===== 关键修改：固定Y，变化X和Z（XOZ平面）=====
    plane->SetOrigin(-scale, groundY, -scale);   // 左下
    plane->SetPoint1(scale,  groundY, -scale);   // 右下（X方向）
    plane->SetPoint2(-scale, groundY,  scale);   // 左上（Z方向）
    plane->Update();

    vtkJPEGReader* reader = vtkJPEGReader::New();
    reader->SetFileName("./data/background/sky.jpg");
    reader->Update();

    vtkTexture* texture = vtkTexture::New();
    texture->SetInputConnection(reader->GetOutputPort());
    texture->InterpolateOn();

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(plane->GetOutputPort());

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    actor->SetTexture(texture);
    actor->GetProperty()->LightingOff();

    actor->SetPosition(0, modelSize * 60.0, 0);

    return actor;
}

vtkActor *disp3dbackground::CreateSkyDome(double modelSize)
{
    // 1. 创建球体（天空穹顶）
    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetRadius(modelSize * 2000.0);
    sphere->SetThetaResolution(32);
    sphere->SetPhiResolution(32);
    sphere->SetCenter(0.0, -600000, 0.0);
    sphere->Update();

    // 2. 生成球面纹理坐标（关键步骤！！！）
    vtkTextureMapToSphere* textureMap = vtkTextureMapToSphere::New();
    textureMap->SetInputConnection(sphere->GetOutputPort());
    textureMap->PreventSeamOn();  // 防止贴图接缝
    textureMap->Update();

    // 3. 读取天空贴图
    vtkJPEGReader* reader = vtkJPEGReader::New();
    reader->SetFileName("./data/background/sky.jpg");
    reader->Update();

    // 4. 创建纹理对象
    vtkTexture* texture = vtkTexture::New();
    texture->SetInputConnection(reader->GetOutputPort());
    texture->InterpolateOn();          // 平滑贴图
    texture->RepeatOff();              // 不重复贴图

    // 5. 映射器（注意输入改为textureMap）
    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(textureMap->GetOutputPort());

    // 6. 创建天空Actor
    vtkActor* skyActor = vtkActor::New();
    skyActor->SetMapper(mapper);
    skyActor->SetTexture(texture);

    // 7. 关键：天空球内部可见（你这点是对的）
    skyActor->GetProperty()->BackfaceCullingOff();
    skyActor->GetProperty()->FrontfaceCullingOn();
    skyActor->GetProperty()->LightingOff();  // 天空不受光照影响

    return skyActor;
}


