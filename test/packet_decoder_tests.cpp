#include "communication/PacketDecoders.h"

#include <QtEndian>
#include <QtTest>

using namespace fosd::communication;

namespace {
int customFrameSize(const QByteArray &bytes) {
    return bytes.size() < 3 ? 0 : int(uchar(bytes.at(2)));
}

bool customFrameValid(const QByteArray &bytes) {
    return bytes.endsWith('!');
}

int incompleteFrameSize(const QByteArray &) {
    return 0;
}

QByteArray frame(quint16 count = 1) {
    QByteArray bytes(10 + count * 6, '\0');
    bytes[0] = char(0x5a);
    bytes[1] = char(0xa5);
    bytes[2] = 1;
    qToLittleEndian<quint16>(count, reinterpret_cast<uchar *>(bytes.data() + 4));
    return bytes;
}
} // namespace

class PacketDecoderTests : public QObject {
    Q_OBJECT
private slots:
    void initTestCase();
    void udpValidation();
    void tcpEverySplit();
    void tcpNoiseAndReset();
    void customRulesAndRecovery();
    void serialCommands();
    void serialLimitAndReset();
    void serialCustomTerminator();
    void invalidConfiguration();
    void decoderErrorSignals();
    void jcsChannelPackets();
};

void PacketDecoderTests::initTestCase() {
    qRegisterMetaType<DecoderError>("fosd::communication::DecoderError");
}

void PacketDecoderTests::udpValidation() {
    UdpPacketDecoder decoder;
    const QByteArray packet = frame();
    QCOMPARE(decoder.feed(packet, Endpoint()), QVector<QByteArray>({packet}));
    QVERIFY(decoder.feed({}, Endpoint()).isEmpty());
    QVERIFY(decoder.feed(packet.left(9), Endpoint()).isEmpty());
    QVERIFY(decoder.feed(packet.left(packet.size() - 1), Endpoint()).isEmpty());
    QVERIFY(decoder.feed(packet + 'x', Endpoint()).isEmpty());
    QVERIFY(decoder.feed(packet + packet, Endpoint()).isEmpty());
    QByteArray bad = packet;
    bad[0] = char(0xa5);
    bad[1] = char(0x5a);
    QVERIFY(decoder.feed(bad, Endpoint()).isEmpty());
    // Separate datagrams are never joined.
    QVERIFY(decoder.feed(packet.left(8), Endpoint()).isEmpty());
    QVERIFY(decoder.feed(packet.mid(8), Endpoint()).isEmpty());
    QCOMPARE(decoder.feed(frame(0), Endpoint()), QVector<QByteArray>({frame(0)}));
}

void PacketDecoderTests::tcpEverySplit() {
    const QByteArray a = frame(2), b = frame(0);
    const QByteArray stream = a + b;
    for (int split = 0; split <= stream.size(); ++split) {
        TcpPacketDecoder decoder;
        QVector<QByteArray> packets = decoder.feed(stream.left(split), Endpoint());
        packets += decoder.feed(stream.mid(split), Endpoint());
        QCOMPARE(packets, QVector<QByteArray>({a, b}));
    }
    TcpPacketDecoder decoder;
    QVector<QByteArray> packets;
    for (char byte : stream)
        packets += decoder.feed(QByteArray(1, byte), Endpoint());
    QCOMPARE(packets, QVector<QByteArray>({a, b}));
}

void PacketDecoderTests::tcpNoiseAndReset() {
    TcpPacketDecoder decoder;
    QByteArray packet = frame(2);
    packet.replace(12, 2, QByteArray::fromHex("5aa5"));
    QVERIFY(decoder.feed(QByteArray("noise") + packet.left(1), Endpoint()).isEmpty());
    QCOMPARE(decoder.feed(packet.mid(1) + "junk" + frame(0), Endpoint()),
             QVector<QByteArray>({packet, frame(0)}));
    QVERIFY(decoder.feed(frame().left(8), Endpoint()).isEmpty());
    decoder.reset();
    QVERIFY(decoder.feed(frame().mid(8), Endpoint()).isEmpty());
    QCOMPARE(decoder.feed(frame(), Endpoint()), QVector<QByteArray>({frame()}));
}

