#include "communication/ConnectControl.h"
#include "communication/PacketDecoders.h"
#include "protocol/ProtocolControl.h"
#include "protocol/MPWavelengthParser.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QtEndian>
#include <QtTest>
#include <atomic>
#include <cmath>
#include <thread>
#include <type_traits>

using namespace fosd;
using communication::Channel;
using communication::Endpoint;
using protocol::ProtocolControl;

static_assert(std::is_const<core::FrameSnapshotPtr::element_type>::value,
              "Snapshots must be read-only");
static_assert(std::is_const<core::FrameDataPtr::element_type>::value, "Payloads must be read-only");
static_assert(std::is_const<core::FrameRepositoryPtr::element_type>::value,
              "Repositories must be read-only");

namespace {
// Independently specified wire bytes: version 3, type 7, count 2, sequence
// 0x12345678; (channel 1, sensor 2, 1.5f), (channel 16, sensor 255, -2.25f).
QByteArray goldenPacket() {
    return QByteArray::fromHex("5aa5030702007856341201020000c03f10ff000010c0");
}

QByteArray sequencePacket(quint32 sequence) {
    QByteArray bytes = goldenPacket();
    qToLittleEndian<quint32>(sequence, reinterpret_cast<uchar *>(bytes.data() + 6));
    return bytes;
}

core::FrameSnapshotPtr snapshotAt(const QSignalSpy &spy, int index) {
    return qvariant_cast<core::FrameSnapshotPtr>(spy.at(index).at(0));
}

struct TestFrame final : core::FrameData {
    explicit TestFrame(QString value) : text(std::move(value)) {
    }
    QString text;
};
class TestParser final : public protocol::IFrameParser {
public:
    QString protocolId() const override {
        return QStringLiteral("test.text");
    }
    bool matches(const QByteArray &bytes) const override {
        return bytes.startsWith("TEST:");
    }
    core::FrameDataPtr parse(const QByteArray &bytes, QString *) const override {
        return std::make_shared<const TestFrame>(QString::fromUtf8(bytes.mid(5)));
    }
};
class ObservedParser final : public protocol::IFrameParser {
public:
    ObservedParser(std::atomic<bool> *flag, QThread *caller) : m_flag(flag), m_caller(caller) {
    }
    QString protocolId() const override {
        return m_parser.protocolId();
    }
    bool matches(const QByteArray &bytes) const override {
        return m_parser.matches(bytes);
    }
    core::FrameDataPtr parse(const QByteArray &bytes, QString *error) const override {
        m_flag->store(QThread::currentThread() != m_caller);
        return m_parser.parse(bytes, error);
    }

private:
    std::atomic<bool> *m_flag;
    QThread *m_caller;
    protocol::MPWavelengthParser m_parser;
};
protocol::FrameParsers testParsers(std::atomic<bool> *factoryFlag, std::atomic<bool> *parseFlag,
                                   QThread *caller) {
    factoryFlag->store(QThread::currentThread() != caller);
    protocol::FrameParsers parsers;
    parsers.emplace_back(new TestParser);
    parsers.emplace_back(new ObservedParser(parseFlag, caller));
    return parsers;
}

struct ThreadTrace {
    std::atomic<QThread *> decoderThread{nullptr};
    std::atomic<QThread *> parserThread{nullptr};
    std::atomic<QThread *> parseThread{nullptr};
    std::atomic<int> decoderDestroyed{0};
    std::atomic<bool> parserDestroyedOnWorker{false};
};
class TrackedDecoder final : public communication::IPacketDecoder {
public:
    TrackedDecoder(Channel channel, ThreadTrace *trace)
        : IPacketDecoder(channel), m_trace(trace),
          m_decoder(communication::createPacketDecoder(channel)) {
        m_trace->decoderThread.store(QThread::currentThread());
    }
    ~TrackedDecoder() override {
        if (QThread::currentThread() == m_trace->decoderThread.load())
            m_trace->decoderDestroyed.fetch_add(1);
    }
    QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) override {
        return m_decoder->feed(bytes, peer);
    }
    void reset() override {
        m_decoder->reset();
    }

private:
    ThreadTrace *m_trace;
    std::unique_ptr<communication::IPacketDecoder> m_decoder;
};
std::unique_ptr<communication::IPacketDecoder> trackedDecoder(ThreadTrace *trace, Channel channel) {
    return std::unique_ptr<communication::IPacketDecoder>(new TrackedDecoder(channel, trace));
}

