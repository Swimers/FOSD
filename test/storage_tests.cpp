#include "ParameterTestIsolation.h"
#include "storage/CsvWriter.h"
#include "storage/SQLiteWriter.h"
#include "storage/StorageRecords.h"
#include "calculation/CalculationControl.h"
#include "protocol/ProtocolControl.h"
#include "protocol/MPWavelengthParser.h"
#include <QFile>
#include <QSignalSpy>
#include <QSqlError>
#include <QSqlQuery>
#include <QTemporaryDir>
#include <QUuid>
#include <QtEndian>
#include <QtTest>
#include <atomic>
#include <cstring>
#include <limits>

using namespace fosd;
using namespace fosd::storage;
using namespace fosd::Calculation;
namespace {
const core::PointId pointId = std::numeric_limits<core::PointId>::max() - 2;
core::FrameSnapshotPtr frame(quint64 revision = 1) {
    core::WavelengthFrame data;
    data.recordCount = 3;
    data.sequence = 123;
    const float values[] = {1002, 2004, 3006};
    for (int i = 0; i < 3; ++i) {
        core::WavelengthRecord record;
        record.channel = quint8(i + 1);
        record.sensor = quint8(i + 4);
        record.wavelength = values[i];
        data.records.append(record);
    }
    core::FrameSnapshot snapshot;
    snapshot.revision = revision;
    snapshot.protocolId = QString::fromUtf8("测试,\"协议\"\n");
    snapshot.receivedAtUtc = QDateTime::fromString("2026-10-17T01:02:03.456Z", Qt::ISODateWithMs);
    snapshot.source.transport = core::TransportKind::Udp;
    snapshot.source.address = QStringLiteral("127.0.0.1");
    snapshot.source.port = 4567;
    snapshot.data = std::make_shared<const core::WavelengthFrame>(std::move(data));
    return std::make_shared<const core::FrameSnapshot>(std::move(snapshot));
}

CalculationConfiguration settings() {
    CalculationConfiguration config;
    config.strainCalibration.originalWavelength = {1000, 2000, 3000};
    config.strainCalibration.calibrationFactor = 2;
    config.strainCalibration.A = 4;
    config.strainCalibration.B = 3;
    config.channelCalibration.channelIndices = {2, 0};
    config.channelCalibration.calibrationFactors = {2, 3};
    return config;
}

core::ModelData model() {
    core::ModelData data;
    data.insert(7, {10, 20, 30});
    data.insert(pointId, {40, 50, 60});
    return data;
}

struct Trace {
    std::atomic<QThread *> writerThread{nullptr}, solveThread{nullptr};
    std::atomic<int> writes{0};
    std::atomic<bool> wrongThread{false}, closed{false}, destroyed{false};
    bool failWrite = false;
};
class TestIFE final : public IFrameIFE {
public:
    explicit TestIFE(Trace *trace) : m_trace(trace) {
    }
    QString IFEId() const override {
        return QStringLiteral("test.translation");
    }
    NodeDofs calculateDofs(const Strain &strain) const override {
        m_trace->solveThread.store(QThread::currentThread());
        return {strain[0]};
    }
    Displacement calculateDisplacement(const Point3D &, const NodeDofs &dofs) const override {
        return {{dofs[0] / 1000.0, 0.002, 0.003}};
    }

private:
    Trace *m_trace;
};
std::unique_ptr<IFrameIFE> algorithm(Trace *trace) {
    return std::unique_ptr<IFrameIFE>(new TestIFE(trace));
}

class TracedWriter final : public IStorageWriter {
public:
    TracedWriter(Trace *trace, StorageFormat format)
        : m_trace(trace), m_inner(createStorageWriter(format)) {
        m_trace->writerThread.store(QThread::currentThread());
    }
    ~TracedWriter() override {
        checkThread();
        m_trace->destroyed.store(true);
    }
    StorageFormat format() const override {
        return m_inner->format();
    }
    bool open(const QString &path, QString *error) override {
        checkThread();
        return m_inner->open(path, error);
    }
    bool write(const QVector<StorageRecord> &records, QString *error) override {
        checkThread();
        m_trace->writes.fetch_add(1);
        if (m_trace->failWrite) {
            *error = QStringLiteral("Injected write failure");
            return false;
        }
        return m_inner->write(records, error);
    }
    bool flush(QString *error) override {
        checkThread();
        return m_inner->flush(error);
    }
    void close() override {
        checkThread();
        m_trace->closed.store(true);
        m_inner->close();
    }
    bool isOpen() const override {
        return m_inner->isOpen();
    }

private:
    void checkThread() {
        if (m_trace->writerThread.load() != QThread::currentThread())
            m_trace->wrongThread.store(true);
    }
    Trace *m_trace;
    std::unique_ptr<IStorageWriter> m_inner;
};
std::unique_ptr<IStorageWriter> writer(Trace *trace, StorageFormat format) {
    return std::unique_ptr<IStorageWriter>(new TracedWriter(trace, format));
}

StorageConfiguration configuration(const QString &path, StorageFormat format) {
    StorageConfiguration config;
    config.path = path;
    config.format = format;
    config.dataTypes =
        DataType::Wavelength | DataType::CalibratedHardware | DataType::PointDisplacement;
    config.pointIds.append(pointId);
    return config;
}

QByteArray readFile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return QByteArray();
    return file.readAll();
}

