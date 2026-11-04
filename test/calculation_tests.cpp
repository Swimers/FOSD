#include "ParameterTestIsolation.h"
#include "calculation/CalculationControl.h"
#include "protocol/ProtocolControl.h"
#include "protocol/MPWavelengthParser.h"
#include "communication/PacketDecoders.h"
#include <QElapsedTimer>
#include <QSignalSpy>
#include <QUdpSocket>
#include <QtEndian>
#include <QtTest>
#include <atomic>
#include <cmath>
#include <cstring>
#include <limits>
#include <thread>
#include <type_traits>

using namespace fosd;
using namespace fosd::Calculation;
static_assert(std::is_const<core::CalculationResultPtr::element_type>::value,
              "Results must be immutable");
static_assert(std::is_const<core::CalculationRepositoryPtr::element_type>::value,
              "Consumers cannot publish");

namespace {
CalculationConfiguration settings() {
    CalculationConfiguration configuration;
    configuration.strainCalibration.originalWavelength.assign(12, 1000.0);
    configuration.strainCalibration.calibrationFactor = 1.0;
    configuration.strainCalibration.A = 1.0;
    for (int index = 0; index < 12; ++index) {
        configuration.channelCalibration.channelIndices.push_back(index);
        configuration.channelCalibration.calibrationFactors.push_back(1.0);
    }
    return configuration;
}

core::ModelData model() {
    core::ModelData data;
    for (int index = 0; index < 30; ++index)
        data.insert(core::PointId(index * 13), {double(index * 40), 10.0, -20.0});
    data.insert(std::numeric_limits<core::PointId>::max() - 2, {1400.0, 20.0, 30.0});
    return data;
}

core::FrameSnapshotPtr frame(float firstValue = 1001.0f, quint64 revision = 1) {
    core::WavelengthFrame values;
    values.recordCount = 12;
    for (int index = 0; index < 12; ++index) {
        core::WavelengthRecord record;
        record.channel = 1;
        record.sensor = quint8(index + 1);
        record.wavelength = index == 0 ? firstValue : 1000.0f;
        values.records.append(record);
    }
    core::FrameSnapshot snapshot;
    snapshot.protocolId = QStringLiteral("mp.wavelength");
    snapshot.revision = revision;
    snapshot.data = std::make_shared<const core::WavelengthFrame>(std::move(values));
    return std::make_shared<const core::FrameSnapshot>(std::move(snapshot));
}

struct Trace {
    std::atomic<QThread *> factoryThread{nullptr};
    std::atomic<QThread *> solveThread{nullptr};
    std::atomic<QThread *> inputThread{nullptr};
    std::atomic<int> pointCalls{0};
    std::atomic<int> solveCalls{0};
    std::atomic<bool> destroyedOnWorker{false};
};
class TranslationIFE final : public IFrameIFE {
public:
    explicit TranslationIFE(Trace *trace) : m_trace(trace) {
        m_trace->factoryThread.store(QThread::currentThread());
    }
    ~TranslationIFE() override {
        m_trace->destroyedOnWorker.store(QThread::currentThread() == m_trace->factoryThread.load());
    }
    QString IFEId() const override {
        return QStringLiteral("test.translation");
    }
    NodeDofs calculateDofs(const Strain &strain) const override {
        m_trace->solveThread.store(QThread::currentThread());
        m_trace->solveCalls.fetch_add(1);
        return {strain.at(0)}; // The controller must not impose HYY's layout.
    }
    Displacement calculateDisplacement(const Point3D &point, const NodeDofs &dofs) const override {
        m_trace->pointCalls.fetch_add(1);
        // Fail after earlier points succeeded, to exercise atomic publication.
        if (point.x >= 400.0 && dofs[0] >= 3.0) {
            return {{std::numeric_limits<double>::quiet_NaN(), 0, 0}};
        }
        return {{dofs[0] * 0.001, 0.002, 0.003}};
    }

private:
    Trace *m_trace;
};
std::unique_ptr<IFrameIFE> translationFactory(Trace *trace) {
    return std::unique_ptr<IFrameIFE>(new TranslationIFE(trace));
}

protocol::FrameParsers mpParsers() {
    protocol::FrameParsers parsers;
    parsers.emplace_back(new protocol::MPWavelengthParser);
    return parsers;
}

std::unique_ptr<communication::IPacketDecoder> observedDecoder(Trace *trace,
                                                               communication::Channel channel) {
    trace->inputThread.store(QThread::currentThread());
    return communication::createPacketDecoder(channel);
}

QByteArray wirePacket() {
    QByteArray packet = QByteArray::fromHex("5aa501010c0001000000");
    for (int index = 0; index < 12; ++index) {
        packet.append(char(1));
        packet.append(char(index + 1));
        const float value = index == 0 ? 1001.0f : 1000.0f;
        quint32 bits;
        std::memcpy(&bits, &value, sizeof(bits));
        char bytes[4];
        qToLittleEndian<quint32>(bits, reinterpret_cast<uchar *>(bytes));
        packet.append(bytes, 4);
    }
    return packet;
}

void readResults(core::CalculationRepositoryPtr repository, std::atomic<bool> *stop,
                 std::atomic<bool> *invalid, std::atomic<int> *reads) {
    quint64 previous = 0;
    while (!stop->load()) {
        const auto result = repository->latest();
        if (!result) {
            std::this_thread::yield();
            continue;
        }
        if (result->revision < previous || result->deformedModel.size() != 31 ||
            result->displacementMap.size() != 31 || !result->sourceFrame)
            invalid->store(true);
        previous = result->revision;
        reads->fetch_add(1);
    }
}

class Readers {
public:
    explicit Readers(core::CalculationRepositoryPtr repository) {
        for (int index = 0; index < 3; ++index)
            threads.emplace_back(readResults, repository, &stop, &invalid, &reads);
    }
    ~Readers() {
        stop.store(true);
        for (auto &thread : threads)
            thread.join();
    }
    std::atomic<bool> stop{false}, invalid{false};
    std::atomic<int> reads{0};

private:
    std::vector<std::thread> threads;
};
} // namespace

