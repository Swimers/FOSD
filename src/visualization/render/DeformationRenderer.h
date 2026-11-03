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
    vtkSmartPointer<vtkPolyData> m_originalMesh;    // 原始网格
    vtkSmartPointer<vtkPolyData> m_displayMesh;     // 变形网格

    vtkSmartPointer<vtkPolyDataMapper> m_mapper;    // 固定指向 displayMesh
    vtkSmartPointer<vtkActor> m_actor;              // 固定指向 m_mapper

    core::CalculationResultPtr m_result;            // 最近一次位移解算结果
    double m_scale = 1.0;
};
} // namespace visualization
} // namespace fosd