// Read CSV independently of the writer, including escaped multiline UTF-8 fields.
QVector<QStringList> csvRows(const QString &path) {
    const QString text = QString::fromUtf8(readFile(path));
    QVector<QStringList> rows;
    QStringList row;
    QString field;
    bool quoted = false;
    for (int i = 0; i < text.size(); ++i) {
        const QChar ch = text[i];
        if (ch == '"') {
            if (quoted && i + 1 < text.size() && text[i + 1] == '"') {
                field += '"';
                ++i;
            } else
                quoted = !quoted;
        } else if (ch == ',' && !quoted) {
            row.append(field);
            field.clear();
        } else if (ch == '\n' && !quoted) {
            row.append(field);
            rows.append(row);
            row.clear();
            field.clear();
        } else
            field += ch;
    }
    return rows;
}

// Each helper owns a separately named connection and releases all queries before
// removeDatabase, so tests also catch backend connection lifecycle warnings.
class Database {
public:
    explicit Database(const QString &path)
        : name(QUuid::createUuid().toString()), db(QSqlDatabase::addDatabase("QSQLITE", name)) {
        db.setDatabaseName(path);
        db.open();
    }
    ~Database() {
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(name);
    }
    bool exec(const QString &sql) {
        QSqlQuery query(db);
        return query.exec(sql);
    }
    QVector<QStringList> rows() {
        QVector<QStringList> result;
        QSqlQuery query(db);
        if (!query.exec("SELECT " + storageColumnNames().join(',') +
                        " FROM fosd_records ORDER BY record_id"))
            return result;
        while (query.next()) {
            QStringList row;
            for (int i = 0; i < ColumnCount; ++i)
                row.append(query.value(i).toString());
            result.append(row);
        }
        return result;
    }
    QString name;
    QSqlDatabase db;
};
QVector<QStringList> savedRows(const QString &path, StorageFormat format) {
    if (format == StorageFormat::Csv) {
        auto rows = csvRows(path);
        if (!rows.isEmpty())
            rows.removeFirst();
        return rows;
    }
    Database database(path);
    return database.rows();
}

protocol::FrameParsers parsers() {
    protocol::FrameParsers result;
    result.emplace_back(new protocol::MPWavelengthParser);
    return result;
}

