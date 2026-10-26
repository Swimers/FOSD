#pragma once
#include "visualization/VisualizationTypes.h"
#include <vtkSmartPointer.h>
class vtkActor;
class vtkSphereSource;
class vtkBillboardTextActor3D;
namespace fosd {
namespace visualization {
struct KeyPointActors {
    vtkIdType pointId = -1;
    vtkSmartPointer<vtkSphereSource> source;
    vtkSmartPointer<vtkActor> sphere;
    vtkSmartPointer<vtkBillboardTextActor3D> label;
};
// Creates/configures props only. Renderer ownership belongs to the manager.
class KeyPointCreator final {
public:
    static bool validateStyle(const KeyPointStyle &style, QString *error = nullptr);
    static KeyPointActors create(vtkIdType id, const std::array<double, 3> &position,
                                 const QString &label, const KeyPointStyle &style);
    static void configure(KeyPointActors &actors, const KeyPointStyle &style);
    static void setPosition(KeyPointActors &actors, const std::array<double, 3> &position,
                            const KeyPointStyle &style);
};
} // namespace visualization
} // namespace fosd
