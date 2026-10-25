#pragma once
#include "StorageTypes.h"

namespace fosd {
namespace storage {
// Data conversion is independent of the file/database backend. Add serializers
// here for new payloads; writers only consume records in storageSchema().
bool wavelengthRecords(const QString &session, const core::FrameSnapshotPtr &frame,
                       StorageBatch *batch, QString *error);
bool calibratedRecords(const QString &session, const core::CalibratedDataPtr &data,
                       StorageBatch *batch, QString *error);
bool displacementRecords(const QString &session, const core::CalculationResultPtr &result,
                         const StorageConfiguration &configuration, StorageBatch *batch,
                         QString *error);
} // namespace storage
} // namespace fosd