QByteArray packet() {
    QByteArray result = QByteArray::fromHex("5aa50101030001000000");
    const float values[] = {1002, 2004, 3006};
    for (int i = 0; i < 3; ++i) {
        result.append(char(i + 1));
        result.append(char(i + 4));
        quint32 bits;
        std::memcpy(&bits, &values[i], sizeof(bits));
        char bytes[4];
        qToLittleEndian<quint32>(bits, reinterpret_cast<uchar *>(bytes));
        result.append(bytes, 4);
    }
    return result;
}
} // namespace
class StorageTests : public QObject {
    Q_OBJECT
    ParameterTestIsolation m_parameterIsolation;
private slots:
    void init() { m_parameterIsolation.reset(); }

    void csvAppendEscapingAndSchema();
    void sqliteTransactionsAndSchema();
    void serializersRejectInvalidPayloads();
    void calculationToStorage_data();
    void calculationToStorage();
    void hardwareWithoutModel();
    void rawProtocolFramesAreNotSkipped();
    void selectedPointsAndErrors();
    void writeFailureDoesNotStopCalculation();
    void shutdownDrainsPendingWrites();
};
void StorageTests::csvAppendEscapingAndSchema() {
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath("nested/data.csv");
    StorageBatch batch;
    QString error;
    QVERIFY(
        wavelengthRecords("session", frame(std::numeric_limits<quint64>::max()), &batch, &error));
    const double precise = 0.12345678901234567;
    batch.records[0].values[WavelengthValue] = precise;
    CsvWriter writer;
    QVERIFY2(writer.open(path, &error), qPrintable(error));
    QVERIFY(writer.write(batch.records, &error));
    writer.close();
    QVERIFY(writer.open(path, &error));
    QVERIFY(writer.write(batch.records, &error));
    const auto before = readFile(path);
    StorageRecord bad;
    QVERIFY(!writer.write({batch.records[0], bad}, &error));
    QCOMPARE(readFile(path), before);
    writer.close();
    const auto rows = csvRows(path);
    QCOMPARE(rows.size(), 7);
    QCOMPARE(rows[0], storageColumnNames());
    QCOMPARE(rows[1][ProtocolId], frame()->protocolId);
    QCOMPARE(rows[1][SourceRevision], QString::number(std::numeric_limits<quint64>::max()));
    QCOMPARE(rows[1][WavelengthValue].toDouble(), precise);
    QFile foreign(dir.filePath("foreign.csv"));
    QVERIFY(foreign.open(QIODevice::WriteOnly));
    foreign.write("custom,header\n1,2\n");
    foreign.close();
    const auto original = readFile(foreign.fileName());
    QVERIFY(!writer.open(foreign.fileName(), &error));
    QVERIFY(!error.isEmpty());
    QCOMPARE(readFile(foreign.fileName()), original);
    QVERIFY(!writer.write(batch.records, &error));
    QFile incomplete(dir.filePath("incomplete.csv"));
    QVERIFY(incomplete.open(QIODevice::WriteOnly));
    incomplete.write(storageColumnNames().join(',').toUtf8() + "\npartial");
    incomplete.close();
    const auto partial = readFile(incomplete.fileName());
    QVERIFY(!writer.open(incomplete.fileName(), &error));
    QCOMPARE(readFile(incomplete.fileName()), partial);
}

