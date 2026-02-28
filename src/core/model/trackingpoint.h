#ifndef TRACKINGPOINT_H
#define TRACKINGPOINT_H

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>

#include <disp3dmodel.h>

class trackingpoint
{
public:
    trackingpoint(vtkSmartPointer<vtkRenderer> renderer,vtkSmartPointer<vtkRenderWindow> renderWindow);
    ~trackingpoint();

public:
    void SetDisplayModel(disp3dmodel * rendisp3dmodel){m_disp3dmodel = rendisp3dmodel;}

public:
    void CreatTrackingPoint();
    void DeleteTrackingPoint();

    void AbleTrackingPoint();
    void DisableTrackingPoint();

    void SetPosition();

    void SetColor();
    void SetSize();

private:
    disp3dmodel *m_disp3dmodel = nullptr;

    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderwindow;

};

#endif // TRACKINGPOINT_H
