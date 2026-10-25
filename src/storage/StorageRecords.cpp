#include "StorageRecords.h"
#include <cmath>

namespace fosd {
namespace storage {
namespace {
QString transportName(core::TransportKind transport) {
    switch (transport) {
        case core::TransportKind::Serial:
            return QStringLiteral("serial");
        case core::TransportKind::Udp:
            return QStringLiteral("udp");
        case core::TransportKind::Tcp:
            return QStringLiteral("tcp");
    }
    return QString();
}

StorageRecord baseRecord(const QString &session, const QString &type, const QString &stage,
                         const core::FrameSnapshotPtr &frame) {
    StorageRecord record;
    for (int i = 0; i < ColumnCount; ++i)
        record.values.append(QVariant());
    auto &v = record.values;
    v[SessionId] = session;
    v[DataTypeName] = type;
    v[Stage] = stage;
    v[ProtocolId] = frame->protocolId;
    v[SourceRevision] = QString::number(frame->revision);
    v[ReceivedAtUtc] = frame->receivedAtUtc.toUTC().toString(Qt::ISODateWithMs);
    v[Transport] = transportName(frame->source.transport);
    v[PeerAddress] = frame->source.address;
    v[PeerPort] = frame->source.port;
    const auto wavelength = std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data);
    if (wavelength)
        v[DeviceSequence] = wavelength->sequence;
    return record;
}

bool fail(QString *error, const QString &message) {
    if (error)
        *error = message;
    return false;
}

bool validWavelength(const core::FrameSnapshotPtr &frame) {
    const auto data =
        frame ? std::dynamic_pointer_cast<const core::WavelengthFrame>(frame->data) : nullptr;
    return data && !data->records.isEmpty() && data->records.size() == data->recordCount;
}

QVariant wavelengthValue(double value) {
    if (std::isfinite(value))
        return value;
    if (std::isnan(value))
        return QStringLiteral("NaN");
    return value > 0 ? QStringLiteral("Inf") : QStringLiteral("-Inf");
}

bool validXyz(const QVector<double> &xyz) {
    return xyz.size() == 3 && std::isfinite(xyz[0]) && std::isfinite(xyz[1]) &&
           std::isfinite(xyz[2]);
}
} // namespace
bool wavelengthRecords(const QString &session, const core::FrameSnapshotPtr &frame,
                       StorageBatch *batch, QString *error) {
    if (error)
        error->clear();
    if (!validWavelength(frame))
        return fail(error, QStringLiteral("Expected a nonempty, consistent wavelength frame"));
    batch->type = DataType::Wavelength;
    batch->sourceRevision = frame->revision;
    batch->records.clear();
    const auto data = std::static_pointer_cast<const core::WavelengthFrame>(frame->data);
    for (int i = 0; i < data->records.size(); ++i) {
        const auto &sample = data->records[i];
        auto record =
            baseRecord(session, QStringLiteral("wavelength"), QStringLiteral("raw"), frame);
        record.values[SampleIndex] = i;
        record.values[SourceIndex] = i;
        record.values[Channel] = sample.channel;
        record.values[Sensor] = sample.sensor;
        record.values[WavelengthValue] = wavelengthValue(sample.wavelength);
        batch->records.append(record);
    }
    return true;
}

bool calibratedRecords(const QString &session, const core::CalibratedDataPtr &data,
                       StorageBatch *batch, QString *error) {
    if (error)
        error->clear();
    if (!data || !validWavelength(data->sourceFrame))
        return fail(error, QStringLiteral("Calibrated data requires a wavelength source frame"));
    const auto frame =
        std::static_pointer_cast<const core::WavelengthFrame>(data->sourceFrame->data);
    if (data->calibratedStrains.size() != frame->records.size() ||
        data->selectedStrains.isEmpty() ||
        data->selectedStrains.size() != data->sourceIndices.size())
        return fail(error, QStringLiteral("Inconsistent calibrated data and source indices"));
    batch->type = DataType::CalibratedHardware;
    batch->sourceRevision = data->sourceFrame->revision;
    batch->records.clear();
    for (int stage = 0; stage < 2; ++stage) {
        const auto &strains = stage == 0 ? data->calibratedStrains : data->selectedStrains;
        for (int i = 0; i < strains.size(); ++i) {
            const int source = stage == 0 ? i : data->sourceIndices[i];
            if (source < 0 || source >= frame->records.size() || !std::isfinite(strains[i]))
                return fail(error, QStringLiteral("Invalid calibrated value or source index"));
            auto record =
                baseRecord(session, QStringLiteral("calibrated_hardware"),
                           stage == 0 ? QStringLiteral("calibrated") : QStringLiteral("selected"),
                           data->sourceFrame);
            const auto &sample = frame->records[source];
            record.values[ConfigurationRevision] = QString::number(data->configurationRevision);
            record.values[SampleIndex] = i;
            record.values[SourceIndex] = source;
            record.values[Channel] = sample.channel;
            record.values[Sensor] = sample.sensor;
            record.values[WavelengthValue] = wavelengthValue(sample.wavelength);
            record.values[StrainValue] = strains[i];
            batch->records.append(record);
        }
    }
    return true;
}

bool displacementRecords(const QString &session, const core::CalculationResultPtr &result,
                         const StorageConfiguration &configuration, StorageBatch *batch,
                         QString *error) {
    if (error)
        error->clear();
    if (!result || !result->sourceFrame)
        return fail(error, QStringLiteral("Displacement data requires a source frame"));
    const auto ids = configuration.allPoints ? result->displacementMap.keys().toVector()
                                             : configuration.pointIds;
    if (ids.isEmpty())
        return fail(error, QStringLiteral("No displacement points available"));
    batch->type = DataType::PointDisplacement;
    batch->sourceRevision = result->sourceFrame->revision;
    batch->records.clear();
    for (PointId id : ids) {
        const auto delta = result->displacementMap.constFind(id);
        const auto xyz = result->deformedModel.constFind(id);
        if (delta == result->displacementMap.constEnd() ||
            xyz == result->deformedModel.constEnd() || !validXyz(delta.value()) ||
            !validXyz(xyz.value()))
            return fail(
                error,
                QStringLiteral("Missing or invalid displacement point %1").arg(qlonglong(id)));
        auto record = baseRecord(session, QStringLiteral("point_displacement"),
                                 QStringLiteral("calculated"), result->sourceFrame);
        record.values[AlgorithmId] = result->algorithmId;
        record.values[CalculationRevision] = QString::number(result->revision);
        record.values[ModelRevision] = QString::number(result->modelRevision);
        record.values[ConfigurationRevision] = QString::number(result->configurationRevision);
        record.values[PointIdCol] = QVariant::fromValue(qlonglong(id));
        for (int axis = 0; axis < 3; ++axis) {
            record.values[DxMm + axis] = delta.value()[axis];
            record.values[XMm + axis] = xyz.value()[axis];
        }
        batch->records.append(record);
    }
    return true;
}
} // namespace storage
} // namespace fosd