void StorageTests::sqliteTransactionsAndSchema() {
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const QString path = dir.filePath("data.sqlite");
    SQLiteWriter writer;
    QString error;
    StorageBatch batch;
    QVERIFY(wavelengthRecords("session", frame(), &batch, &error));
    QVERIFY2(writer.open(path, &error), qPrintable(error));
    {
        Database database(path);
        QVERIFY(database.exec("CREATE TRIGGER reject_second BEFORE INSERT ON fosd_records "
                              "WHEN NEW.sample_index=1 BEGIN SELECT RAISE(ABORT,'injected'); END"));
    }
    QVERIFY(!writer.write(batch.records, &error));
    QVERIFY(error.contains("injected"));
    {
        Database database(path);
        QCOMPARE(database.rows().size(), 0);
        QVERIFY(database.exec("DROP TRIGGER reject_second"));
    }
    batch.records[0].values[PointIdCol] = QVariant::fromValue(qlonglong(pointId));
    QVERIFY(writer.write(batch.records, &error));
    writer.close();
    QVERIFY(writer.open(path, &error));
    QVERIFY(writer.write(batch.records, &error));
    writer.close();
    {
        Database database(path);
        const auto rows = database.rows();
        QCOMPARE(rows.size(), 6);
        QCOMPARE(rows[0][ProtocolId], frame()->protocolId);
        QCOMPARE(rows[0][PointIdCol], QString::number(qlonglong(pointId)));
    }
    const QString incompatible = dir.filePath("foreign.sqlite");
    {
        Database database(incompatible);
        QVERIFY(database.exec("CREATE TABLE fosd_records (custom TEXT)"));
    }
    QVERIFY(!writer.open(incompatible, &error));
    QVERIFY(error.contains("schema"));
    QVERIFY(!writer.isOpen());
    {
        Database database(incompatible);
        QVERIFY(database.exec("SELECT custom FROM fosd_records"));
    }
}

void StorageTests::serializersRejectInvalidPayloads() {
    StorageBatch batch;
    QString error;
    QVERIFY(!wavelengthRecords("session", core::FrameSnapshotPtr(), &batch, &error));
    auto source = *frame();
    auto wavelengths = *std::static_pointer_cast<const core::WavelengthFrame>(source.data);
    wavelengths.recordCount = 4;
    source.data = std::make_shared<const core::WavelengthFrame>(wavelengths);
    QVERIFY(!wavelengthRecords("session", std::make_shared<const core::FrameSnapshot>(source),
                               &batch, &error));
    wavelengths.recordCount = 3;
    wavelengths.records[0].wavelength = std::numeric_limits<float>::quiet_NaN();
    wavelengths.records[1].wavelength = std::numeric_limits<float>::infinity();
    wavelengths.records[2].wavelength = -std::numeric_limits<float>::infinity();
    source.data = std::make_shared<const core::WavelengthFrame>(wavelengths);
    QVERIFY(wavelengthRecords("session", std::make_shared<const core::FrameSnapshot>(source),
                              &batch, &error));
    QCOMPARE(batch.records[0].values[WavelengthValue].toString(), QStringLiteral("NaN"));
    QCOMPARE(batch.records[1].values[WavelengthValue].toString(), QStringLiteral("Inf"));
    QCOMPARE(batch.records[2].values[WavelengthValue].toString(), QStringLiteral("-Inf"));
    core::CalibratedData calibrated;
    calibrated.sourceFrame = frame();
    calibrated.calibratedStrains = {4, 5, 6};
    calibrated.selectedStrains = {12};
    calibrated.sourceIndices = {3};
    QVERIFY(!calibratedRecords("session", std::make_shared<const core::CalibratedData>(calibrated),
                               &batch, &error));
    calibrated.sourceIndices = {2};
    calibrated.calibratedStrains[0] = std::numeric_limits<double>::quiet_NaN();
    QVERIFY(!calibratedRecords("session", std::make_shared<const core::CalibratedData>(calibrated),
                               &batch, &error));
    // SQLite must preserve special raw values as text, rather than NaN -> NULL.
    QTemporaryDir dir;
    SQLiteWriter writer;
    QVERIFY(writer.open(dir.filePath("special.sqlite"), &error));
    QVERIFY(wavelengthRecords("session", std::make_shared<const core::FrameSnapshot>(source),
                              &batch, &error));
    QVERIFY(writer.write(batch.records, &error));
    writer.close();
    const auto rows = savedRows(dir.filePath("special.sqlite"), StorageFormat::SQLite);
    QCOMPARE(rows[0][WavelengthValue], QStringLiteral("NaN"));
    QCOMPARE(rows[1][WavelengthValue], QStringLiteral("Inf"));
    QCOMPARE(rows[2][WavelengthValue], QStringLiteral("-Inf"));
}

