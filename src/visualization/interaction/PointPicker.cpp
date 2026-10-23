#include "PointPicker.h"
#include <vtkPointPicker.h>
#include <vtkPointLocator.h>
namespace fosd { namespace visualization {
PointPicker::PointPicker(QObject *parent) : QObject(parent) {}
PointPicker::~PointPicker() = default;
} }