class TrackedParser final : public protocol::IFrameParser {
public:
    explicit TrackedParser(ThreadTrace *trace) : m_trace(trace) {
        m_trace->parserThread.store(QThread::currentThread());
    }
    ~TrackedParser() override {
        m_trace->parserDestroyedOnWorker.store(QThread::currentThread() ==
                                               m_trace->parserThread.load());
    }
    QString protocolId() const override {
        return m_parser.protocolId();
    }
    bool matches(const QByteArray &bytes) const override {
        return m_parser.matches(bytes);
    }
    core::FrameDataPtr parse(const QByteArray &bytes, QString *error) const override {
        m_trace->parseThread.store(QThread::currentThread());
        return m_parser.parse(bytes, error);
    }

private:
    ThreadTrace *m_trace;
    protocol::MPWavelengthParser m_parser;
};
protocol::FrameParsers trackedParsers(ThreadTrace *trace) {
    protocol::FrameParsers parsers;
    parsers.emplace_back(new TrackedParser(trace));
    return parsers;
}

void readSnapshots(core::FrameRepositoryPtr repository, std::atomic<bool> *stop,
                   std::atomic<bool> *invalid, std::atomic<int> *reads) {
    quint64 previous = 0;
    while (!stop->load()) {
        const core::FrameSnapshotPtr snapshot = repository->latest();
        if (!snapshot) {
            std::this_thread::yield();
            continue;
        }
        const auto data = std::dynamic_pointer_cast<const core::WavelengthFrame>(snapshot->data);
        if (!data || snapshot->revision < previous || data->sequence != snapshot->revision ||
            data->recordCount != data->records.size() || data->records.at(0).wavelength != 1.5f)
            invalid->store(true);
        previous = snapshot->revision;
        reads->fetch_add(1);
    }
}

// Joining in the destructor also handles early QtTest assertion returns safely.
class Readers {
public:
    explicit Readers(core::FrameRepositoryPtr repository) {
        for (int index = 0; index < 4; ++index)
            m_threads.emplace_back(readSnapshots, repository, &stop, &invalid, &reads);
    }
    ~Readers() {
        stop.store(true);
        for (std::thread &thread : m_threads)
            thread.join();
    }
    std::atomic<bool> stop{false};
    std::atomic<bool> invalid{false};
    std::atomic<int> reads{0};

private:
    std::vector<std::thread> m_threads;
};
} // namespace

class ProtocolTests : public QObject {
    Q_OBJECT
private slots:
    void defaultParsers() {
        auto parsers = protocol::createFrameParsers();
        QStringList ids;
        for (const auto &parser : parsers)
            ids.append(parser->protocolId());
        QVERIFY(ids.contains("mp.wavelength"));
        QVERIFY(ids.contains("jcs.wavelength"));
    }
    void parseFieldsAndBoundaries();
    void rejectMalformedFrames();
    void dispatchAndThreadOwnership();
    void failuresPreserveSnapshot();
    void concurrentReadersAndLifetime();
    void udpEndToEnd();
    void tcpFragmentsEndToEnd();
    void sharedThreadWithoutMainDispatchAndShutdown();
    void hostedCommunicationLifecycle();
};

