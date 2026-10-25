#include "ModelInteractorStyle.h"
#include "PointPicker.h"
#include <vtkObjectFactory.h>
namespace fosd { 
namespace visualization {

vtkStandardNewMacro(ModelInteractorStyle);
ModelInteractorStyle::ModelInteractorStyle() = default;
ModelInteractorStyle::~ModelInteractorStyle() = default;

void ModelInteractorStyle::setPointPicker(PointPicker *picker)
{
    m_picker = picker;
}

void ModelInteractorStyle::setCameraLocked(bool locked)
{
    m_cameraLocked = locked;
}

void ModelInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void ModelInteractorStyle::OnLeftButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void ModelInteractorStyle::OnLeftButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void ModelInteractorStyle::OnMiddleButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void ModelInteractorStyle::OnMiddleButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}

void ModelInteractorStyle::OnRightButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void ModelInteractorStyle::OnRightButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void ModelInteractorStyle::OnMouseWheelForward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
}

void ModelInteractorStyle::OnMouseWheelBackward()
{
    vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
}

void ModelInteractorStyle::OnKeyDown()
{
    vtkInteractorStyleTrackballCamera::OnKeyDown();
}

void ModelInteractorStyle::OnKeyUp()
{
    vtkInteractorStyleTrackballCamera::OnKeyUp();
}

void ModelInteractorStyle::OnChar()
{
    vtkInteractorStyleTrackballCamera::OnChar();
}

void ModelInteractorStyle::Rotate()
{
    vtkInteractorStyleTrackballCamera::Rotate();
}

void ModelInteractorStyle::Spin()
{
    vtkInteractorStyleTrackballCamera::Spin();
}

void ModelInteractorStyle::Pan()
{
    vtkInteractorStyleTrackballCamera::Pan();
}

void ModelInteractorStyle::Dolly()
{
    vtkInteractorStyleTrackballCamera::Dolly();
}

}
}
