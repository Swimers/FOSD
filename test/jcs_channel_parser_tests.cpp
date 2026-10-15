#include "protocol/JCSChannelParser.h"
#include <QtEndian>
#include <QtTest>

using fosd::protocol::JCSChannelParser;

namespace {
QByteArray channelPacket(quint16 points, quint8 channel = 1) {
    QByteArray packet(708 + int(points) * 2, '\0');
    packet[0] = packet[2] = char(0xFA);
    packet[1] = packet[3] = char(channel);
    qToLittleEndian<quint16>(points, reinterpret_cast<uchar *>(packet.data() + 6));
    return packet;
}
}

class JCSChannelParserTests : public QObject {
    Q_OBJECT
private slots:
    void recognizesLegacyChannels();
    void decodesAndDiscards();
    void rejectsMalformedPackets();
    void pointCountBoundaries();
};

void JCSChannelParserTests::recognizesLegacyChannels() {
    JCSChannelParser parser;
    QCOMPARE(parser.protocolId(), QStringLiteral("jcs.channel"));
    for (int channel = 0; channel < 6; ++channel)
        QVERIFY(parser.matches(channelPacket(3, quint8(channel)).left(4)));
    QVERIFY(!parser.matches(channelPacket(3, 6)));
    QVERIFY(!parser.matches(channelPacket(3, 255)));
    QVERIFY(!parser.matches(QByteArray::fromHex("fafbfafb")));
    QVERIFY(!parser.matches(QByteArray::fromHex("fa01fa")));
}

void JCSChannelParserTests::decodesAndDiscards() {
    JCSChannelParser parser;
    QByteArray packet = channelPacket(3);
    packet.replace(4, 2, QByteArray::fromHex("abcd")); // Unknown header bytes.
    packet.replace(8, 6, QByteArray::fromHex("6400c8002c01")); // X: 100,200,300.
    packet.replace(708, 6, QByteArray::fromHex("0a0014001e00")); // Y: 10,20,30.
    QString error = QStringLiteral("stale");
    QVERIFY(!parser.parse(packet, &error));
    QVERIFY(error.isEmpty()); // Null is intentional, not a decoding error.
    QVERIFY(!parser.parse(packet));
    error = QStringLiteral("stale");
    QVERIFY(!parser.parse(packet + QByteArray::fromHex("aabb"), &error));
    QVERIFY(error.isEmpty());
}

void JCSChannelParserTests::rejectsMalformedPackets() {
    JCSChannelParser parser;
    const QByteArray valid = channelPacket(3);
    // Includes short headers, incomplete X/padding area, and truncated Y data.
    for (int size = 0; size < valid.size(); ++size) {
        QString error;
        QVERIFY(!parser.parse(valid.left(size), &error));
        QVERIFY(!error.isEmpty());
    }
    QVector<QByteArray> invalid;
    QByteArray packet = valid;
    packet[0] = 0;
    invalid.append(packet);
    packet = valid;
    packet[3] = 2;
    invalid.append(packet);
    invalid.append(channelPacket(3, 6));
    invalid.append(channelPacket(3, 0xFB));
    packet = valid;
    packet[6] = packet[7] = char(0xFF);
    invalid.append(packet);
    for (const QByteArray &bytes : invalid) {
        QString error;
        QVERIFY(!parser.parse(bytes, &error));
        QVERIFY(!error.isEmpty());
        QVERIFY(!parser.parse(bytes));
    }
}

void JCSChannelParserTests::pointCountBoundaries() {
    JCSChannelParser parser;
    // No MAX_POINTS definition was provided; preserve legacy offset decoding
    // even when the two areas overlap rather than inventing a new restriction.
    const QVector<quint16> counts = {0, 1, 350, 351, 65535};
    for (quint16 count : counts) {
        QString error = QStringLiteral("stale");
        const QByteArray packet = channelPacket(count);
        QVERIFY(!parser.parse(packet, &error));
        QVERIFY(error.isEmpty());
        QVERIFY(!parser.parse(packet.left(packet.size() - 1), &error));
        QVERIFY(!error.isEmpty());
    }
}

QTEST_GUILESS_MAIN(JCSChannelParserTests)
#include "jcs_channel_parser_tests.moc"
