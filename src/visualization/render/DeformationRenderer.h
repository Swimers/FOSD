#pragma once
#include "core/Datamodel/ModelData.h"
#include <vtkSmartPointer.h>
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
namespace fosd {
namespace visualization {
class DeformationRenderer final {
public:
    DeformationRenderer();
    ~DeformationRenderer();
    void setOriginalMesh(const vtkPolyData *mesh);
    void applyResult(const core::CalculationResultPtr &result);
    void setDeformationScale(double scale);
    void resetDeformation();
    void clear();
    vtkPolyData *displayMesh() const; // Non-owning component accessors.
    vtkPolyDataMapper *mapper() const;
    vtkActor *actor() const;

private:
    vtkSmartPointer<vtkPolyData> m_originalMesh;
    vtkSmartPointer<vtkPolyData> m_displayMesh;
    vtkSmartPointer<vtkPolyDataMapper> m_mapper;
    vtkSmartPointer<vtkActor> m_actor;
    core::CalculationResultPtr m_result;
    double m_scale = 1.0;
};
} // namespace visualization
} // namespace fosd
