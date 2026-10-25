#pragma once
#include "visualization/VisualizationTypes.h"
#include <vtkSmartPointer.h>
class vtkDoubleArray;
class vtkLookupTable;
class vtkPolyData;
class vtkPolyDataMapper;
namespace fosd {
namespace visualization {
class ScalarColorMapper final {
public:
    ScalarColorMapper();
    ~ScalarColorMapper();
    void setScalarField(ScalarField field);
    void setColorRangeMode(ColorRangeMode mode);
    void setColorRange(double minimum, double maximum);
    void applyResult(const core::CalculationResultPtr &result, vtkPolyData *displayMesh,
                     vtkPolyDataMapper *mapper);
    void clear();

private:
    vtkSmartPointer<vtkDoubleArray> m_scalars;
    vtkSmartPointer<vtkLookupTable> m_lookupTable;
    ScalarField m_field = ScalarField::Magnitude;
    ColorRangeMode m_rangeMode = ColorRangeMode::Automatic;
    double m_minimum = 0.0;
    double m_maximum = 1.0;
};
} // namespace visualization
} // namespace fosd
