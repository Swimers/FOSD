#include "trackingpoint.h"

#include <QFile>
#include <QDebug>

#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>

trackingpoint::trackingpoint(vtkSmartPointer<vtkRenderer> renderer, vtkSmartPointer<vtkRenderWindow> renderWindow)
{
    m_renderer = renderer;
    m_renderwindow = renderWindow;
}

trackingpoint::~trackingpoint()
{

}

bool trackingpoint::LoadTrackingPointsFromFile(const QString &filePath)
{
    qDebug() << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开追踪点文件:" << filePath;
        return false;
    }

    ClearTrackingPoint();

    QTextStream in(&file);
    int lineNum = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineNum++;

        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        QStringList parts = line.split(",");
        if (parts.size() < 5) {
            qDebug() << "第" << lineNum << "行格式错误，跳过:" << line;
            continue;
        }

        bool ok;
        uint id = parts[0].trimmed().toUInt(&ok);
        if (!ok) {
            qDebug() << "第" << lineNum << "行ID格式错误，跳过:" << line;
            continue;
        }

        QString name = parts[1].trimmed();
        double x = parts[2].trimmed().toDouble(&ok);
        if (!ok)
            continue;
        double y = parts[3].trimmed().toDouble(&ok);
        if (!ok)
            continue;
        double z = parts[4].trimmed().toDouble(&ok);
        if (!ok)
            continue;

        CreatTrackingPoint(id, name, x, y, z);
    }

    file.close();
    qDebug() << "成功加载" << trackingPoints.size() << "个追踪点";
    return true;
}

vtkActor *trackingpoint::CreateSphereSource(double x, double y, double z)
{
    vtkSphereSource *sphereSource = vtkSphereSource::New();             // 创建vtkSphereSource对象
    sphereSource->SetThetaResolution(4);                                // 经度分段（越小面数越少）
    sphereSource->SetPhiResolution(4);                                  // 纬度分段（越小面数越少）
    sphereSource->Update();                                             // 请求数据
    vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();         // 创建映射器对象
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());    // 输入vtkSphereSource对象数据
    vtkActor *sphereActor = vtkActor::New();                            // 创建小球对象
    sphereActor->SetMapper(sphereMapper);
    sphereActor->SetPosition(x, y, z);                                  // 设置小球的位置为世界坐标
    sphereActor->GetProperty()->SetColor(0, 1, 0);                      // 设置小球对象为红色
    sphereActor->SetScale(m_disp3dmodel->ModelSize / 2);                    // 设置小球的大小
    return sphereActor;                                                 // 返回红色小球对象
}

vtkCaptionActor2D *trackingpoint::CreateTextActor(double x, double y, double z)
{
    // 创建文本标签显示变形量（使用vtkCaptionActor2D自动跟随3D位置）
    vtkCaptionActor2D *textActor = vtkCaptionActor2D::New();
    textActor->SetCaption("(0.000, 0.000, 0.000)");
    textActor->SetAttachmentPoint(x, y, z);  // 附着点为3D世界坐标
    textActor->BorderOff();                                             // 不显示边框
    textActor->LeaderOff();                                             // 不显示指示线
    textActor->SetHeight(0.02);                                         // 文本高度占视口的2%（调小这个值可以让字体更小）
    textActor->SetWidth(0.15);                                          // 文本宽度占视口的15%

    // 设置文本属性
    vtkTextProperty *textProp = textActor->GetCaptionTextProperty();
    textProp->SetFontSize(10);           // 字体大小（配合Height使用）
    textProp->SetColor(0, 1, 0);         // 文字颜色：红色 (R, G, B)
    textProp->SetBold(0);                // 是否粗体：0=正常，1=粗体
    textProp->ShadowOff();               // 关闭阴影
    textProp->SetOpacity(0.9);           // 透明度：0.0-1.0
    textProp->SetFontFamilyToArial();    // 字体：Arial
    textProp->SetJustificationToLeft();  // 左对齐

    return textActor;
}

void trackingpoint::CreatTrackingPoint(uint id, const QString &name, double x, double y, double z)
{
    if (GetTrackingPointById(id) != nullptr) {
        qDebug() << "追踪点ID" << id << "已存在，跳过添加";
        return;
    }

    TrackingPoint *tp = new TrackingPoint;
    tp->id = id;
    tp->name = name;
    tp->originalPos[0] = x;
    tp->originalPos[1] = y;
    tp->originalPos[2] = z;
    tp->currentDisp[0] = 0.0;
    tp->currentDisp[1] = 0.0;
    tp->currentDisp[2] = 0.0;
    tp->visible = true;
    tp->textVisible = false;  // 默认隐藏文本标签

    // 创建球体
    tp->sphereActor = CreateSphereSource(x,y,z);
    tp->textActor = CreateTextActor(x,y,z);

    m_renderer->AddActor(tp->sphereActor);
    m_renderer->AddActor2D(tp->textActor);
    tp->sphereActor->SetVisibility(tp->visible ? 1 : 0);
    tp->textActor->SetVisibility(tp->textVisible ? 1 : 0);  // 使用textVisible字段控制文本标签显示
    trackingPoints.append(tp);

    m_renderwindow->Render();
    qDebug() << "添加追踪点:" << id << name << "坐标:(" << x << "," << y << "," << z << ")";
}

void trackingpoint::ResetTrackingPoint()
{
    for (TrackingPoint *tp : trackingPoints) {
        tp->currentDisp[0] = 0.0;
        tp->currentDisp[1] = 0.0;
        tp->currentDisp[2] = 0.0;

        if (tp->visible) {
            // 更新球体位置
            tp->sphereActor->SetPosition(tp->originalPos);
            tp->sphereActor->Modified();
            // 更新文本标签内容和3D附着点位置
            char dispText[100];
            sprintf(dispText, "(%.3f, %.3f, %.3f)", tp->currentDisp[0], tp->currentDisp[1], tp->currentDisp[2]);
            tp->textActor->SetCaption(dispText);
            tp->textActor->SetAttachmentPoint(tp->originalPos);
            tp->textActor->Modified();
        }
    }
}

void trackingpoint::ClearTrackingPoint()
{
    for (TrackingPoint *tp : trackingPoints) {
        m_renderer->RemoveActor(tp->sphereActor);
        m_renderer->RemoveActor2D(tp->textActor);
        tp->sphereActor->Delete();
        tp->textActor->Delete();
        delete tp;
    }
    trackingPoints.clear();
    m_renderwindow->Render();
    qDebug() << "清空所有追踪点";
}

TrackingPoint *trackingpoint::GetTrackingPointById(uint id)
{
    for (TrackingPoint *tp : trackingPoints) {
        if (tp->id == id) {
            return tp;
        }
    }
    return nullptr;
}

void trackingpoint::AbleTrackingPoint(uint id, bool visible)
{
    TrackingPoint *tp = GetTrackingPointById(id);
    if (tp != nullptr) {
        tp->visible = visible;
        tp->sphereActor->SetVisibility(visible ? 1 : 0);
        m_renderwindow->Render();
    }
}

void trackingpoint::AbleAllTrackingPoint(bool visible)
{
    for (TrackingPoint *tp : trackingPoints) {
        tp->visible = visible;
        tp->sphereActor->SetVisibility(visible ? 1 : 0);
    }
    m_renderwindow->Render();
}