void ProtocolTests::parseFieldsAndBoundaries() {
    protocol::MPWavelengthParser parser;
    QString error = QStringLiteral("old error");
    const auto frame = std::dynamic_pointer_cast<const core::WavelengthFrame>(
        parser.parse(goldenPacket(), &error));
    QVERIFY(frame);
    QVERIFY(error.isEmpty());
    QCOMPARE(frame->version, quint8(3));
    QCOMPARE(frame->dataType, quint8(7));
    QCOMPARE(frame->recordCount, quint16(2));
    QCOMPARE(frame->sequence, quint32(0x12345678));
    QCOMPARE(frame->records.size(), 2);
    QCOMPARE(frame->records.at(0).channel, quint8(1));
    QCOMPARE(frame->records.at(0).sensor, quint8(2));
    QCOMPARE(frame->records.at(0).wavelength, 1.5f);
    QCOMPARE(frame->records.at(1).channel, quint8(16));
    QCOMPARE(frame->records.at(1).sensor, quint8(255));
    QCOMPARE(frame->records.at(1).wavelength, -2.25f);
    const auto empty = std::dynamic_pointer_cast<const core::WavelengthFrame>(
        parser.parse(QByteArray::fromHex("5aa5ffff0000ffffffff")));
    QVERIFY(empty);
    QCOMPARE(empty->sequence, quint32(0xffffffff));
    QVERIFY(empty->records.isEmpty());

    QByteArray maximum(10 + 65535 * 6, '\0');
    maximum.replace(0, 10, QByteArray::fromHex("5aa50101ffff00000000"));
    const auto maxFrame =
        std::dynamic_pointer_cast<const core::WavelengthFrame>(parser.parse(maximum));
    QVERIFY(maxFrame);
    QCOMPARE(maxFrame->records.size(), 65535);
    QByteArray special = goldenPacket();
    special.replace(12, 4, QByteArray::fromHex("0000807f"));
    special.replace(18, 4, QByteArray::fromHex("0000c07f"));
    const auto specialFrame =
        std::dynamic_pointer_cast<const core::WavelengthFrame>(parser.parse(special));
    QVERIFY(specialFrame);
    QVERIFY(std::isinf(specialFrame->records.at(0).wavelength));
    QVERIFY(std::isnan(specialFrame->records.at(1).wavelength));
}

void ProtocolTests::rejectMalformedFrames() {
    protocol::MPWavelengthParser parser;
    QVector<QByteArray> invalid = {
        {}, goldenPacket().left(9), goldenPacket().left(21), goldenPacket() + 'x'};
    QByteArray wrongMagic = goldenPacket();
    wrongMagic[0] = 0;
    invalid.append(wrongMagic);
    QByteArray wrongCount = goldenPacket();
    wrongCount[4] = char(0xff);
    wrongCount[5] = char(0xff);
    invalid.append(wrongCount);
    for (const QByteArray &packet : invalid) {
        QString error;
        QVERIFY(!parser.parse(packet, &error));
        QVERIFY(!error.isEmpty());
    }
}

void ProtocolTests::dispatchAndThreadOwnership() {
    std::atomic<bool> factoryOnWorker(false), parseOnWorker(false);
    ProtocolControl control(nullptr, std::bind(testParsers, &factoryOnWorker, &parseOnWorker,
                                               QCoreApplication::instance()->thread()));
    QSignalSpy frames(&control, &ProtocolControl::frameParsed);
    QSignalSpy errors(&control, &ProtocolControl::errorOccurred);
    control.parseFrame(Channel::Serial, "TEST:hello");
    control.parseFrame(Channel::Tcp, goldenPacket());
    QTRY_COMPARE(frames.count(), 2);
    QVERIFY(factoryOnWorker.load());
    QVERIFY(parseOnWorker.load());
    QCOMPARE(snapshotAt(frames, 0)->protocolId, QStringLiteral("test.text"));
    QCOMPARE(std::dynamic_pointer_cast<const TestFrame>(snapshotAt(frames, 0)->data)->text,
             QStringLiteral("hello"));
    QCOMPARE(snapshotAt(frames, 1)->protocolId, QStringLiteral("mp.wavelength"));
    QCOMPARE(errors.count(), 0);
}

