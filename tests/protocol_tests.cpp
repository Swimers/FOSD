#include <QtTest>
#include <QtEndian>
#include <QTemporaryDir>
#include <QDir>
#include <QUdpSocket>
#include <limits>
#include <cstring>
#include <algorithm>
#include <memory>
#include "wavelengthframe.h"
#include "analyzepackets.h"
#include "udptask.h"
#include "tcptask.h"

namespace {
void appendFloat(QByteArray &bytes, float value)
{
    quint32 bits;
    std::memcpy(&bits, &value, sizeof(bits));
    char encoded[4];
    qToLittleEndian<quint32>(bits, reinterpret_cast<uchar *>(encoded));
    bytes.append(encoded, 4);
}

QByteArray newPacket(const QVector<WavelengthRecord> &records)
{
    QByteArray bytes = QByteArray::fromHex("5aa501027856efcdab89");
    qToLittleEndian<quint16>(quint16(records.size()), reinterpret_cast<uchar *>(bytes.data() + 4));
    for (const auto &record : records) {
        bytes.append(char(record.channel));
        bytes.append(char(record.sensor));
        appendFloat(bytes, record.wavelength);
    }
    return bytes;
}

QByteArray legacyPacket(const QVector<WavelengthRecord> &records)
{
    QByteArray bytes(DataAddress, '\0');
    bytes.replace(0, 4, QByteArray::fromHex("fafbfafb"));
    for (const auto &record : records)
        bytes[record.channel + 3] = char(quint8(bytes[record.channel + 3]) + 1);
    bytes[MAX_CHANNELS + 4] = char(records.size());
    for (const auto &record : records)
        appendFloat(bytes, record.wavelength);
    return bytes;
}

void selectAll(analyzepackets &analyzer)
{
    int flags[MAX_CHANNELS];
    std::fill(flags, flags + MAX_CHANNELS, 1);
    analyzer.freshCH(flags);
}
}

class ProtocolTests : public QObject
{
    Q_OBJECT
private slots:
    void knownWireBytes()
    {
        // Independent fixture: 1550.25 = IEEE754 0x44C1C800, little endian.
        WavelengthFrame frame;
        QVERIFY(parseWavelengthFrame(QByteArray::fromHex("5aa501020100efcdab89010200c8c144"), frame));
        QCOMPARE(frame.version, quint8(1));
        QCOMPARE(frame.dataType, quint8(2));
        QCOMPARE(frame.sequence, quint32(0x89abcdef));
        QCOMPARE(frame.records.size(), 1);
        QCOMPARE(frame.records[0].channel, quint8(1));
        QCOMPARE(frame.records[0].sensor, quint8(2));
        QCOMPARE(frame.records[0].wavelength, 1550.25f);
    }

    void rejectsMalformedPackets()
    {
        const QByteArray valid = newPacket({{1, 1, 1550.0f}});
        QVector<QByteArray> invalid;
        for (int length = 0; length < valid.size(); ++length)
            invalid.append(valid.left(length));
        invalid.append(valid + '\0');
        auto badMagic = valid;
        badMagic[0] = 0;
        invalid.append(badMagic);
        auto badCount = valid;
        badCount[4] = 2;
        invalid.append(badCount);
        invalid.append(newPacket({{0, 1, 1550}}));
        invalid.append(newPacket({{17, 1, 1550}}));
        invalid.append(newPacket({{1, 0, 1550}}));
        invalid.append(newPacket({{1, 1, 1550}, {1, 1, 1551}}));
        invalid.append(newPacket({{1, 1, std::numeric_limits<float>::infinity()}}));
        invalid.append(newPacket({{1, 1, std::numeric_limits<float>::quiet_NaN()}}));
        WavelengthFrame frame;
        frame.sequence = 42;
        frame.records = {{2, 3, 1234}};
        for (const auto &bytes : invalid) {
            QString error;
            QVERIFY(!parseWavelengthFrame(bytes, frame, &error));
            QVERIFY(!error.isEmpty());
            QCOMPARE(frame.sequence, quint32(42));
            QCOMPARE(frame.records.size(), 1);
            QCOMPARE(frame.records[0].wavelength, 1234.0f);
        }
    }

