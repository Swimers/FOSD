#pragma once

#include "FrameData.h"
#include <QMap>

namespace fosd {
namespace core {
using PointId = qint64;

using PointMap = QMap<PointId, QVector<double>>;
using ModelData = PointMap;
using ModelDataPtr = std::shared_ptr<const ModelData>;

struct CalculationResult {
    FrameSnapshotPtr sourceFrame;
    QString algorithmId;
    quint64 revision = 0;
    quint64 modelRevision = 0;
    quint64 configurationRevision = 0;
    QVector<double> strains;
    // Opaque algorithm intermediate data, retained for diagnostics only.
    // The orchestration layer does not interpret its layout or validate it.
    QVector<double> nodeDofs;
    PointMap displacementMap; // {dx,dy,dz}, millimetres.
    ModelData deformedModel;  // Original coordinates + displacement, millimetres.
};
using CalculationResultPtr = std::shared_ptr<const CalculationResult>;

} // namespace core
} // namespace fosd

Q_DECLARE_METATYPE(fosd::core::PointMap)
Q_DECLARE_METATYPE(fosd::core::CalculationResultPtr)