void PacketDecoderTests::customRulesAndRecovery() {
    PacketRule rule;
    rule.marker = "XY";
    rule.maxFrameSize = 8;
    rule.frameSize = customFrameSize;
    rule.validate = customFrameValid;
    const QByteArray valid = QByteArray("XY") + char(4) + '!';
    UdpPacketDecoder udp({rule});
    QCOMPARE(udp.feed(valid, Endpoint()), QVector<QByteArray>({valid}));
    QVERIFY(udp.feed(QByteArray("XY") + char(4) + '?', Endpoint()).isEmpty());
    TcpPacketDecoder tcp({rule, Rule::MP_PacketRule()});
    const QByteArray invalid = QByteArray("XY") + char(9) + "XY" + char(4) + '?';
    QCOMPARE(tcp.feed(invalid + valid + frame(), Endpoint()),
             QVector<QByteArray>({valid, frame()}));
    rule.frameSize = incompleteFrameSize;
    TcpPacketDecoder bounded({rule, Rule::MP_PacketRule()});
    QCOMPARE(bounded.feed(QByteArray("XY12345678") + frame(), Endpoint()),
             QVector<QByteArray>({frame()}));
}

void PacketDecoderTests::serialCommands() {
    SerialPacketDecoder decoder;
    QVERIFY(decoder.feed("SET ", Endpoint()).isEmpty());
    QCOMPARE(decoder.feed("1\rGET\r\rtail", Endpoint()),
             QVector<QByteArray>({"SET 1\r", "GET\r", "\r"}));
    QCOMPARE(decoder.feed("\r", Endpoint()), QVector<QByteArray>({"tail\r"}));
    QVERIFY(decoder.feed({}, Endpoint()).isEmpty());
}

void PacketDecoderTests::serialLimitAndReset() {
    SerialPacketDecoder decoder("\r", 4);
    QCOMPARE(decoder.feed("abc\rtoolong\rOK\r", Endpoint()),
             QVector<QByteArray>({"abc\r", "OK\r"}));
    QVERIFY(decoder.feed("12345", Endpoint()).isEmpty());
    QCOMPARE(decoder.feed("tail\rX\r", Endpoint()), QVector<QByteArray>({"X\r"}));
    QVERIFY(decoder.feed("12345", Endpoint()).isEmpty());
    decoder.reset();
    QCOMPARE(decoder.feed("X\r", Endpoint()), QVector<QByteArray>({"X\r"}));
    QVERIFY(decoder.feed("old", Endpoint()).isEmpty());
    decoder.reset();
    QCOMPARE(decoder.feed("\r", Endpoint()), QVector<QByteArray>({"\r"}));
}

void PacketDecoderTests::serialCustomTerminator() {
    SerialPacketDecoder decoder("\r\n", 6);
    QVERIFY(decoder.feed("CMD\r", Endpoint()).isEmpty());
    QCOMPARE(decoder.feed("\nX\r\n", Endpoint()), QVector<QByteArray>({"CMD\r\n", "X\r\n"}));
    QVERIFY(decoder.feed("toolong\r", Endpoint()).isEmpty());
    QCOMPARE(decoder.feed("\nOK\r\n", Endpoint()), QVector<QByteArray>({"OK\r\n"}));
}