    void capacityAndEmptyFrame()
    {
        QVector<WavelengthRecord> records;
        for (int i = 0; i < 1024; ++i)
            records.append({quint8(i / 255 + 1), quint8(i % 255 + 1), float(1500 + i)});
        WavelengthFrame frame;
        QVERIFY(parseWavelengthFrame(newPacket(records), frame));
        QCOMPARE(frame.records.size(), 1024);
        records.append({16, 255, 1600});
        QVERIFY(!parseWavelengthFrame(newPacket(records), frame));
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        records.removeLast();
        analyzer->analyzeData(newPacket(records));
        QCOMPARE(analyzer->CH_SUM, 1024);
        QCOMPARE(analyzer->CH_datagram[1023], 2523.0f);
        analyzer->analyzeData(newPacket({}));
        QCOMPARE(analyzer->CH_SUM, 0);
        QCOMPARE(analyzer->CH_datagram[1023], 0.0f);
        QVERIFY(analyzer->wavelength_static_txt.isEmpty());
    }

    void newAndLegacyUseSamePath()
    {
        const QVector<WavelengthRecord> ordered = {{1, 1, 1550.25f}, {1, 2, 1551.5f}, {16, 1, 1560.75f}};
        auto modern = std::unique_ptr<analyzepackets>(new analyzepackets);
        auto legacy = std::unique_ptr<analyzepackets>(new analyzepackets);
        for (auto *analyzer : {modern.get(), legacy.get()}) {
            int flags[MAX_CHANNELS] = {};
            flags[0] = 1; // Final channel deselected: CSV must still finish each frame.
            analyzer->freshCH(flags);
            analyzer->A = 2;
            analyzer->B = 0.125f;
            analyzer->original_wavelength[0] = 1550;
            analyzer->original_wavelength[1] = 1551;
        }
        modern->analyzeData(newPacket({ordered[2], ordered[1], ordered[0]}));
        legacy->analyzeData(legacyPacket(ordered) + QByteArray::fromHex("abcd"));
        QCOMPARE(modern->CH_SUM, 3);
        QCOMPARE(modern->channel_counts[0], 2);
        QCOMPARE(modern->channel_counts[15], 1);
        for (int i = 0; i < 1024; ++i) {
            QCOMPARE(modern->FBG_count[i], legacy->FBG_count[i]);
            QCOMPARE(modern->CH_datagram[i], legacy->CH_datagram[i]);
            QCOMPARE(modern->strain_float[i], legacy->strain_float[i]);
        }
        QCOMPARE(modern->FBG_count[0], QString("CH1_1"));
        QCOMPARE(modern->wavelength[0], QString("1550.25000"));
        QVERIFY(qAbs(modern->strain_float[0] - (0.00075f * 0.25f / 2 + 0.125f)) < 1e-7f);
        QCOMPARE(modern->fbg_txt_1, QString("time,CH1_1,CH1_2,CH1_1,CH1_2"));
        QCOMPARE(modern->wavelength_static_txt, legacy->wavelength_static_txt);
        QCOMPARE(modern->strain_txt_1, legacy->strain_txt_1);
        modern->analyzeData(newPacket({{1, 2, 1551.5f}, {1, 1, 1550.25f}}));
        QCOMPARE(modern->wavelength_txt_1, QString("1550.25000,1551.50000"));
        int flags[MAX_CHANNELS] = {};
        modern->freshCH(flags);
        modern->analyzeData(newPacket(ordered));
        QVERIFY(modern->wavelength_static_txt.isEmpty());
        QCOMPARE(modern->strain_float[0], 0.0f);
    }

