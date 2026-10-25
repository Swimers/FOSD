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
    bool setSettings(const ScalarColorSettings &settings, QString *error = nullptr);
    ScalarColorSettings settings() const {
        return m_settings;
    }
    ScalarColorRange range() const {
        return m_range;
    }
    bool applyResult(const core::CalculationResultPtr &result, vtkPolyData *displayMesh,
                     vtkPolyDataMapper *mapper, QString *error = nullptr);
    bool applyField(const PointScalarField &field, vtkPolyData *displayMesh,
                    vtkPolyDataMapper *mapper, QString *error = nullptr);
    void clear(vtkPolyData *mesh = nullptr, vtkPolyDataMapper *mapper = nullptr);

private:
    vtkSmartPointer<vtkDoubleArray> m_scalars;
    vtkSmartPointer<vtkDoubleArray> m_rawScalars;
    vtkSmartPointer<vtkLookupTable> m_lookupTable;
    ScalarColorSettings m_settings;
    ScalarColorRange m_range;
};
} // namespace visualization
} // namespace fosd