void StorageTests::calculationToStorage_data() {
    QTest::addColumn<int>("format");
    QTest::newRow("csv") << int(StorageFormat::Csv);
    QTest::newRow("sqlite") << int(StorageFormat::SQLite);
}

void StorageTests::calculationToStorage() {
    QFETCH(int, format);
    const auto type = StorageFormat(format);
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    const auto path = dir.filePath(type == StorageFormat::Csv ? "data.csv" : "data.sqlite");
    Trace trace;
    {
        CalculationControl calculation(core::FrameRepositoryPtr(), nullptr,
                                       std::bind(algorithm, &trace),
                                       std::bind(writer, &trace, std::placeholders::_1));
        auto &storage = calculation.storage();
        QSignalSpy started(&storage, &StorageControl::started),
            stopped(&storage, &StorageControl::stopped);
        QSignalSpy batches(&storage, &StorageControl::batchWritten),
            errors(&storage, &StorageControl::errorOccurred);
        QSignalSpy results(&calculation, &CalculationControl::resultReady);
        auto config = configuration(path, type);
        storage.start(config);
        calculation.setConfiguration(settings());
        calculation.setModel(model());
        calculation.submitFrame(frame());
        QTRY_COMPARE(batches.count(), 3);
        QTRY_COMPARE(results.count(), 1);
        storage.stop();
        QTRY_COMPARE(stopped.count(), 1);
        QCOMPARE(errors.count(), 0);
        QCOMPARE(started.count(), 1);
        QCOMPARE(trace.writes.load(), 3);
        QVERIFY(trace.writerThread.load() != QThread::currentThread());
        QCOMPARE(trace.writerThread.load(), trace.solveThread.load());
        const auto rows = savedRows(path, type);
        QCOMPARE(rows.size(), 9);
        for (const auto &row : rows) {
            QCOMPARE(row.size(), int(ColumnCount));
            QCOMPARE(row[SessionId], started[0][0].toString());
            QCOMPARE(row[SourceRevision], QStringLiteral("1"));
            QCOMPARE(row[ReceivedAtUtc], QStringLiteral("2026-10-17T01:02:03.456Z"));
            QCOMPARE(row[Transport], QStringLiteral("udp"));
            QCOMPARE(row[PeerPort].toInt(), 4567);
            QCOMPARE(row[DeviceSequence].toInt(), 123);
        }
        QCOMPARE(rows[0][WavelengthValue].toDouble(), 1002.0);
        QCOMPARE(rows[2][Channel].toInt(), 3);
        QCOMPARE(rows[3][Stage], QStringLiteral("calibrated"));
        QCOMPARE(rows[3][StrainValue].toDouble(), 4.0);
        QCOMPARE(rows[5][StrainValue].toDouble(), 6.0);
        QCOMPARE(rows[6][Stage], QStringLiteral("selected"));
        QCOMPARE(rows[6][SourceIndex].toInt(), 2);
        QCOMPARE(rows[6][StrainValue].toDouble(), 12.0);
        QCOMPARE(rows[7][SourceIndex].toInt(), 0);
        QCOMPARE(rows[8][PointIdCol], QString::number(qlonglong(pointId)));
        QCOMPARE(rows[8][DxMm].toDouble(), 12.0);
        QCOMPARE(rows[8][DyMm].toDouble(), 2.0);
        QCOMPARE(rows[8][XMm].toDouble(), 52.0);
        QCOMPARE(rows[8][ModelRevision], QStringLiteral("1"));
        // Restart appends a new session, selecting only wavelengths.
        config.dataTypes = DataType::Wavelength;
        config.pointIds.clear();
        storage.start(config);
        storage.writeWavelengthFrame(frame(2));
        storage.stop();
        QTRY_COMPARE(stopped.count(), 2);
        QCOMPARE(started.count(), 2);
        QVERIFY(started[0][0].toString() != started[1][0].toString());
        QCOMPARE(savedRows(path, type).size(), 12);
    }
    QVERIFY(trace.closed.load());
    QVERIFY(trace.destroyed.load());
    QVERIFY(!trace.wrongThread.load());
}