class CalculationTests : public QObject {
    Q_OBJECT
    ParameterTestIsolation m_parameterIsolation;
private slots:
    void init() { m_parameterIsolation.reset(); }

    void calibrationFormulaAndSelection();
    void calibrationRejectsInvalidInputs();
    void opaqueAlgorithmData();
    void workerTraversesWholeModel();
    void failuresPreservePreviousResult();
    void defaultHyyPipeline();
    void udpToResultWithoutMainDispatch();
    void concurrentReadersAndShutdown();
};

void CalculationTests::calibrationFormulaAndSelection() {
    QObject owner;
    auto *calibration = new StrainCalibration(&owner);
    QCOMPARE(calibration->parent(), &owner);
    CalibrationStrainData parameters;
    parameters.originalWavelength = {1000.0, 2000.0, 3000.0};
    parameters.calibrationFactor = 2.0;
    parameters.A = 4.0;
    parameters.B = 3.0;
    CalibrationChannelData channels;
    channels.channelIndices = {2, 0};
    channels.calibrationFactors = {2.0, 3.0};
    calibration->setCalibrationStrainData(parameters);
    calibration->setCalibrationChannelData(channels);
    QVERIFY(calibration->checkPara());
    const auto strains = calibration->applyCalibration({1002, 2004, 3006});
    QVERIFY(strains == Strain({4, 5, 6}));
    QVERIFY(calibration->rearrangeCalibration(strains) == Strain({12, 12}));
    calibration->initializeCalibration();
    QVERIFY(!calibration->checkPara());
    QCOMPARE(calibration->getCalibrationStrainData().calibrationFactor, 0.00075);
    QCOMPARE(calibration->getCalibrationStrainData().A, 0.0);
}