void ProtocolTests::failuresPreserveSnapshot() {
    ProtocolControl control;
    QSignalSpy frames(&control, &ProtocolControl::frameParsed);
    QSignalSpy errors(&control, &ProtocolControl::errorOccurred);
    QVERIFY(!control.dataModel()->latest());
    control.parseFrame(Channel::Udp, goldenPacket());
    QTRY_COMPARE(frames.count(), 1);
    const core::FrameSnapshotPtr previous = control.dataModel()->latest();
    Endpoint source;
    source.address = QHostAddress::LocalHost;
    source.port = 1234;
    control.parseFrame(Channel::Tcp, goldenPacket().left(9), source);
    control.parseFrame(Channel::Tcp, "unknown", source);
    QTRY_COMPARE(errors.count(), 2);
    QCOMPARE(frames.count(), 1);
    QVERIFY(control.dataModel()->latest() == previous);
    const auto error = qvariant_cast<protocol::ProtocolError>(errors.first().at(0));
    QCOMPARE(error.protocolId, QStringLiteral("mp.wavelength"));
    QCOMPARE(error.channel, Channel::Tcp);
    QCOMPARE(error.peer.port, source.port);
    QVERIFY(!error.message.isEmpty());
    QCOMPARE(qvariant_cast<protocol::ProtocolError>(errors.last().at(0)).protocolId, QString());
}

void ProtocolTests::concurrentReadersAndLifetime() {
    core::FrameRepositoryPtr repository;
    core::FrameSnapshotPtr first;
    {
        ProtocolControl control;
        repository = control.dataModel();
        Readers readers(repository);
        QSignalSpy frames(&control, &ProtocolControl::frameParsed);
        control.parseFrame(Channel::Udp, sequencePacket(1));
        QTRY_COMPARE(frames.count(), 1);
        first = snapshotAt(frames, 0);
        for (quint32 sequence = 2; sequence <= 200; ++sequence)
            control.parseFrame(Channel::Udp, sequencePacket(sequence));
        QTRY_COMPARE(frames.count(), 200);
        QTRY_VERIFY(readers.reads.load() > 0);
        QVERIFY(!readers.invalid.load());
        QCOMPARE(repository->latest()->revision, quint64(200));
        QCOMPARE(std::dynamic_pointer_cast<const core::WavelengthFrame>(first->data)->sequence,
                 quint32(1));
        for (int index = 0; index < frames.count(); ++index)
            QCOMPARE(snapshotAt(frames, index)->revision, quint64(index + 1));
    }
    QCOMPARE(repository->latest()->revision, quint64(200));
    QCOMPARE(first->revision, quint64(1));
}

void ProtocolTests::udpEndToEnd() {
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 port = probe.localPort();
    probe.close();
    ProtocolControl protocol;
    communication::ConnectControl &communication = protocol.communication();
    QSignalSpy states(&communication, &communication::ConnectControl::stateChanged);
    QSignalSpy frames(&protocol, &ProtocolControl::frameParsed);
    QSignalSpy errors(&protocol, &ProtocolControl::errorOccurred);
    QSignalSpy decoderErrors(&communication, &communication::ConnectControl::decoderErrorOccurred);
    communication::UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = port;
    communication.openUdp(config);
    QTRY_VERIFY(!states.isEmpty());
    QUdpSocket peer;
    QVERIFY(peer.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    QCOMPARE(peer.writeDatagram("bad", 3, QHostAddress::LocalHost, port), qint64(3));
    const QByteArray first = sequencePacket(1), second = sequencePacket(2);
    QCOMPARE(peer.writeDatagram(first, QHostAddress::LocalHost, port), qint64(first.size()));
    QCOMPARE(peer.writeDatagram(second, QHostAddress::LocalHost, port), qint64(second.size()));
    QTRY_COMPARE(frames.count(), 2);
    QTRY_COMPARE(decoderErrors.count(), 1);
    QCOMPARE(errors.count(), 0);
    const auto snapshot = snapshotAt(frames, 0);
    QCOMPARE(snapshot->source.transport, core::TransportKind::Udp);
    QCOMPARE(snapshot->source.port, peer.localPort());
    QCOMPARE(snapshot->source.address, QStringLiteral("127.0.0.1"));
    QVERIFY(snapshot->receivedAtUtc.isValid());
    QCOMPARE(std::dynamic_pointer_cast<const core::WavelengthFrame>(snapshot->data)->sequence,
             quint32(1));
    QCOMPARE(protocol.dataModel()->latest()->revision, quint64(2));
}

void ProtocolTests::tcpFragmentsEndToEnd() {
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));
    ProtocolControl protocol;
    communication::ConnectControl &communication = protocol.communication();
    QSignalSpy raw(&communication, &communication::ConnectControl::rawDataReceived);
    QSignalSpy frames(&protocol, &ProtocolControl::frameParsed);
    QSignalSpy errors(&protocol, &ProtocolControl::errorOccurred);
    communication::TcpConfig config;
    config.port = server.serverPort();
    communication.openTcp(config);
    QTRY_VERIFY(server.hasPendingConnections());
    QTcpSocket *peer = server.nextPendingConnection();
    QVERIFY(peer);
    const QByteArray first = sequencePacket(1), second = sequencePacket(2);
    QCOMPARE(peer->write(first.left(1)), qint64(1));
    QTRY_COMPARE(raw.count(), 1);
    QCOMPARE(frames.count(), 0);
    const QByteArray rest = first.mid(1) + second;
    QCOMPARE(peer->write(rest), qint64(rest.size()));
    QTRY_COMPARE(frames.count(), 2);
    QCOMPARE(errors.count(), 0);
    QCOMPARE(snapshotAt(frames, 0)->source.transport, core::TransportKind::Tcp);
    QCOMPARE(snapshotAt(frames, 0)->source.port, server.serverPort());
    QCOMPARE(std::dynamic_pointer_cast<const core::WavelengthFrame>(snapshotAt(frames, 1)->data)
                 ->sequence,
             quint32(2));
}

