#pragma once
#include "core/Datamodel/ModelData.h"
#include <QMetaType>
#include <QString>
#include <array>

#include <vtkType.h>

namespace fosd { namespace visualization {
enum class ScalarField { DisplacementX, DisplacementY, DisplacementZ, Magnitude };
enum class ColorRangeMode { Automatic, Manual };
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
} }
Q_DECLARE_METATYPE(fosd::core::ModelDataPtr)
Q_DECLARE_METATYPE(fosd::visualization::ScalarField)
Q_DECLARE_METATYPE(fosd::visualization::ColorRangeMode)
Q_DECLARE_METATYPE(fosd::visualization::CameraPreset)
Q_DECLARE_METATYPE(fosd::visualization::KeyPointDefinition)
Q_DECLARE_METATYPE(fosd::visualization::PickedPointInfo)
