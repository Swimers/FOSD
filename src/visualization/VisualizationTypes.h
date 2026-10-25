#pragma once
#include "core/Datamodel/ModelData.h"
#include <QMetaType>
#include <QString>
#include <array>

#include <vtkType.h>

namespace fosd {
namespace visualization {
enum class ScalarField { DisplacementX, DisplacementY, DisplacementZ, Magnitude, Custom };

// Physical retains the field's unit and sign (the legacy non-normalized mode).
enum class ScalarMappingMode { Normalized, Physical };
enum class ScalarValueMode { Signed, Absolute };
enum class ColorRangeMode { Automatic, Manual };

struct ScalarColorSettings {
    bool enabled = false;
    ScalarField field = ScalarField::Magnitude;
    ScalarMappingMode mappingMode = ScalarMappingMode::Normalized;
    ScalarValueMode valueMode = ScalarValueMode::Signed;
    ColorRangeMode rangeMode = ColorRangeMode::Manual;
    double minimum = 0.0;
    double maximum = 50.0;
    double hueStart = 0.6;
    double hueEnd = 0.1;
};

// Generic point field, independent of hardware strain arrays. IDs must match
// the model's VTK point IDs; custom fields are explicitly bound to a revision.
struct PointScalarField {
    QString id;
    QString label;
    QString unit;
    quint64 modelRevision = 0;
    QMap<core::PointId, double> values;
};

struct ScalarColorRange {
    double dataMinimum = 0.0; // After Signed/Absolute, before normalization.
    double dataMaximum = 0.0;
    double lookupMinimum = 0.0;
    double lookupMaximum = 1.0;
};
enum class CameraPreset { Isometric, Front, Back, Left, Right, Top, Bottom };

struct KeyPointDefinition {
    vtkIdType pointId = -1;
    QString label;
};
struct PickedPointInfo {
    vtkIdType pointId = -1;
    std::array<double, 3> originalPositionMm{{0.0, 0.0, 0.0}};
    std::array<double, 3> displayPositionMm{{0.0, 0.0, 0.0}};
    bool valid = false;
};

} // namespace visualization
} // namespace fosd

Q_DECLARE_METATYPE(fosd::core::ModelDataPtr)
Q_DECLARE_METATYPE(fosd::visualization::ScalarField)
Q_DECLARE_METATYPE(fosd::visualization::ColorRangeMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarMappingMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarValueMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarColorSettings)
Q_DECLARE_METATYPE(fosd::visualization::PointScalarField)
Q_DECLARE_METATYPE(fosd::visualization::CameraPreset)
Q_DECLARE_METATYPE(fosd::visualization::KeyPointDefinition)
Q_DECLARE_METATYPE(fosd::visualization::PickedPointInfo)