void ProtocolTests::sharedThreadWithoutMainDispatchAndShutdown() {
    QUdpSocket probe;
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
    const quint16 port = probe.localPort();
    probe.close();
    ThreadTrace trace;
    core::FrameRepositoryPtr repository;
    {
        ProtocolControl control(nullptr, std::bind(trackedParsers, &trace),
                                std::bind(trackedDecoder, &trace, std::placeholders::_1));
        repository = control.dataModel();
        auto &communication = control.communication();
        QSignalSpy states(&communication, &communication::ConnectControl::stateChanged);
        communication::UdpConfig config;
        config.bindAddress = QHostAddress::LocalHost;
        config.localPort = port;
        communication.openUdp(config);
        QTRY_VERIFY(!states.isEmpty());
        QCOMPARE(trace.decoderThread.load(), trace.parserThread.load());
        QVERIFY(trace.parserThread.load() != QThread::currentThread());
        QUdpSocket peer;
        const QByteArray packet = goldenPacket();
        QCOMPARE(peer.writeDatagram(packet, QHostAddress::LocalHost, port), qint64(packet.size()));
        // Deliberately do not process main-thread events: socket -> decoder ->
        // parser -> repository must complete entirely on the shared thread.
        QElapsedTimer deadline;
        deadline.start();
        while (!repository->latest() && deadline.elapsed() < 3000)
            QThread::msleep(1);
        QVERIFY(repository->latest());
        QCOMPARE(trace.parseThread.load(), trace.decoderThread.load());
    }
    QCOMPARE(trace.decoderDestroyed.load(), 3);
    QVERIFY(trace.parserDestroyedOnWorker.load());
    QVERIFY(repository->latest());
    // Destruction closed the socket before returning to the caller.
    QVERIFY(probe.bind(QHostAddress(QHostAddress::LocalHost), port));
}

void ProtocolTests::hostedCommunicationLifecycle() {
    QThread host;
    {
        communication::ConnectControl control(host);
        QVERIFY(!host.isRunning());
        QSignalSpy states(&control, &communication::ConnectControl::stateChanged);
        host.start();
        communication::UdpConfig config;
        config.bindAddress = QHostAddress::LocalHost;
        control.openUdp(config);
        QTRY_VERIFY(!states.isEmpty());
    }
    QVERIFY(host.isRunning()); // Hosted control does not own thread termination.
    host.quit();
    QVERIFY(host.wait(3000));
    // Also cover destruction before a hosted thread has ever started.
    communication::ConnectControl unstarted(host);
}

QTEST_GUILESS_MAIN(ProtocolTests)
#include "protocol_tests.moc"
