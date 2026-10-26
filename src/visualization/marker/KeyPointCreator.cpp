#include "KeyPointCreator.h"
#include <vtkActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTextProperty.h>
#include <cmath>
namespace fosd {
namespace visualization {
bool KeyPointCreator::validateStyle(const KeyPointStyle &style, QString *error) {
    if (error)
        error->clear();
    bool valid = std::isfinite(style.radiusMm) && style.radiusMm > 0 && style.fontSize >= 1 &&
                 style.fontSize <= 256 && style.sphereResolution >= 4 &&
                 style.sphereResolution <= 128;
    for (int axis = 0; axis < 3; ++axis)
        valid = valid && std::isfinite(style.labelOffsetMm[axis]) &&
                std::isfinite(style.color[axis]) && style.color[axis] >= 0 &&
                style.color[axis] <= 1 && std::isfinite(style.labelColor[axis]) &&
                style.labelColor[axis] >= 0 && style.labelColor[axis] <= 1;
    if (!valid && error)
        *error =
            QStringLiteral("Invalid key point radius, RGB, label offset, font size or resolution");
    return valid;
}

KeyPointActors KeyPointCreator::create(vtkIdType id, 
                                       const std::array<double, 3> &position,
                                       const QString &label, 
                                       const KeyPointStyle &style) {
    KeyPointActors actors;
    if (id < 0 || !validateStyle(style))
        return actors;
    for (double value : position)
        if (!std::isfinite(value))
            return actors;
    
    actors.pointId = id;
    actors.source = vtkSmartPointer<vtkSphereSource>::New();
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(actors.source->GetOutputPort());
    mapper->ScalarVisibilityOff();
    actors.sphere = vtkSmartPointer<vtkActor>::New();
    actors.sphere->SetMapper(mapper);
    actors.sphere->PickableOff();
    actors.sphere->UseBoundsOff();

    actors.label = vtkSmartPointer<vtkBillboardTextActor3D>::New();
    actors.label->SetInput(
        (label.isEmpty() ? QString::number(qlonglong(id)) : label).toUtf8().constData());
    actors.label->PickableOff();
    actors.label->UseBoundsOff();

    configure(actors, style);
    setPosition(actors, position, style);

    return actors;
}

void KeyPointCreator::configure(KeyPointActors &actors, const KeyPointStyle &style) {
    if (!actors.sphere || !actors.label || !actors.source || !validateStyle(style))
        return;
    actors.source->SetRadius(style.radiusMm);
    actors.source->SetThetaResolution(style.sphereResolution);
    actors.source->SetPhiResolution(style.sphereResolution);
    actors.sphere->GetProperty()->SetColor(style.color[0], style.color[1], style.color[2]);
    actors.label->GetTextProperty()->SetColor(style.labelColor[0], style.labelColor[1],
                                              style.labelColor[2]);
    actors.label->GetTextProperty()->SetFontSize(style.fontSize);
    actors.label->SetVisibility(style.labelsVisible);
}

void KeyPointCreator::setPosition(KeyPointActors &actors, 
                                  const std::array<double, 3> &position,
                                  const KeyPointStyle &style) {
    if (!actors.sphere || !actors.label)
        return;
    actors.sphere->SetPosition(position[0], position[1], position[2]);
    actors.label->SetPosition(position[0] + style.labelOffsetMm[0],
                              position[1] + style.labelOffsetMm[1],
                              position[2] + style.labelOffsetMm[2]);
}
} // namespace visualization
} // namespace fosd
