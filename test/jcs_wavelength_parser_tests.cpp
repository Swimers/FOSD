#include "protocol/JCSWavelengthParser.h"
#include <QtTest>
#include <cmath>

using namespace fosd;

namespace {
QByteArray examplePacket() {
    // Protocol document example: CH1=2, CH2=1, N=3.
    return QByteArray::fromHex("fafbfafb020100000000000300c0c14400d0c14400b8c144");
}
std::shared_ptr<const core::WavelengthFrame> decode(const QByteArray &packet, QString *error = nullptr) {
    return std::dynamic_pointer_cast<const core::WavelengthFrame>(
        protocol::JCSWavelengthParser().parse(packet, error));
}
}

class JCSParserTests : public QObject {
    Q_OBJECT
private slots:
    void documentExample();
    void sparseChannelsAndUnsignedCounts();
    void emptyAndTrailingBytes();
    void malformedPackets();
    void floatBitPatterns();
};

void JCSParserTests::documentExample() {
    QString error = QStringLiteral("stale");
    const auto frame = decode(examplePacket(), &error);
    QVERIFY(frame);
    QVERIFY(error.isEmpty());
    QCOMPARE(frame->recordCount, quint16(3));
    QCOMPARE(frame->records.size(), 3);
    QCOMPARE(frame->version, quint8(0));
    QCOMPARE(frame->dataType, quint8(0));
    QCOMPARE(frame->sequence, quint32(0));
    QCOMPARE(frame->records.at(0).channel, quint8(1));
    QCOMPARE(frame->records.at(0).sensor, quint8(1));
    QCOMPARE(frame->records.at(0).wavelength, 1550.0f);
    QCOMPARE(frame->records.at(1).channel, quint8(1));
    QCOMPARE(frame->records.at(1).sensor, quint8(2));
    QCOMPARE(frame->records.at(1).wavelength, 1550.5f);
    QCOMPARE(frame->records.at(2).channel, quint8(2));
    QCOMPARE(frame->records.at(2).sensor, quint8(1));
    QCOMPARE(frame->records.at(2).wavelength, 1549.75f);
}

void JCSParserTests::sparseChannelsAndUnsignedCounts() {
    QByteArray packet = QByteArray::fromHex("fafbfafb0002000000000103");
    packet += examplePacket().mid(12);
    const auto sparse = decode(packet);
    QVERIFY(sparse);
    QCOMPARE(sparse->records.at(0).channel, quint8(2));
    QCOMPARE(sparse->records.at(1).sensor, quint8(2));
    QCOMPARE(sparse->records.at(2).channel, quint8(7));
    QCOMPARE(sparse->records.at(2).sensor, quint8(1));

    packet = QByteArray::fromHex("fafbfafb000000000000ffff");
    for (int index = 0; index < 255; ++index)
        packet += QByteArray::fromHex("0000c03f");
    const auto maximum = decode(packet);
    QVERIFY(maximum);
    QCOMPARE(maximum->recordCount, quint16(255));
    QCOMPARE(maximum->records.size(), 255);
    QCOMPARE(maximum->records.last().channel, quint8(7));
    QCOMPARE(maximum->records.last().sensor, quint8(255));
    QCOMPARE(maximum->records.last().wavelength, 1.5f);
}

void JCSParserTests::emptyAndTrailingBytes() {
    const auto empty = decode(QByteArray::fromHex("fafbfafb0000000000000000"));
    QVERIFY(empty);
    QCOMPARE(empty->recordCount, quint16(0));
    QVERIFY(empty->records.isEmpty());
    const auto padded = decode(examplePacket() + QByteArray::fromHex("aabbcc"));
    QVERIFY(padded);
    QCOMPARE(padded->records.size(), 3);
    QCOMPARE(padded->records.last().wavelength, 1549.75f);
}

void JCSParserTests::malformedPackets() {
    protocol::JCSWavelengthParser parser;
    const QByteArray valid = examplePacket();
    for (int size = 0; size < valid.size(); ++size) {
        QString error;
        QVERIFY(!parser.parse(valid.left(size), &error));
        QVERIFY(!error.isEmpty());
        QVERIFY(!parser.parse(valid.left(size))); // Optional error output.
    }
    QVector<QByteArray> invalid;
    QByteArray packet = valid;
    packet[0] = 0;
    invalid.append(packet);
    packet = valid;
    packet[11] = 2; // Total smaller than channel sum.
    invalid.append(packet);
    packet = valid;
    packet[4] = 0; // Total larger than channel sum.
    invalid.append(packet);
    packet = QByteArray::fromHex("fafbfafbffffffffffffffff");
    packet.append(QByteArray(255 * 4, '\0')); // Sum must not wrap at 255.
    invalid.append(packet);
    for (const QByteArray &bytes : invalid) {
        QString error;
        QVERIFY(!parser.parse(bytes, &error));
        QVERIFY(!error.isEmpty());
    }
    QVERIFY(decode(valid)); // Rejection leaves no parser state behind.
}

void JCSParserTests::floatBitPatterns() {
    QByteArray packet = examplePacket().left(12);
    packet += QByteArray::fromHex("0000807f0000c07f000010c0");
    const auto frame = decode(packet);
    QVERIFY(frame);
    QVERIFY(std::isinf(frame->records.at(0).wavelength));
    QVERIFY(std::isnan(frame->records.at(1).wavelength));
    QCOMPARE(frame->records.at(2).wavelength, -2.25f);
}

QTEST_GUILESS_MAIN(JCSParserTests)
#include "jcs_wavelength_parser_tests.moc"
