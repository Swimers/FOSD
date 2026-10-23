#include "PointPicker.h"
#include <vtkPointPicker.h>
#include <vtkPointLocator.h>
namespace fosd { namespace visualization {
PointPicker::PointPicker(QObject *parent) : QObject(parent) {}
PointPicker::~PointPicker() = default;

void PointPicker::setRenderer(vtkRenderer *renderer)
{
    m_renderer = renderer;
}

} }