void CalculationTests::calibrationRejectsInvalidInputs() {
    StrainCalibration calibration;
    QSignalSpy errors(&calibration, &StrainCalibration::errorOccurred);
    QVERIFY(!calibration.checkPara());
    auto config = settings();
    calibration.setCalibrationStrainData(config.strainCalibration);
    calibration.setCalibrationChannelData(config.channelCalibration);
    QVERIFY(calibration.applyCalibration({1001}).empty());
    QVERIFY(calibration.rearrangeCalibration({1}).empty());
    Wavelength values(12, 1000);
    values[0] = std::numeric_limits<double>::quiet_NaN();
    QVERIFY(calibration.applyCalibration(values).empty());
    values[0] = std::numeric_limits<double>::infinity();
    QVERIFY(calibration.applyCalibration(values).empty());
    for (int index : {-1, 12}) {
        auto bad = config.channelCalibration;
        bad.channelIndices[0] = index;
        calibration.setCalibrationChannelData(bad);
        QVERIFY(!calibration.checkPara());
    }
    calibration.setCalibrationChannelData(config.channelCalibration);
    for (double value :
         {0.0, std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN()}) {
        auto bad = config.strainCalibration;
        bad.A = value;
        calibration.setCalibrationStrainData(bad);
        QVERIFY(!calibration.checkPara());
    }
    auto bad = config.strainCalibration;
    bad.originalWavelength[0] = -1;
    calibration.setCalibrationStrainData(bad);
    QVERIFY(!calibration.checkPara());
    QVERIFY(!errors.isEmpty());
}

void CalculationTests::opaqueAlgorithmData() {
    Trace trace;
    CalculationControl control(core::FrameRepositoryPtr(), nullptr,
                               std::bind(translationFactory, &trace));
    QSignalSpy configured(&control, &CalculationControl::configurationAccepted);
    QSignalSpy results(&control, &CalculationControl::resultReady);
    QSignalSpy errors(&control, &CalculationControl::errorOccurred);
    auto configuration = settings();
    configuration.channelCalibration.channelIndices = {0, 2, 1};
    configuration.channelCalibration.calibrationFactors = {1, 1, 1};
    control.setConfiguration(configuration);
    control.setModel(model());
    QTRY_COMPARE(configured.count(), 1);
    QCOMPARE(trace.solveCalls.load(), 0); // Configuration does not trial-run the algorithm.
    QCOMPARE(trace.pointCalls.load(), 0);
    control.submitFrame(frame());
    QTRY_COMPARE(results.count(), 1);
    const auto result = control.dataModel()->latest();
    QCOMPARE(result->strains.size(), 3);
    QCOMPARE(result->nodeDofs.size(), 1); // Opaque algorithm-specific intermediate data.
    QCOMPARE(result->deformedModel.value(0)[0], 1.0);
    QCOMPARE(trace.solveCalls.load(), 1);
    QCOMPARE(errors.count(), 0);
}