    void invalidFramesPreservePublishedData()
    {
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        analyzer->analyzeData(newPacket({{1, 1, 1550}}));
        analyzer->analyzeData(newPacket({{1, 1, 1700}}).chopped(1));
        QByteArray badLegacy = legacyPacket({{1, 1, 1800}});
        badLegacy[20] = 2;
        analyzer->analyzeData(badLegacy);
        analyzer->analyzeData(legacyPacket({{1, 1, 1900}}).chopped(1));
        QCOMPARE(analyzer->CH_datagram[0], 1550.0f);
        QCOMPARE(analyzer->CH_SUM, 1);
    }

    void resetAndSave()
    {
        QTemporaryDir dir;
        QVERIFY(dir.isValid());
        const QString oldCwd = QDir::currentPath();
        struct RestoreCwd { QString path; ~RestoreCwd() { QDir::setCurrent(path); } } restore{oldCwd};
        QVERIFY(QDir::setCurrent(dir.path()));
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        QSignalSpy finished(analyzer.get(), &analyzepackets::resetOriginalWavelengthFinished);
        analyzer->resetOriginalWavelength();
        for (int i = 0; i < 20; ++i) {
            const QVector<WavelengthRecord> records = {{1, 1, float(1500 + i)}, {2, 1, float(1600 + i)}};
            analyzer->analyzeData(i % 2 ? legacyPacket(records) : newPacket({records[1], records[0]}));
        }
        QCOMPARE(finished.count(), 1);
        QVERIFY(!analyzer->Flag_Reset);
        QCOMPARE(analyzer->original_wavelength[0], 1509.5f);
        QCOMPARE(analyzer->original_wavelength[1], 1609.5f);
        QFile baseline(QString::fromUtf8("初始波长.txt"));
        QVERIFY(baseline.open(QIODevice::ReadOnly));
        QVERIFY(baseline.readAll().contains("1509.5"));
        analyzer->saveManual();
        QFile saved(QString::fromUtf8("data/outputs/手动存储/data.csv"));
        QVERIFY(saved.open(QIODevice::ReadOnly));
        const QByteArray csv = saved.readAll();
        QVERIFY(csv.contains("time,CH1_1,CH2_1,CH1_1,CH2_1"));
        QVERIFY(csv.contains("1519.00000,1619.00000"));
        analyzer->saveAuto(1);
        QTest::qWait(20);
        analyzer->saveAuto(1);
        const QStringList files = QDir(QString::fromUtf8("data/outputs/自动存储")).entryList({"*.csv"}, QDir::Files);
        QCOMPARE(files.size(), 1);
        QFile dynamic(QString::fromUtf8("data/outputs/自动存储/") + files[0]);
        QVERIFY(dynamic.open(QIODevice::ReadOnly));
        QVERIFY(dynamic.readAll().contains("1519.00000,1619.00000"));
    }

    void sparseIdsAndLayoutReset()
    {
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        analyzer->resetOriginalWavelength();
        analyzer->analyzeData(newPacket({{16, 255, 1550}}));
        QCOMPARE(analyzer->FBG_count[0], QString("CH16_255"));
        QCOMPARE(analyzer->count_num, 1);
        analyzer->analyzeData(newPacket({{16, 254, 1600}}));
        QCOMPARE(analyzer->count_num, 1);
        QCOMPARE(analyzer->original_wavelength_temp[0], 1600.0f);
        analyzer->resetOriginalWavelength();
        QCOMPARE(analyzer->count_num, 0);
        QCOMPARE(analyzer->original_wavelength_temp[0], 0.0f);
    }

    void legacySpectrumStillWorks()
    {
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        QByteArray bytes(710, '\0');
        bytes.replace(0, 4, QByteArray::fromHex("fa00fa00"));
        bytes[6] = 1;
        bytes[8] = 0x34;
        bytes[9] = 0x12;
        bytes[708] = 0x78;
        bytes[709] = 0x56;
        analyzer->analyzeData(bytes);
        QCOMPARE(analyzer->CH_point[0], 1);
        QCOMPARE(analyzer->data_x_trans[0][0], 0x1234);
        QCOMPARE(analyzer->data_y_trans[0][0], 0x5678);
        analyzer->analyzeData(bytes.left(8));
        QCOMPARE(analyzer->CH_point[0], 0);
    }