void StorageTests::hardwareWithoutModel() {
    QTemporaryDir dir;
    Trace trace;
    CalculationControl calculation(core::FrameRepositoryPtr(), nullptr,
                                   std::bind(algorithm, &trace));
    auto config = configuration(dir.filePath("hardware.csv"), StorageFormat::Csv);
    config.dataTypes = DataType::CalibratedHardware;
    QSignalSpy batches(&calculation.storage(), &StorageControl::batchWritten);
    QSignalSpy results(&calculation, &CalculationControl::resultReady);
    QSignalSpy errors(&calculation, &CalculationControl::errorOccurred);
    calculation.storage().start(config);
    calculation.setConfiguration(settings());
    calculation.submitFrame(frame());
    QTRY_COMPARE(batches.count(), 1);
    QCOMPARE(results.count(), 0);
    QCOMPARE(errors.count(), 0);
    QCOMPARE(savedRows(config.path, config.format).size(), 5);
}

void StorageTests::rawProtocolFramesAreNotSkipped() {
    QTemporaryDir dir;
    protocol::ProtocolControl protocol(nullptr, parsers);
    CalculationControl calculation(
        protocol.dataModel()); // No configuration/model required for raw logging.
    connect(&protocol, &protocol::ProtocolControl::frameParsed, &calculation.storage(),
            &StorageControl::writeWavelengthFrame);
    StorageConfiguration config;
    config.path = dir.filePath("raw.sqlite");
    config.format = StorageFormat::SQLite;
    QSignalSpy started(&calculation.storage(), &StorageControl::started);
    QSignalSpy batches(&calculation.storage(), &StorageControl::batchWritten);
    QSignalSpy errors(&calculation.storage(), &StorageControl::errorOccurred);
    QSignalSpy parseErrors(&protocol, &protocol::ProtocolControl::errorOccurred);
    calculation.storage().start(config);
    QTRY_COMPARE(started.count(), 1);
    for (int i = 0; i < 20; ++i)
        protocol.parseFrame(communication::Channel::Udp, packet());
    QTRY_COMPARE(batches.count(), 20);
    QCOMPARE(errors.count(), 0);
    QCOMPARE(parseErrors.count(), 0);
    const auto rows = savedRows(config.path, config.format);
    QCOMPARE(rows.size(), 60);
    for (int i = 0; i < 20; ++i)
        QCOMPARE(rows[i * 3][SourceRevision].toInt(), i + 1);
}