void PacketDecoderTests::invalidConfiguration() {
    QVector<QVector<PacketRule>> invalidRules;
    invalidRules.append(QVector<PacketRule>());
    PacketRule emptyMarker = Rule::MP_PacketRule();
    emptyMarker.marker.clear();
    invalidRules.append({emptyMarker});
    PacketRule missingSize = Rule::MP_PacketRule();
    missingSize.frameSize = {};
    invalidRules.append({missingSize});
    PacketRule invalidLimit = Rule::MP_PacketRule();
    invalidLimit.maxFrameSize = invalidLimit.marker.size() - 1;
    invalidRules.append({invalidLimit});
    // Even when another rule is valid, reject the entire invalid configuration.
    invalidRules.append({Rule::MP_PacketRule(), missingSize});
    for (const QVector<PacketRule> &rules : invalidRules) {
        UdpPacketDecoder udp(rules);
        TcpPacketDecoder tcp(rules);
        QSignalSpy udpErrors(&udp, &IPacketDecoder::errorOccurred);
        QSignalSpy tcpErrors(&tcp, &IPacketDecoder::errorOccurred);
        // Initialization happens after observers connect, not in constructors.
        QVERIFY(!udp.initialize());
        QVERIFY(!tcp.initialize());
        QCOMPARE(udpErrors.count(), 1);
        QCOMPARE(tcpErrors.count(), 1);
        const DecoderError udpError = qvariant_cast<DecoderError>(udpErrors.first().at(0));
        const DecoderError tcpError = qvariant_cast<DecoderError>(tcpErrors.first().at(0));
        QCOMPARE(udpError.channel, Channel::Udp);
        QCOMPARE(tcpError.channel, Channel::Tcp);
        QCOMPARE(udpError.operation, QStringLiteral("initialize"));
        QCOMPARE(tcpError.operation, udpError.operation);
        QVERIFY(!udpError.message.isEmpty());
        QCOMPARE(tcpError.message, udpError.message);
        QVERIFY(udp.feed(frame(), Endpoint()).isEmpty());
        QVERIFY(tcp.feed(frame(), Endpoint()).isEmpty());
        tcp.reset();
        QVERIFY(!tcp.initialize());
        QVERIFY(tcp.feed(frame(), Endpoint()).isEmpty());
        QCOMPARE(udpErrors.count(), 1);
        QCOMPARE(tcpErrors.count(), 1);
    }
    SerialPacketDecoder emptyTerminator(QByteArray{}, 1);
    SerialPacketDecoder invalidSerialLimit("\r\n", 1);
    QSignalSpy serialErrors(&emptyTerminator, &IPacketDecoder::errorOccurred);
    QSignalSpy serialLimitErrors(&invalidSerialLimit, &IPacketDecoder::errorOccurred);
    QVERIFY(!emptyTerminator.initialize());
    QVERIFY(!invalidSerialLimit.initialize());
    QCOMPARE(serialErrors.count(), 1);
    QCOMPARE(serialLimitErrors.count(), 1);
    QCOMPARE(qvariant_cast<DecoderError>(serialErrors.first().at(0)).channel, Channel::Serial);
    QVERIFY(emptyTerminator.feed("CMD\r", Endpoint()).isEmpty());
    QVERIFY(invalidSerialLimit.feed("CMD\r\n", Endpoint()).isEmpty());
    emptyTerminator.reset();
    QVERIFY(!emptyTerminator.initialize());
    QVERIFY(emptyTerminator.feed("\r", Endpoint()).isEmpty());
    QCOMPARE(serialErrors.count(), 1);
    QVERIFY(UdpPacketDecoder().initialize());
    QVERIFY(TcpPacketDecoder().initialize());
    QVERIFY(SerialPacketDecoder().initialize());
    QVERIFY(createPacketDecoder(Channel::Udp));
    QVERIFY(createPacketDecoder(Channel::Tcp));
    QVERIFY(createPacketDecoder(Channel::Serial));
}

