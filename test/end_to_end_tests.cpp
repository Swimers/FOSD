#include "calculation/CalculationControl.h"
#include "calculation/HYYIFE.h"
#include "protocol/MPWavelengthParser.h"
#include "protocol/ProtocolControl.h"
#include <QFile>
#include <QSet>
#include <QSignalSpy>
#include <QSqlError>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QUdpSocket>
#include <QUuid>
#include <QtEndian>
#include <QtTest>
#include <cmath>
#include <cstring>
#include <limits>

using namespace fosd;
using namespace fosd::Calculation;
using namespace fosd::storage;
namespace {
const int recordCount = 12;
const int frameCount = 3;
const int rowsPerFrame = recordCount * 3 + 2;
const vtkIdType largePointId = std::numeric_limits<vtkIdType>::max() - 2;

protocol::FrameParsers mpParsers() {
    protocol::FrameParsers parsers;
    parsers.emplace_back(new protocol::MPWavelengthParser);
    return parsers;
}
double reference(int index) { return 1000.0 + index * 10; }
float wavelength(int frame, int index) { return float(reference(index) + (frame + 1) * (index + 1)); }
quint32 sequence(int frame) { return quint32(0x120000 + frame); }
double calibratedStrain(int frame, int source) { return 3.0 + 0.5 * (frame + 1) * (source + 1); }
Strain selectedStrains(int frame) {
    Strain strains;
    for (int index = 0; index < recordCount; ++index)
        strains.push_back(calibratedStrain(frame, recordCount - 1 - index) * (index + 1));
    return strains;
}
QByteArray wirePacket(int frame) {
    QByteArray bytes = QByteArray::fromHex("5aa501010c0000000000");
    qToLittleEndian<quint32>(sequence(frame), reinterpret_cast<uchar *>(bytes.data() + 6));
    for (int index = 0; index < recordCount; ++index) {
        bytes.append(char(index / 4 + 1)); bytes.append(char(index % 4 + 1));
        const float value = wavelength(frame, index);
        quint32 bits;
        std::memcpy(&bits, &value, sizeof(bits));
        char encoded[4];
        qToLittleEndian<quint32>(bits, reinterpret_cast<uchar *>(encoded));
        bytes.append(encoded, 4);
    }
    return bytes;
}
CalculationConfiguration calibration() {
    CalculationConfiguration config;
    config.strainCalibration.calibrationFactor = 2;
    config.strainCalibration.A = 4; config.strainCalibration.B = 3;
    for (int index = 0; index < recordCount; ++index) {
        config.strainCalibration.originalWavelength.push_back(reference(index));
        config.channelCalibration.channelIndices.push_back(recordCount - 1 - index);
        config.channelCalibration.calibrationFactors.push_back(index + 1);
    }
    return config;
}
core::PointMap modelPoints() {
    core::PointMap points;
    points.insert(7, {100, 10, -20});
    points.insert(7001, {800, -15, 30});
    points.insert(largePointId, {1350, 25, 10});
    return points;
}
bool hasState(const QSignalSpy &states, communication::Channel channel, communication::ConnectionState state) {
    for (const auto &arguments : states)
        if (qvariant_cast<communication::Channel>(arguments[0]) == channel &&
            qvariant_cast<communication::ConnectionState>(arguments[1]) == state)
            return true;
    return false;
}
QByteArray receivedBytes(const QSignalSpy &received) {
    QByteArray bytes;
    for (const auto &arguments : received) bytes += arguments[1].toByteArray();
    return bytes;
}
bool closeEnough(double actual, double expected) {
    return std::isfinite(actual) && std::isfinite(expected) &&
        std::abs(actual - expected) <= 1e-10 * (1.0 + std::abs(expected));
}
// Read the closed output independently of IStorageWriter. Fixture text fields
// contain no CSV delimiters, quotes or newlines, so a strict split is sufficient.
bool readRows(const QString &path, StorageFormat format, QVector<QStringList> *rows, QString *error) {
    rows->clear();
    if (format == StorageFormat::Csv) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) { *error = file.errorString(); return false; }
        if (file.readLine() != storageColumnNames().join(',').toUtf8() + '\n') {
            *error = QStringLiteral("Unexpected CSV header"); return false;
        }
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            if (!line.endsWith('\n')) { *error = QStringLiteral("Incomplete CSV record"); return false; }
            line.chop(1);
            const auto fields = QString::fromUtf8(line).split(',', QString::KeepEmptyParts);
            if (fields.size() != ColumnCount) { *error = QStringLiteral("Unexpected CSV column count"); return false; }
            rows->append(fields);
        }
        return true;
    }
    const QString connection = QUuid::createUuid().toString();
    bool success = false;
    {
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", connection);
        database.setDatabaseName(path); database.setConnectOptions("QSQLITE_OPEN_READONLY");
        if (!database.open()) *error = database.lastError().text();
        else {
            QSqlQuery query(database);
            success = query.exec("SELECT " + storageColumnNames().join(',') + " FROM fosd_records ORDER BY record_id");
            if (!success) *error = query.lastError().text();
            while (success && query.next()) {
                QStringList fields;
                for (int column = 0; column < ColumnCount; ++column) {
                    const auto value = query.value(column);
                    fields.append(value.type() == QVariant::Double
                        ? QString::number(value.toDouble(), 'g', 17) : value.toString());
                }
                rows->append(fields);
            }
        }
    }
    QSqlDatabase::removeDatabase(connection);
    return success;
}
}