void StorageTests::selectedPointsAndErrors() {
    QTemporaryDir dir;
    Trace trace;
    CalculationControl calculation(core::FrameRepositoryPtr(), nullptr,
                                   std::bind(algorithm, &trace));
    auto &storage = calculation.storage();
    QSignalSpy errors(&storage, &StorageControl::errorOccurred),
        started(&storage, &StorageControl::started);
    QSignalSpy stopped(&storage, &StorageControl::stopped),
        batches(&storage, &StorageControl::batchWritten);
    QSignalSpy results(&calculation, &CalculationControl::resultReady);
    auto config = configuration(dir.filePath("points.csv"), StorageFormat::Csv);
    config.dataTypes = DataType::PointDisplacement;
    config.pointIds.clear();
    storage.start(config);
    QTRY_COMPARE(errors.count(), 1);
    QCOMPARE(started.count(), 0);
    config.pointIds = {7, 7};
    storage.start(config);
    QTRY_COMPARE(errors.count(), 2);
    config.pointIds = {7, 8};
    storage.start(config); // ID 8 is absent: don't write point 7 partially.
    calculation.setConfiguration(settings());
    calculation.setModel(model());
    calculation.submitFrame(frame());
    QTRY_COMPARE(errors.count(), 3);
    QTRY_COMPARE(results.count(), 1);
    QTRY_COMPARE(stopped.count(), 1);
    QCOMPARE(batches.count(), 0);
    QCOMPARE(savedRows(config.path, config.format).size(), 0);
    const auto error = qvariant_cast<StorageError>(errors.last()[0]);
    QCOMPARE(error.operation, QStringLiteral("write"));
    QCOMPARE(error.sourceRevision, quint64(1));
    QCOMPARE(error.path, config.path);
    config.pointIds.clear();
    config.allPoints = true;
    storage.start(config);
    calculation.submitFrame(frame(2));
    QTRY_COMPARE(batches.count(), 1);
    QCOMPARE(savedRows(config.path, config.format).size(), 2);
    auto replacement = config;
    replacement.path = dir.filePath("must_not_create.csv");
    storage.start(replacement);
    QTRY_COMPARE(errors.count(), 4);
    QVERIFY(!QFile::exists(replacement.path));
    calculation.submitFrame(frame(3));
    QTRY_COMPARE(batches.count(), 2);
    QCOMPARE(savedRows(config.path, config.format).size(), 4);
    storage.stop();
    QTRY_COMPARE(stopped.count(), 2);
    // A regular file cannot act as the parent directory.
    config.path += "/invalid.csv";
    storage.start(config);
    QTRY_COMPARE(errors.count(), 5);
}

void StorageTests::writeFailureDoesNotStopCalculation() {
    QTemporaryDir dir;
    Trace trace;
    trace.failWrite = true;
    CalculationControl calculation(core::FrameRepositoryPtr(), nullptr,
                                   std::bind(algorithm, &trace),
                                   std::bind(writer, &trace, std::placeholders::_1));
    QSignalSpy errors(&calculation.storage(), &StorageControl::errorOccurred);
    QSignalSpy stopped(&calculation.storage(), &StorageControl::stopped);
    QSignalSpy results(&calculation, &CalculationControl::resultReady);
    const auto config = configuration(dir.filePath("failed.csv"), StorageFormat::Csv);
    calculation.storage().start(config);
    calculation.setConfiguration(settings());
    calculation.setModel(model());
    calculation.submitFrame(frame());
    calculation.submitFrame(frame(2));
    QTRY_COMPARE(results.count(), 2);
    QTRY_COMPARE(errors.count(), 1);
    QTRY_COMPARE(stopped.count(), 1);
    QCOMPARE(trace.writes.load(), 1);
    QVERIFY(trace.closed.load());
    QCOMPARE(savedRows(config.path, config.format).size(), 0);
}

void StorageTests::shutdownDrainsPendingWrites() {
    QTemporaryDir dir;
    Trace trace;
    const auto config = configuration(dir.filePath("shutdown.sqlite"), StorageFormat::SQLite);
    core::CalculationRepositoryPtr repository;
    {
        CalculationControl calculation(core::FrameRepositoryPtr(), nullptr,
                                       std::bind(algorithm, &trace),
                                       std::bind(writer, &trace, std::placeholders::_1));
        repository = calculation.dataModel();
        calculation.storage().start(config);
        calculation.setConfiguration(settings());
        calculation.setModel(model());
        for (quint64 i = 1; i <= 20; ++i)
            calculation.submitFrame(frame(i));
        // Destroy immediately, without processing any caller-thread events.
    }
    QVERIFY(trace.closed.load());
    QVERIFY(trace.destroyed.load());
    QVERIFY(!trace.wrongThread.load());
    QCOMPARE(repository->latest()->sourceFrame->revision, quint64(20));
    QCOMPARE(savedRows(config.path, config.format).size(), 180);
}

QTEST_GUILESS_MAIN(StorageTests)
#include "storage_tests.moc"
