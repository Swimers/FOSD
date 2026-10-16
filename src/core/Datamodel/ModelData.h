#pragma once

#include "FrameData.h"
#include <QMap>
#include <vtkType.h>

namespace fosd { namespace core {

using PointMap = QMap<vtkIdType, QVector<double>>;

struct ModelData {
    // Original coordinates in millimetres, exactly {x,y,z} per point.
    QMap<vtkIdType, QVector<double>> rawPointMap;
};
using ModelDataPtr = std::shared_ptr<const ModelData>;

struct CalculationResult {
    FrameSnapshotPtr sourceFrame;
    QString algorithmId;
    quint64 revision = 0;
    quint64 modelRevision = 0;
    quint64 configurationRevision = 0;
    QVector<double> strains;
    // Translations in metres; rotations in radians, in algorithm DOF order.
    QVector<double> nodeDofs;
    PointMap displacementMap; // {dx,dy,dz}, millimetres.
    ModelData deformedModel;  // Original coordinates + displacement, millimetres.
};
using CalculationResultPtr = std::shared_ptr<const CalculationResult>;

} }
Q_DECLARE_METATYPE(fosd::core::ModelData)
Q_DECLARE_METATYPE(fosd::core::CalculationResultPtr)
