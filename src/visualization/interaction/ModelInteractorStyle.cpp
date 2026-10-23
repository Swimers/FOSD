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

}
}