void CalculationTests::workerTraversesWholeModel() {
    Trace trace;
    CalculationControl control(core::FrameRepositoryPtr(), nullptr,
                               std::bind(translationFactory, &trace));
    QSignalSpy accepted(&control, &CalculationControl::modelAccepted);
    QSignalSpy results(&control, &CalculationControl::resultReady);
    QSignalSpy errors(&control, &CalculationControl::errorOccurred);
    auto geometry = model();
    const auto original = geometry;
    control.setConfiguration(settings());
    control.setRawPointMap(geometry);
    geometry.clear(); // Detaches: queued geometry must remain valid.
    QTRY_COMPARE(accepted.count(), 1);
    const int previousCalls = trace.pointCalls.load();
    QCOMPARE(previousCalls, 0); // Model validation must not invoke interpolation.
    QCOMPARE(trace.solveCalls.load(), 0);

    const auto source = frame();
    control.submitFrame(source);
    QTRY_COMPARE(results.count(), 1);
    QCOMPARE(trace.pointCalls.load() - previousCalls, original.size());
    QCOMPARE(trace.solveCalls.load(), 1);

    QVERIFY(trace.factoryThread.load() != QThread::currentThread());
    QCOMPARE(trace.factoryThread.load(), trace.solveThread.load());
    const auto result = control.dataModel()->latest();
    QCOMPARE(result->displacementMap.size(), original.size());
    QVERIFY(result->sourceFrame == source);
    for (auto point = original.constBegin(); point != original.constEnd(); ++point) {
        QVERIFY(result->displacementMap.value(point.key()) == QVector<double>({1, 2, 3}));
        const auto updated = result->deformedModel.value(point.key());
        QCOMPARE(updated[0], point.value()[0] + 1);
        QCOMPARE(updated[1], point.value()[1] + 2);
        QCOMPARE(updated[2], point.value()[2] + 3);
    }
    control.submitFrame(frame(1002, 2));
    QTRY_COMPARE(results.count(), 2);
    QCOMPARE(control.dataModel()->latest()->deformedModel.value(0)[0], 2.0);
    QCOMPARE(result->deformedModel.value(0)[0], 1.0);
    QCOMPARE(errors.count(), 0);
}

void CalculationTests::failuresPreservePreviousResult() {
    Trace trace;
    CalculationControl control(core::FrameRepositoryPtr(), nullptr,
                               std::bind(translationFactory, &trace));
    QSignalSpy results(&control, &CalculationControl::resultReady);
    QSignalSpy errors(&control, &CalculationControl::errorOccurred);
    control.submitFrame(frame());
    QTRY_COMPARE(errors.count(), 1);
    control.setConfiguration(settings());
    control.setModel(model());
    control.submitFrame(frame());
    QTRY_COMPARE(results.count(), 1);
    const auto previous = control.dataModel()->latest();
    control.submitFrame(frame(1003, 3)); // Fails partway through traversal.
    control.submitFrame(frame(std::numeric_limits<float>::quiet_NaN(), 4));
    control.submitFrame(core::FrameSnapshotPtr());
    auto invalidModel = model();
    invalidModel[0] = {1, 2};
    control.setRawPointMap(invalidModel);
    auto invalidSettings = settings();
    invalidSettings.strainCalibration.A = 0;
    control.setConfiguration(invalidSettings);
    QTRY_COMPARE(errors.count(), 6);
    QVERIFY(control.dataModel()->latest() == previous);
    control.submitFrame(frame(1002, 5));
    QTRY_COMPARE(results.count(), 2);
    QCOMPARE(control.dataModel()->latest()->modelRevision, quint64(1));
    QCOMPARE(control.dataModel()->latest()->configurationRevision, quint64(1));
}

void CalculationTests::defaultHyyPipeline() {
    CalculationControl control;
    QSignalSpy results(&control, &CalculationControl::resultReady);
    QSignalSpy errors(&control, &CalculationControl::errorOccurred);
    auto geometry = model();
    control.setConfiguration(settings());
    control.setModel(geometry);
    control.submitFrame(frame(1000));
    QTRY_COMPARE(results.count(), 1);
    QVERIFY(control.dataModel()->latest()->deformedModel == geometry);
    control.submitFrame(frame(1001, 2));
    QTRY_COMPARE(results.count(), 2);
    const auto result = control.dataModel()->latest();
    QCOMPARE(result->displacementMap.size(), geometry.size());
    for (const auto &point : result->deformedModel)
        for (double value : point)
            QVERIFY(std::isfinite(value));
    // Geometry admission is based on the public coordinate contract, not an
    // algorithm's internal element/domain assumptions.
    geometry[0] = {1401, 0, 0};
    control.setModel(geometry);
    control.submitFrame(frame(1001, 3));
    QTRY_COMPARE(results.count(), 3);
    QCOMPARE(control.dataModel()->latest()->modelRevision, quint64(2));
    QCOMPARE(errors.count(), 0);
}

