#include "DeformationRenderer.h"

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>

namespace fosd {
namespace visualization {
DeformationRenderer::DeformationRenderer() = default;
DeformationRenderer::~DeformationRenderer() = default;

void DeformationRenderer::setOriginalMesh(const vtkPolyData *mesh) {

    clear();
    if (!mesh)
        return;

    m_originalMesh = vtkSmartPointer<vtkPolyData>::New();
    m_originalMesh->DeepCopy(const_cast<vtkPolyData *>(mesh));
    m_displayMesh = vtkSmartPointer<vtkPolyData>::New();
    m_displayMesh->DeepCopy(m_originalMesh);

    m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_mapper->SetInputData(m_displayMesh);
    m_mapper->ScalarVisibilityOff();

    m_actor = vtkSmartPointer<vtkActor>::New();
    m_actor->SetMapper(m_mapper);
}

void DeformationRenderer::applyResult(const core::CalculationResultPtr &result) {
    m_result = result;

    if (!m_originalMesh)
        return;

    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();
    points->SetNumberOfPoints(m_originalMesh->GetNumberOfPoints());

    for (vtkIdType id = 0; id < m_originalMesh->GetNumberOfPoints(); ++id) {
        double xyz[3];
        m_originalMesh->GetPoint(id, xyz);
        if (result) {
            const auto delta = result->displacementMap.value(id);
            if (delta.size() == 3)
                for (int axis = 0; axis < 3; ++axis)
                    xyz[axis] += m_scale * delta[axis];
        }
        points->SetPoint(id, xyz);
    }
    
    m_displayMesh->SetPoints(points);
    m_displayMesh->Modified();
}

void DeformationRenderer::setDeformationScale(double scale) {
    m_scale = scale;
    applyResult(m_result);
}

void DeformationRenderer::resetDeformation() {
    applyResult(core::CalculationResultPtr());
}

void DeformationRenderer::clear() {
    m_result.reset();
    m_actor = nullptr;
    m_mapper = nullptr;
    m_displayMesh = nullptr;
    m_originalMesh = nullptr;
}

vtkPolyData *DeformationRenderer::displayMesh() const {
    return m_displayMesh;
}

vtkPolyDataMapper *DeformationRenderer::mapper() const {
    return m_mapper;
}

vtkActor *DeformationRenderer::actor() const {
    return m_actor;
}

} // namespace visualization
} // namespace fosd