class EndToEndTests final : public QObject {
    Q_OBJECT
private slots:
    void communicationToStorage_data();
    void communicationToStorage();
};
void EndToEndTests::communicationToStorage_data() {
    QTest::addColumn<bool>("tcp");
    QTest::addColumn<int>("format");
    QTest::newRow("udp-csv") << false << int(StorageFormat::Csv);
    QTest::newRow("udp-sqlite") << false << int(StorageFormat::SQLite);
    QTest::newRow("tcp-fragments-csv") << true << int(StorageFormat::Csv);
    QTest::newRow("tcp-fragments-sqlite") << true << int(StorageFormat::SQLite);
}
void EndToEndTests::communicationToStorage() {
    QFETCH(bool, tcp);
    QFETCH(int, format);
    const StorageFormat storageFormat = StorageFormat(format);
    const auto channel = tcp ? communication::Channel::Tcp : communication::Channel::Udp;
    QTemporaryDir directory; QVERIFY(directory.isValid());
    QUdpSocket udpPeer;
    QTcpServer tcpServer;
    std::unique_ptr<QTcpSocket> tcpPeer;
    // Use the production decoder, MP parser, calibration, HYY and storage
    // backends. Only the external device is represented by a loopback socket.
    protocol::ProtocolControl protocol(nullptr, mpParsers);
    CalculationControl calculation(protocol.dataModel());
    auto &transport = protocol.communication();
    auto &storage = calculation.storage();
    connect(&protocol, &protocol::ProtocolControl::frameParsed,
            &storage, &StorageControl::writeWavelengthFrame);
    QSignalSpy states(&transport, &communication::ConnectControl::stateChanged);
    QSignalSpy raw(&transport, &communication::ConnectControl::rawDataReceived);
    QSignalSpy packets(&transport, &communication::ConnectControl::packetReceived);
    QSignalSpy parsed(&protocol, &protocol::ProtocolControl::frameParsed);
    QSignalSpy results(&calculation, &CalculationControl::resultReady);
    QSignalSpy configured(&calculation, &CalculationControl::configurationAccepted);
    QSignalSpy models(&calculation, &CalculationControl::modelAccepted);
    QSignalSpy started(&storage, &StorageControl::started), stopped(&storage, &StorageControl::stopped);
    QSignalSpy written(&storage, &StorageControl::batchWritten);
    QSignalSpy communicationErrors(&transport, &communication::ConnectControl::errorOccurred);
    QSignalSpy decoderErrors(&transport, &communication::ConnectControl::decoderErrorOccurred);
    QSignalSpy protocolErrors(&protocol, &protocol::ProtocolControl::errorOccurred);
    QSignalSpy calculationErrors(&calculation, &CalculationControl::errorOccurred);
    QSignalSpy storageErrors(&storage, &StorageControl::errorOccurred);
    QVERIFY(raw.isValid() && packets.isValid() && parsed.isValid() && results.isValid() && written.isValid());

    StorageConfiguration config;
    config.format = storageFormat;
    config.path = directory.filePath(tcp ? "tcp" : "udp") +
        (storageFormat == StorageFormat::Csv ? ".csv" : ".sqlite");
    config.dataTypes = DataType::Wavelength | DataType::CalibratedHardware | DataType::PointDisplacement;
    config.pointIds = {7, largePointId};
    storage.start(config);
    calculation.setConfiguration(calibration()); calculation.setRawPointMap(modelPoints());
    QTRY_COMPARE(started.count(), 1); QTRY_COMPARE(configured.count(), 1); QTRY_COMPARE(models.count(), 1);

    QHostAddress receiveAddress;
    quint16 receivePort = 0, sourcePort = 0;
    if (tcp) {
        QVERIFY(tcpServer.listen(QHostAddress::LocalHost, 0));
        communication::TcpConfig connection;
        connection.role = communication::TcpRole::Client; connection.port = tcpServer.serverPort();
        transport.openTcp(connection);
        QTRY_VERIFY(tcpServer.hasPendingConnections());
        tcpPeer.reset(tcpServer.nextPendingConnection()); QVERIFY(tcpPeer);
        QTRY_VERIFY(hasState(states, channel, communication::ConnectionState::Connected));
        sourcePort = tcpPeer->localPort();
    } else {
        QVERIFY(udpPeer.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
        communication::UdpConfig connection;
        connection.bindAddress = QHostAddress::LocalHost; connection.localPort = 0;
        connection.defaultPeer.address = QHostAddress::LocalHost; connection.defaultPeer.port = udpPeer.localPort();
        transport.openUdp(connection);
        QTRY_VERIFY(hasState(states, channel, communication::ConnectionState::Ready));
        // Discover the OS-assigned receive port through the real send API,
        // avoiding a reserve/close/rebind race and fixed-port dependencies.
        const QByteArray probe("pipeline-ready");
        transport.send(channel, probe);
        QTRY_VERIFY(udpPeer.hasPendingDatagrams());
        QByteArray reply(int(udpPeer.pendingDatagramSize()), Qt::Uninitialized);
        QCOMPARE(udpPeer.readDatagram(reply.data(), reply.size(), &receiveAddress, &receivePort), qint64(probe.size()));
        QCOMPARE(reply, probe); QVERIFY(receivePort != 0);
        sourcePort = udpPeer.localPort();
    }

    const QDateTime beforeReceive = QDateTime::currentDateTimeUtc();
    QVector<core::FrameSnapshotPtr> snapshots;
    QVector<core::CalculationResultPtr> outputs;
    QByteArray sent;
    for (int frame = 0; frame < frameCount; ++frame) {
        const auto bytes = wirePacket(frame);
        if (tcp) {
            QCOMPARE(tcpPeer->write(bytes.left(5)), qint64(5)); tcpPeer->flush();
            QTRY_COMPARE(receivedBytes(raw), sent + bytes.left(5));
            QCOMPARE(packets.count(), frame); QCOMPARE(parsed.count(), frame);
            QCOMPARE(results.count(), frame); QCOMPARE(written.count(), frame * 3);
            QCOMPARE(tcpPeer->write(bytes.mid(5)), qint64(bytes.size() - 5)); tcpPeer->flush();
        } else {
            QCOMPARE(udpPeer.writeDatagram(bytes, receiveAddress, receivePort), qint64(bytes.size()));
        }
        sent += bytes;
        // Calculation intentionally polls latest(). Wait for each complete
        // frame before sending the next; do not assume lossless burst solving.
        QTRY_COMPARE(packets.count(), frame + 1); QTRY_COMPARE(parsed.count(), frame + 1);
        QTRY_COMPARE(results.count(), frame + 1); QTRY_COMPARE(written.count(), (frame + 1) * 3);
        QCOMPARE(packets[frame][1].toByteArray(), bytes);
        QCOMPARE(qvariant_cast<communication::Channel>(packets[frame][0]), channel);
        const auto peer = qvariant_cast<communication::Endpoint>(packets[frame][2]);
        QVERIFY(peer.address.isLoopback()); QCOMPARE(peer.port, sourcePort);
        const auto snapshot = qvariant_cast<core::FrameSnapshotPtr>(parsed[frame][0]);
        const auto result = qvariant_cast<core::CalculationResultPtr>(results[frame][0]);
        QVERIFY(snapshot && result); QVERIFY(result->sourceFrame == snapshot);
        QVERIFY(protocol.dataModel()->latest() == snapshot); QVERIFY(calculation.dataModel()->latest() == result);
        QCOMPARE(snapshot->revision, quint64(frame + 1)); QCOMPARE(result->revision, quint64(frame + 1));
        QCOMPARE(snapshot->protocolId, QStringLiteral("mp.wavelength"));
        QCOMPARE(snapshot->source.transport, tcp ? core::TransportKind::Tcp : core::TransportKind::Udp);
        QCOMPARE(snapshot->source.port, sourcePort); QCOMPARE(QHostAddress(snapshot->source.address), peer.address);
        QVERIFY(snapshot->receivedAtUtc >= beforeReceive && snapshot->receivedAtUtc <= QDateTime::currentDateTimeUtc());
        const auto data = std::dynamic_pointer_cast<const core::WavelengthFrame>(snapshot->data);
        QVERIFY(data); QCOMPARE(data->sequence, sequence(frame)); QCOMPARE(data->records.size(), recordCount);
        for (int index = 0; index < recordCount; ++index) {
            QCOMPARE(data->records[index].wavelength, wavelength(frame, index));
            QCOMPARE(data->records[index].channel, quint8(index / 4 + 1));
            QCOMPARE(data->records[index].sensor, quint8(index % 4 + 1));
        }
        QVERIFY(result->strains == QVector<double>::fromStdVector(selectedStrains(frame)));
        QCOMPARE(result->algorithmId, QStringLiteral("HYY.IFE"));
        QCOMPARE(result->displacementMap.size(), 3); QCOMPARE(result->deformedModel.rawPointMap.size(), 3);
        snapshots.append(snapshot); outputs.append(result);
    }
    QCOMPARE(receivedBytes(raw), sent);
    transport.close(channel); QTRY_VERIFY(hasState(states, channel, communication::ConnectionState::Closed));
    storage.stop(); QTRY_COMPARE(stopped.count(), 1);
    QCOMPARE(communicationErrors.count(), 0); QCOMPARE(decoderErrors.count(), 0);
    QCOMPARE(protocolErrors.count(), 0); QCOMPARE(calculationErrors.count(), 0); QCOMPARE(storageErrors.count(), 0);

    QVector<QStringList> rows; QString error;
    QVERIFY2(readRows(config.path, storageFormat, &rows, &error), qPrintable(error));
    QCOMPARE(rows.size(), frameCount * rowsPerFrame);
    QSet<QString> keys;
    HYYIFE expectedAlgorithm;
    for (const auto &row : rows) {
        QCOMPARE(row[SessionId], started[0][0].toString());
        const int frame = row[SourceRevision].toInt() - 1;
        QVERIFY(frame >= 0 && frame < frameCount);
        const auto &snapshot = snapshots[frame];
        QCOMPARE(row[ProtocolId], snapshot->protocolId);
        QCOMPARE(row[ReceivedAtUtc], snapshot->receivedAtUtc.toString(Qt::ISODateWithMs));
        QCOMPARE(row[Transport], tcp ? QStringLiteral("tcp") : QStringLiteral("udp"));
        QCOMPARE(row[PeerAddress], snapshot->source.address); QCOMPARE(row[PeerPort].toUInt(), uint(sourcePort));
        QCOMPARE(row[DeviceSequence].toUInt(), uint(sequence(frame)));
        QString key = row[SourceRevision] + '/' + row[DataTypeName] + '/' + row[Stage] + '/';
        if (row[DataTypeName] == "point_displacement") {
            key += row[PointId];
            const auto id = vtkIdType(row[PointId].toLongLong());
            QVERIFY(config.pointIds.contains(id)); QCOMPARE(row[Stage], QStringLiteral("calculated"));
            QCOMPARE(row[AlgorithmId], QStringLiteral("HYY.IFE"));
            QCOMPARE(row[CalculationRevision], QString::number(outputs[frame]->revision));
            const auto xyz = modelPoints().value(id);
            Point3D point; point.x = xyz[0]; point.y = xyz[1]; point.z = xyz[2];
            // HYY is treated as a black box. Check interface hand-off and unit
            // conversion, without asserting its internal matrix or DOF layout.
            const auto delta = expectedAlgorithm.calculateDisplacement(point,
                expectedAlgorithm.calculateDofs(selectedStrains(frame)));
            for (int axis = 0; axis < 3; ++axis) {
                const double millimetres = delta[size_t(axis)] * 1000;
                QVERIFY(closeEnough(row[DxMm + axis].toDouble(), millimetres));
                QVERIFY(closeEnough(row[XMm + axis].toDouble(), xyz[axis] + millimetres));
                QVERIFY(closeEnough(outputs[frame]->displacementMap.value(id)[axis], millimetres));
            }
            QCOMPARE(row[ModelRevision], QStringLiteral("1"));
            QCOMPARE(row[ConfigurationRevision], QStringLiteral("1"));
        } else {
            const bool raw = row[DataTypeName] == "wavelength";
            QVERIFY(raw || row[DataTypeName] == "calibrated_hardware");
            const bool selected = row[Stage] == "selected";
            QVERIFY(raw ? row[Stage] == "raw" : selected || row[Stage] == "calibrated");
            const int index = row[SampleIndex].toInt();
            QVERIFY(index >= 0 && index < recordCount);
            key += row[SampleIndex];
            const int source = selected ? recordCount - 1 - index : index;
            QCOMPARE(row[SourceIndex].toInt(), source);
            QCOMPARE(row[Channel].toInt(), source / 4 + 1); QCOMPARE(row[Sensor].toInt(), source % 4 + 1);
            QCOMPARE(row[WavelengthValue].toDouble(), double(wavelength(frame, source)));
            if (!raw) {
                QCOMPARE(row[StrainValue].toDouble(), selected ? selectedStrains(frame)[size_t(index)]
                                                              : calibratedStrain(frame, source));
                QCOMPARE(row[ConfigurationRevision], QStringLiteral("1"));
            }
        }
        QVERIFY2(!keys.contains(key), qPrintable("Duplicate stored record: " + key));
        keys.insert(key);
    }
    QCOMPARE(keys.size(), frameCount * rowsPerFrame);
    for (int frame = 0; frame < frameCount; ++frame) {
        const auto prefix = QString::number(frame + 1) + '/';
        for (int index = 0; index < recordCount; ++index) {
            QVERIFY(keys.contains(prefix + "wavelength/raw/" + QString::number(index)));
            QVERIFY(keys.contains(prefix + "calibrated_hardware/calibrated/" + QString::number(index)));
            QVERIFY(keys.contains(prefix + "calibrated_hardware/selected/" + QString::number(index)));
        }
        for (vtkIdType id : config.pointIds)
            QVERIFY(keys.contains(prefix + "point_displacement/calculated/" + QString::number(qlonglong(id))));
    }
}
QTEST_GUILESS_MAIN(EndToEndTests)
#include "end_to_end_tests.moc"
