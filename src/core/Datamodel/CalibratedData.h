#pragma once
#include "FrameData.h"

namespace fosd {
namespace core {
struct CalibratedData {
    FrameSnapshotPtr sourceFrame;
    quint64 configurationRevision = 0;
    QVector<double> calibratedStrains; // Before channel selection/gain.
    QVector<double> selectedStrains;   // Actual IFE input, after selection/gain.
    QVector<int> sourceIndices;        // Selected index -> original record index.
};

using CalibratedDataPtr = std::shared_ptr<const CalibratedData>;

} // namespace core
} // namespace fosd
Q_DECLARE_METATYPE(fosd::core::CalibratedDataPtr)
