#include "protocol/ProtocolControl.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QTemporaryDir>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <QUdpSocket>
#include <QtEndian>
#include <cmath>
#include <cstring>
#include <functional>
#include <limits>
#include <stdexcept>

using namespace fosd;
namespace {
// IPv4 UDP payload limit: floor((65507 - 10) / 6) records.
const int maxUdpRecords = 10916;
void require(bool ok, const QString &message) {
    if (!ok)
        throw std::runtime_error(message.toStdString());
}
quint32 floatBits(float value) {
    quint32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}
QByteArray encode(const core::WavelengthFrame &frame) {
    require(frame.records.size() <= maxUdpRecords, "Too many records for an IPv4 UDP datagram");
    QByteArray bytes(10 + frame.records.size() * 6, '\0');
    auto *data = reinterpret_cast<uchar *>(bytes.data());
    qToLittleEndian<quint16>(0xa55a, data);
    data[2] = frame.version;
    data[3] = frame.dataType;
    qToLittleEndian<quint16>(quint16(frame.records.size()), data + 4);
    qToLittleEndian<quint32>(frame.sequence, data + 6);
    for (int i = 0; i < frame.records.size(); ++i) {
        const auto &record = frame.records[i];
        data[10 + i * 6] = record.channel;
        data[11 + i * 6] = record.sensor;
        qToLittleEndian<quint32>(floatBits(record.wavelength), data + 12 + i * 6);
    }
    return bytes;
}
core::WavelengthFrame sample(quint32 sequence, int channels, int sensors) {
    core::WavelengthFrame frame;
    frame.version = 1;
    frame.dataType = 1;
    frame.sequence = sequence;
    for (int channel = 1; channel <= channels; ++channel) {
        for (int sensor = 1; sensor <= sensors; ++sensor) {
            core::WavelengthRecord record;
            record.channel = quint8(channel);
            record.sensor = quint8(sensor);
            const int index = (channel - 1) * sensors + sensor - 1;
            record.wavelength = float(1550.0 + index * 0.5 + 0.02 * std::sin(double(sequence) * 0.1));
            frame.records.append(record);
        }
    }
    frame.recordCount = quint16(frame.records.size());
    return frame;
}
bool waitUntil(const std::function<bool()> &ready, int timeoutMs = 3000) {
    QElapsedTimer clock;
    clock.start();
    while (!ready() && clock.elapsed() < timeoutMs) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        QThread::msleep(1);
    }
    return ready();
}
// Self-test configuration never touches the application's persisted parameters.
struct ConfigIsolation {
    QTemporaryDir directory;
    bool hadOverride = qEnvironmentVariableIsSet("FOSD_CONFIG_FILE");
    QByteArray previous = qgetenv("FOSD_CONFIG_FILE");
    ConfigIsolation() {
        require(directory.isValid(), "Cannot create isolated configuration directory");
        qputenv("FOSD_CONFIG_FILE", directory.filePath("parameters.ini").toUtf8());
    }
    ~ConfigIsolation() {
        if (hadOverride) qputenv("FOSD_CONFIG_FILE", previous);
        else qunsetenv("FOSD_CONFIG_FILE");
    }
};
int selfTest() {
    ConfigIsolation isolated;
    protocol::ProtocolControl receiver; // Default production decoder and parser factories.
    auto &transport = receiver.communication();
    QUdpSocket reservation;
    require(reservation.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)), reservation.errorString());
    const quint16 port = reservation.localPort();
    reservation.close();
    bool ready = false;
    QString transportError;
    int rawCount = 0, parsedCount = 0, decodeErrors = 0, parseErrors = 0;
    core::FrameSnapshotPtr latest;
    QObject::connect(&transport, &communication::ConnectControl::stateChanged,
                     [&ready](communication::Channel, communication::ConnectionState state) {
                         ready = state == communication::ConnectionState::Ready;
                     });
    QObject::connect(&transport, &communication::ConnectControl::errorOccurred,
                     [&transportError](const communication::CommunicationError &error) {
                         transportError = error.message;
                     });
    QObject::connect(&transport, &communication::ConnectControl::rawDataReceived,
                     [&rawCount](communication::Channel, const QByteArray &, const communication::Endpoint &) { ++rawCount; });
    QObject::connect(&transport, &communication::ConnectControl::decoderErrorOccurred,
                     [&decodeErrors](const communication::DecoderError &) { ++decodeErrors; });
    QObject::connect(&receiver, &protocol::ProtocolControl::errorOccurred,
                     [&parseErrors](const protocol::ProtocolError &) { ++parseErrors; });
    QObject::connect(&receiver, &protocol::ProtocolControl::frameParsed,
                     [&](const core::FrameSnapshotPtr &frame) { latest = frame; ++parsedCount; });
    communication::UdpConfig config;
    config.bindAddress = QHostAddress::LocalHost;
    config.localPort = port;
    transport.openUdp(config);
    require(waitUntil([&] { return ready || !transportError.isEmpty(); }) && ready,
            "Cannot open loopback UDP receiver: " + transportError);
    QUdpSocket sender;
    require(sender.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)), sender.errorString());
    QTextStream out(stdout);
    out << "Loopback 127.0.0.1:" << sender.localPort() << " -> 127.0.0.1:" << port << "\n";
    auto send = [&](const QByteArray &bytes) {
        require(sender.writeDatagram(bytes, QHostAddress::LocalHost, port) == bytes.size(), sender.errorString());
    };
    auto check = [&](const core::WavelengthFrame &expected, const QByteArray &bytes) {
        const int before = parsedCount;
        send(bytes);
        require(waitUntil([&] { return parsedCount > before || !transportError.isEmpty(); }), "Timed out waiting for parsed frame");
        require(parsedCount == before + 1 && latest, "Unexpected parsed frame count");
        require(latest->protocolId == "mp.wavelength" && latest->source.transport == core::TransportKind::Udp,
                "Wrong protocol or transport");
        require(QHostAddress(latest->source.address).isLoopback() && latest->source.port == sender.localPort(), "Wrong source endpoint");
        const auto actual = std::dynamic_pointer_cast<const core::WavelengthFrame>(latest->data);
        require(bool(actual), "Wrong parsed frame type");
        require(actual->version == expected.version && actual->dataType == expected.dataType &&
                actual->sequence == expected.sequence && actual->recordCount == expected.records.size() &&
                actual->records.size() == expected.records.size(), "Header or record count mismatch");
        for (int i = 0; i < expected.records.size(); ++i) {
            const auto &a = actual->records[i];
            const auto &e = expected.records[i];
            require(a.channel == e.channel && a.sensor == e.sensor && floatBits(a.wavelength) == floatBits(e.wavelength),
                    QString("Record %1 mismatch").arg(i));
        }
        require(receiver.dataModel()->latest()->revision == latest->revision, "Repository was not updated");
    };
    core::WavelengthFrame golden;
    golden.version = 1; golden.dataType = 1; golden.sequence = 0x12345678;
    core::WavelengthRecord a, b;
    a.channel = 1; a.sensor = 2; a.wavelength = 1.5f;
    b.channel = 16; b.sensor = 255; b.wavelength = -2.25f;
    golden.records = {a, b};
    // Independent literal fixture catches encoder/decoder agreeing on the wrong byte order.
    const auto literal = QByteArray::fromHex("5aa5010102007856341201020000c03f10ff000010c0");
    require(encode(golden) == literal, "Encoder differs from independent little-endian fixture");
    check(golden, literal);
    out << "PASS golden packet: sequence=305419896; (1,2)=1.5; (16,255)=-2.25\n";
    core::WavelengthFrame empty;
    empty.version = 255; empty.dataType = 255; empty.sequence = 0xffffffff;
    check(empty, encode(empty));
    auto boundary = golden;
    boundary.version = 0; boundary.dataType = 0; boundary.sequence = 0;
    boundary.records[0].channel = 0; boundary.records[0].sensor = 0;
    boundary.records[1].channel = 255; boundary.records[1].sensor = 255;
    check(boundary, encode(boundary));
    auto special = golden;
    special.records.clear();
    for (float value : {0.f, -0.f, std::numeric_limits<float>::denorm_min(),
                        std::numeric_limits<float>::max(), std::numeric_limits<float>::infinity(),
                        -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()}) {
        a.wavelength = value; special.records.append(a);
    }
    check(special, encode(special));
    auto large = sample(10, 1, 1);
    large.records.fill(large.records[0], maxUdpRecords);
    check(large, encode(large));
    out << "PASS empty/boundary fields, exact float bits, maximum UDP packet (65506 bytes)\n";
    for (int i = 0; i < 100; ++i) {
        const auto frame = sample(quint32(i + 100), 3, 4);
        check(frame, encode(frame));
    }
    out << "PASS 100 successive 12-record frames; every header and record matched\n";
    auto wrongMagic = literal; wrongMagic[0] = '\0';
    auto wrongCount = literal; wrongCount[4] = 3;
    const QVector<QByteArray> invalid{wrongMagic, literal.left(9), literal.left(literal.size() - 1),
        literal + 'x', wrongCount, literal + literal, literal.left(11), literal.mid(11)};
    for (const auto &bytes : invalid) {
        const auto previous = receiver.dataModel()->latest();
        const int errors = decodeErrors, raws = rawCount, frames = parsedCount;
        send(bytes);
        require(waitUntil([&] { return decodeErrors > errors && rawCount > raws; }), "Malformed packet was not rejected");
        require(parsedCount == frames && receiver.dataModel()->latest() == previous, "Malformed packet replaced valid snapshot");
    }
    require(rawCount == 113 && parsedCount == 105 && decodeErrors == 8 && parseErrors == 0 && transportError.isEmpty(),
            "Unexpected final frame/error counters");
    out << "PASS 8 malformed datagrams rejected; latest valid snapshot preserved\n"
        << "RESULT PASS: received=113 parsed=105 rejected=8 parserErrors=0\n";
    return 0;
}
} // namespace

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("MPWavelengthSimulator");
    QCommandLineParser cli;
    cli.setApplicationDescription("Send MP wavelength UDP packets to IPv4 loopback, or verify the production decoder/parser.");
    cli.addHelpOption();
    cli.addOption({"self-test", "Verify production UDP -> decoder -> MPWavelengthParser using an isolated receiver."});
    cli.addOption({"port", "Destination port on 127.0.0.1.", "port", "9000"});
    cli.addOption({"count", "Number of frames; 0 sends until Ctrl+C.", "count", "100"});
    cli.addOption({"interval", "Send interval in milliseconds (minimum 1).", "ms", "20"});
    cli.addOption({"channels", "Sensor channels (1..255).", "channels", "3"});
    cli.addOption({"sensors", "Sensors per channel (1..255).", "sensors", "4"});
    cli.addOption({"sequence", "Initial uint32 frame sequence.", "sequence", "1"});
    cli.process(app);
    try {
        if (cli.isSet("self-test")) return selfTest();
        auto integer = [&](const QString &name, quint64 min, quint64 max) {
            bool ok = false;
            const auto value = cli.value(name).toULongLong(&ok);
            require(ok && value >= min && value <= max, "Invalid --" + name);
            return value;
        };
        const auto port = quint16(integer("port", 1, 65535));
        const auto count = integer("count", 0, 0xffffffff);
        const int interval = int(integer("interval", 1, 3600000));
        const int channels = int(integer("channels", 1, 255));
        const int sensors = int(integer("sensors", 1, 255));
        quint32 sequence = quint32(integer("sequence", 0, 0xffffffff));
        require(channels * sensors <= maxUdpRecords, "channels * sensors exceeds the UDP payload limit (10916)");
        QUdpSocket sender;
        require(sender.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)), sender.errorString());
        QTextStream out(stdout);
        out << "Sending MP packets to 127.0.0.1:" << port << "; records/frame=" << channels * sensors
            << "; interval=" << interval << "ms. Ctrl+C to stop.\n";
        out.flush();
        quint64 sent = 0;
        QTimer timer;
        QObject::connect(&timer, &QTimer::timeout, &app, [&] {
            const auto bytes = encode(sample(sequence, channels, sensors));
            if (sender.writeDatagram(bytes, QHostAddress::LocalHost, port) != bytes.size()) {
                QTextStream(stderr) << "SEND FAILED: " << sender.errorString() << "\n";
                app.exit(1); return;
            }
            ++sent;
            if (sent == 1 || sent % 50 == 0 || sent == count) {
                out << "sent=" << sent << " sequence=" << sequence << " bytes=" << bytes.size() << "\n";
                out.flush();
            }
            ++sequence;
            if (count && sent >= count) app.quit();
        });
        timer.start(interval);
        return app.exec();
    } catch (const std::exception &error) {
        QTextStream(stderr) << "FAIL: " << error.what() << "\n";
        return 1;
    }
}