void CalculationTests::udpToResultWithoutMainDispatch() {
    Trace trace;
    protocol::ProtocolControl protocol(nullptr, mpParsers,
                                       std::bind(observedDecoder, &trace, std::placeholders::_1));
    CalculationControl control(protocol.dataModel(), nullptr,
                               std::bind(translationFactory, &trace));
    QSignalSpy models(&control, &CalculationControl::modelAccepted);
    QSignalSpy states(&protocol.communication(), &communication::ConnectControl::stateChanged);
    control.setConfiguration(settings());
    control.setModel(model());
    QTRY_COMPARE(models.count(), 1);
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 port = probe.localPort();
    probe.close();
    communication::UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = port;
    protocol.communication().openUdp(config);
    QTRY_VERIFY(!states.isEmpty());
    QUdpSocket peer;
    const auto packet = wirePacket();
    QCOMPARE(peer.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
    QElapsedTimer deadline;
    deadline.start();
    while (!control.dataModel()->latest() && deadline.elapsed() < 3000)
        QThread::msleep(1); // No UI events: all three processing stages must run independently.
    const auto result = control.dataModel()->latest();
    QVERIFY(result);
    QCOMPARE(result->sourceFrame->source.port, peer.localPort());
    QCOMPARE(result->strains[0], 1.0);
    QCOMPARE(result->deformedModel.value(0)[0], 1.0);
    QVERIFY(trace.solveThread.load() != QThread::currentThread());
    QVERIFY(trace.solveThread.load() != trace.inputThread.load());
    QTest::qWait(35);
    QVERIFY(control.dataModel()->latest() ==
            result); // Do not solve an unchanged snapshot repeatedly.
    auto geometry = model();
    geometry[0][0] = 20;
    // Importers/editors may submit via a queued Qt connection as well.
    QVERIFY(QMetaObject::invokeMethod(&control, "setRawPointMap", Qt::QueuedConnection,
                                      Q_ARG(fosd::core::PointMap, geometry)));
    QTRY_COMPARE(control.dataModel()->latest()->modelRevision, quint64(2));
    QCOMPARE(control.dataModel()->latest()->deformedModel.value(0)[0], 21.0);
    QVERIFY(control.dataModel()->latest()->sourceFrame == result->sourceFrame);
    auto configuration = settings();
    configuration.strainCalibration.A = 2;
    control.setConfiguration(configuration);
    QTRY_COMPARE(control.dataModel()->latest()->configurationRevision, quint64(2));
    QCOMPARE(control.dataModel()->latest()->deformedModel.value(0)[0], 20.5);
}

void CalculationTests::concurrentReadersAndShutdown() {
    core::CalculationRepositoryPtr repository;
    core::CalculationResultPtr first;
    Trace trace;
    {
        CalculationControl control(core::FrameRepositoryPtr(), nullptr,
                                   std::bind(translationFactory, &trace));
        repository = control.dataModel();
        Readers readers(repository);
        QSignalSpy results(&control, &CalculationControl::resultReady);
        control.setConfiguration(settings());
        control.setModel(model());
        control.submitFrame(frame());
        QTRY_COMPARE(results.count(), 1);
        first = repository->latest();
        for (quint64 index = 2; index <= 40; ++index)
            control.submitFrame(frame(1001, index));
        QTRY_COMPARE(results.count(), 40);
        QTRY_VERIFY(readers.reads.load() > 0);
        QVERIFY(!readers.invalid.load());
    }
    QVERIFY(trace.destroyedOnWorker.load());
    QCOMPARE(repository->latest()->revision, quint64(40));
    QCOMPARE(first->revision, quint64(1));
    for (int index = 0; index < 5; ++index) {
        CalculationControl control;
        control.setConfiguration(settings());
        control.setModel(model());
    }
}

QTEST_GUILESS_MAIN(CalculationTests)
#include "calculation_tests.moc"