void PacketDecoderTests::decoderErrorSignals() {
    UdpPacketDecoder udp;
    QSignalSpy udpErrors(&udp, &IPacketDecoder::errorOccurred);
    QVERIFY(udp.feed("invalid", Endpoint()).isEmpty());
    QCOMPARE(udpErrors.count(), 1);
    const DecoderError error = qvariant_cast<DecoderError>(udpErrors.first().at(0));
    QCOMPARE(error.channel, Channel::Udp);
    QCOMPARE(error.operation, QStringLiteral("decode"));
    QVERIFY(!error.message.isEmpty());
    QCOMPARE(error.code, 0);
    QCOMPARE(udp.feed(frame(), Endpoint()), QVector<QByteArray>({frame()}));
    QCOMPARE(udpErrors.count(), 1);

    TcpPacketDecoder tcp;
    QSignalSpy tcpErrors(&tcp, &IPacketDecoder::errorOccurred);
    QVERIFY(tcp.feed(frame().left(1), Endpoint()).isEmpty());
    QCOMPARE(tcpErrors.count(), 0);
    QCOMPARE(tcp.feed(frame().mid(1), Endpoint()), QVector<QByteArray>({frame()}));
    QCOMPARE(tcpErrors.count(), 0);
    QCOMPARE(tcp.feed(QByteArray("noise") + frame(), Endpoint()), QVector<QByteArray>({frame()}));
    QCOMPARE(tcpErrors.count(), 1);
    QCOMPARE(qvariant_cast<DecoderError>(tcpErrors.first().at(0)).channel, Channel::Tcp);

    SerialPacketDecoder serial("\r", 4);
    QSignalSpy serialErrors(&serial, &IPacketDecoder::errorOccurred);
    QVERIFY(serial.feed("12345", Endpoint()).isEmpty());
    QCOMPARE(serialErrors.count(), 1);
    QVERIFY(serial.feed("more", Endpoint()).isEmpty());
    QCOMPARE(serialErrors.count(), 1);
    QCOMPARE(serial.feed("\rOK\r", Endpoint()), QVector<QByteArray>({"OK\r"}));
    QCOMPARE(serialErrors.count(), 1);
    QCOMPARE(qvariant_cast<DecoderError>(serialErrors.first().at(0)).channel, Channel::Serial);
    QCOMPARE(serial.feed("oversized\rX\r", Endpoint()), QVector<QByteArray>({"X\r"}));
    QCOMPARE(serialErrors.count(), 2);
}

void PacketDecoderTests::jcsChannelPackets() {
    const PacketRule rule = Rule::JCS_Channel_PacketRule();
    for (int channel = 1; channel <= 16; ++channel) {
        QByteArray packet = frame(2);
        packet[0] = packet[2] = char(0xFA);
        packet[1] = packet[3] = char(channel);
        UdpPacketDecoder udp({rule});
        QCOMPARE(udp.feed(packet, Endpoint()), QVector<QByteArray>({packet}));
        for (int split = 0; split <= packet.size(); ++split) {
            TcpPacketDecoder tcp({rule});
            QSignalSpy errors(&tcp, &IPacketDecoder::errorOccurred);
            QVector<QByteArray> packets = tcp.feed(packet.left(split), Endpoint());
            packets += tcp.feed(packet.mid(split), Endpoint());
            QCOMPARE(packets, QVector<QByteArray>({packet}));
            QCOMPARE(errors.count(), 0);
        }
    }

    QByteArray channelPacket = frame();
    channelPacket.replace(0, 4, QByteArray::fromHex("fa01fa01"));
    UdpPacketDecoder udp({rule});
    const QVector<QByteArray> badHeaders = {
        QByteArray::fromHex("fa00fa00"), QByteArray::fromHex("fa11fa11"),
        QByteArray::fromHex("fa01fa02"), QByteArray::fromHex("fa010001")};
    for (const QByteArray &header : badHeaders) {
        QByteArray bad = channelPacket;
        bad.replace(0, 4, header);
        QCOMPARE(rule.frameSize(bad), -1);
        QVERIFY(udp.feed(bad, Endpoint()).isEmpty());
    }

    // A fixed FA FB FA FB header takes priority over the FA candidate marker.
    const QByteArray dataPacket =
        QByteArray::fromHex("fafbfafb020100000000000300c0c14400d0c14400b8c144");
    UdpPacketDecoder dataUdp;
    QCOMPARE(dataUdp.feed(dataPacket, Endpoint()), QVector<QByteArray>({dataPacket}));
    const QByteArray stream = dataPacket + channelPacket + dataPacket;
    for (int split = 0; split <= stream.size(); ++split) {
        TcpPacketDecoder tcp({Rule::JCS_Data_PacketRule(), rule});
        QVector<QByteArray> packets = tcp.feed(stream.left(split), Endpoint());
        packets += tcp.feed(stream.mid(split), Endpoint());
        QCOMPARE(packets, QVector<QByteArray>({dataPacket, channelPacket, dataPacket}));
    }
}

QTEST_GUILESS_MAIN(PacketDecoderTests)
#include "packet_decoder_tests.moc"
