#ifndef TRACKINGPOINT_H
#define TRACKINGPOINT_H

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkRenderWindow.h>

#include <disp3dmodel.h>

struct TrackingPoint {
    uint id;                        // 点ID
    QString name;                  // 点名称
    double originalPos[3];         // 初始坐标 (毫米)
    double currentDisp[3];         // 当前变形量 (毫米)
    vtkActor* sphereActor;         // 球体Actor
    vtkCaptionActor2D* textActor;  // 文本标签Actor（自动跟随3D位置）
    bool visible;                  // 球体是否可见
    bool textVisible;              // 文本标签是否可见
};

class trackingpoint
{
public:
    trackingpoint(vtkSmartPointer<vtkRenderer> renderer,vtkSmartPointer<vtkRenderWindow> renderWindow);
    ~trackingpoint();

public:
    void SetDisplayModel(disp3dmodel * rendisp3dmodel){m_disp3dmodel = rendisp3dmodel;}

public:
    bool LoadTrackingPointsFromFile(const QString &filePath);

    vtkActor *CreateSphereSource(double x, double y, double z);
    vtkCaptionActor2D *CreateTextActor(double x, double y, double z);

    void CreatTrackingPoint(uint id, const QString& name, double x, double y, double z);
    void ResetTrackingPoint();
    void DeleteTrackingPoint();
    void ClearTrackingPoint();

    TrackingPoint *GetTrackingPointById(uint id);

    void AbleTrackingPoint(uint id, bool visible);
    void AbleAllTrackingPoint(bool visible);

    void SetPosition();
    void SetColor();
    void SetSize();

public:
    QList<TrackingPoint*> trackingPoints;  // 追踪点列表

private:
    disp3dmodel *m_disp3dmodel = nullptr;

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderwindow;
};

#endif // TRACKINGPOINT_H