    void tcpFragmentationAndCoalescing()
    {
        QTcpServer portProbe;
        QVERIFY(portProbe.listen(QHostAddress::LocalHost, 0));
        const quint16 port = portProbe.serverPort();
        portProbe.close();
        tcptask receiver;
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        connect(&receiver, &tcptask::tcpPocketsReceived, analyzer.get(), &analyzepackets::analyzeData);
        QSignalSpy packets(&receiver, &tcptask::tcpPocketsReceived);
        QSignalSpy texts(&receiver, &tcptask::tcpDataReceived);
        receiver.tcpOpen(QHostAddress::LocalHost, port);
        QTcpSocket sender;
        sender.connectToHost(QHostAddress::LocalHost, port);
        QVERIFY(sender.waitForConnected());
        QTest::qWait(10);
        const QByteArray first = newPacket({{1, 1, 1550.25f}});
        for (int i = 0; i < first.size(); ++i) {
            QCOMPARE(sender.write(first.mid(i, 1)), qint64(1));
            sender.flush();
            QTest::qWait(5);
        }
        QTRY_COMPARE(packets.count(), 1);
        QCOMPARE(packets[0][0].toByteArray(), first);
        QCOMPARE(analyzer->CH_datagram[0], 1550.25f);
        const QByteArray second = newPacket({{1, 1, 1551.5f}});
        const QByteArray combined = second + first;
        QCOMPARE(sender.write(combined), qint64(combined.size()));
        sender.flush();
        QTRY_COMPARE(packets.count(), 3);
        QCOMPARE(packets[1][0].toByteArray(), second);
        QCOMPARE(packets[2][0].toByteArray(), first);
        QCOMPARE(texts.count(), 0);
        const QByteArray legacy = legacyPacket({{1, 1, 1560}});
        QCOMPARE(sender.write(legacy), qint64(legacy.size()));
        sender.flush();
        QTRY_COMPARE(packets.count(), 4);
        QCOMPARE(analyzer->CH_datagram[0], 1560.0f);
        // Closing with a partial frame must not leak it to the next client.
        sender.write(first.left(7));
        sender.flush();
        QTest::qWait(10);
        receiver.tcpClose();
        receiver.tcpOpen(QHostAddress::LocalHost, port);
        QTcpSocket next;
        next.connectToHost(QHostAddress::LocalHost, port);
        QVERIFY(next.waitForConnected());
        next.write(second);
        next.flush();
        QTRY_COMPARE(packets.count(), 5);
        QCOMPARE(analyzer->CH_datagram[0], 1551.5f);
        receiver.tcpClose();
    }

    void udpReception()
    {
        QUdpSocket portProbe;
        QVERIFY(portProbe.bind(QHostAddress(QHostAddress::LocalHost), 0));
        const quint16 port = portProbe.localPort();
        portProbe.close();
        udptask receiver;
        auto analyzer = std::unique_ptr<analyzepackets>(new analyzepackets);
        selectAll(*analyzer);
        connect(&receiver, &udptask::udpPocketsReceived, analyzer.get(), &analyzepackets::analyzeData);
        QSignalSpy packets(&receiver, &udptask::udpPocketsReceived);
        QSignalSpy status(&receiver, &udptask::connectionStatusChanged);
        receiver.udpOpen(QHostAddress::LocalHost, port);
        QCOMPARE(status.count(), 1);
        QVERIFY(status[0][0].toBool());
        QUdpSocket sender;
        for (const auto &bytes : {newPacket({{1, 1, 1550.25f}}), legacyPacket({{1, 1, 1551.5f}})}) {
            const int before = packets.count();
            QCOMPARE(sender.writeDatagram(bytes, QHostAddress::LocalHost, port), qint64(bytes.size()));
            QTRY_COMPARE(packets.count(), before + 1);
            QCOMPARE(analyzer->CH_datagram[0], before == 0 ? 1550.25f : 1551.5f);
        }
        receiver.udpClose();
    }
};

QTEST_GUILESS_MAIN(ProtocolTests)
#include "protocol_tests.moc"
