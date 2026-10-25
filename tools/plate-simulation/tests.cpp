#include "PlateIFE.h"
#include "PlateUdpSender.h"
#include "protocol/MPWavelengthParser.h"
#include "protocol/ProtocolControl.h"
#include <QtTest>
#include <QFile>
#include <QTemporaryDir>
#include <QSignalSpy>
#include <QProcess>
#include <QCoreApplication>
#include <QUdpSocket>
#include <QtEndian>
#include <cmath>
#include <cstring>
using namespace fosd;
using namespace fosd::simulation;
class PlateSimulationTests final : public QObject {
    Q_OBJECT
private slots:
    void inverseFitAndClampedEdge() {
        PlateIFE algorithm;
        Calculation::Strain strain;
        for (int i = 0; i < 12; ++i) {
            const double eta = -0.9 + 1.8 * i / 11;
            strain.push_back(-4.0 * (12.0 - 3.0 * eta + 4.0 * eta * eta));
        }
        const auto dofs = algorithm.calculateDofs(strain);
        QVERIFY(std::abs(dofs[0] - 12) < 1e-10);
        QVERIFY(std::abs(dofs[1] + 3) < 1e-10);
        QVERIFY(std::abs(dofs[2] - 4) < 1e-10);
        Calculation::Point3D point;
        point.x = 1000;
        point.y = 200;
        point.z = 2;
        const auto d = algorithm.calculateDisplacement(point, dofs);
        QVERIFY(std::abs(d[2] * 1000 - 13) < 1e-10);
        QVERIFY(std::abs(d[0] * 1000 + 0.052) < 1e-10);
        QVERIFY(std::abs(d[1] * 1000 + 0.05) < 1e-10);
        point.x = 0;
        const auto fixed = algorithm.calculateDisplacement(point, dofs);
        for (double value : fixed)
            QCOMPARE(value, 0.0);
    }
    void packetAndCalibrationRoundTrip() {
        PlateMotion motion;
        const double seconds = 0.63;
        protocol::MPWavelengthParser parser;
        QString error;
        const auto frame = std::dynamic_pointer_cast<const core::WavelengthFrame>(
            parser.parse(wavelengthPacket(42, seconds, motion), &error));
        QVERIFY2(frame, qPrintable(error));
        QCOMPARE(frame->sequence, quint32(42));
        QCOMPARE(frame->recordCount, quint16(12));
        const auto config = defaultCalibration();
        Calculation::StrainCalibration calibration;
        calibration.setCalibrationStrainData(config.strainCalibration);
        calibration.setCalibrationChannelData(config.channelCalibration);
        QVERIFY(calibration.checkPara());
        Calculation::Wavelength values;
        for (int i = 0; i < 12; ++i) {
            QCOMPARE(frame->records[i].channel, quint8(i + 1));
            values.push_back(frame->records[i].wavelength);
        }
        PlateIFE algorithm;
        const auto dofs = algorithm.calculateDofs(
            calibration.rearrangeCalibration(calibration.applyCalibration(values)));
        const auto expected = modesAt(seconds, motion);
        for (int i = 0; i < 3; ++i)
            QVERIFY(std::abs(dofs[size_t(i)] - expected[size_t(i)]) < 0.15);
    }
    void modelGeometry() {
        QFile model(defaultModelPath());
        QVERIFY2(model.open(QIODevice::ReadOnly), qPrintable(model.fileName()));
        const auto bytes = model.readAll();
        QVERIFY(bytes.size() > 84);
        const quint32 faces =
            qFromLittleEndian<quint32>(reinterpret_cast<const uchar *>(bytes.constData() + 80));
        QCOMPARE(faces, quint32(2784));
        QCOMPARE(bytes.size(), int(84 + 50 * faces));
        double minimum[3] = {1e9, 1e9, 1e9}, maximum[3] = {-1e9, -1e9, -1e9};
        for (quint32 i = 0; i < faces; ++i)
            for (int vertex = 0; vertex < 3; ++vertex)
                for (int axis = 0; axis < 3; ++axis) {
                    const int offset = 84 + int(i) * 50 + 12 + vertex * 12 + axis * 4;
                    const quint32 bits = qFromLittleEndian<quint32>(
                        reinterpret_cast<const uchar *>(bytes.constData() + offset));
                    float value;
                    std::memcpy(&value, &bits, 4);
                    minimum[axis] = qMin(minimum[axis], double(value));
                    maximum[axis] = qMax(maximum[axis], double(value));
                }
        QCOMPARE(minimum[0], 0.0);
        QCOMPARE(maximum[0], 1000.0);
        QCOMPARE(minimum[1], -200.0);
        QCOMPARE(maximum[1], 200.0);
        QCOMPARE(minimum[2], -2.0);
        QCOMPARE(maximum[2], 2.0);
    }
    void udpCalculationAndStorage() {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        protocol::ProtocolControl protocol;
        Calculation::CalculationControl calculation(protocol.dataModel(), nullptr, createPlateIFE);
        connect(&protocol, &protocol::ProtocolControl::frameParsed, &calculation.storage(),
                &storage::StorageControl::writeWavelengthFrame);
        QSignalSpy configured(&calculation,
                              &Calculation::CalculationControl::configurationAccepted);
        QSignalSpy accepted(&calculation, &Calculation::CalculationControl::modelAccepted);
        QSignalSpy results(&calculation, &Calculation::CalculationControl::resultReady);
        QSignalSpy errors(&calculation, &Calculation::CalculationControl::errorOccurred);
        QSignalSpy parsingErrors(&protocol, &protocol::ProtocolControl::errorOccurred);
        QSignalSpy started(&calculation.storage(), &storage::StorageControl::started);
        QSignalSpy stopped(&calculation.storage(), &storage::StorageControl::stopped);
        QSignalSpy storageErrors(&calculation.storage(), &storage::StorageControl::errorOccurred);
        calculation.setConfiguration(defaultCalibration());
        core::ModelData model{{0, {0, 0, 0}}, {1, {1000, 200, 2}}, {2, {700, -200, -2}}};
        calculation.setModel(model);
        QTRY_COMPARE(configured.count(), 1);
        QTRY_COMPARE(accepted.count(), 1);
        storage::StorageConfiguration output;
        output.path = directory.filePath("plate.csv");
        output.allPoints = true;
        output.dataTypes = storage::DataType::Wavelength | storage::DataType::CalibratedHardware |
                           storage::DataType::PointDisplacement;
        calculation.storage().start(output);
        QTRY_COMPARE(started.count(), 1);
        QUdpSocket reservation;
        QVERIFY(reservation.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
        const quint16 port = reservation.localPort();
        reservation.close();
        communication::UdpConfig udp;
        udp.bindAddress = QHostAddress::LocalHost;
        udp.localPort = port;
        QSignalSpy states(&protocol.communication(), &communication::ConnectControl::stateChanged);
        protocol.communication().openUdp(udp);
        QTRY_VERIFY(states.count() > 0);
        PlateUdpSender sender;
        PlateSenderSettings settings;
        settings.port = port;
        settings.rateHz = 40;
        QSignalSpy sent(&sender, &PlateUdpSender::datagramSent);
        QVERIFY(sender.start(settings));
        QTRY_VERIFY(results.count() >= 8);
        sender.stop();
        QVERIFY(!sender.isRunning());
        const auto result = calculation.dataModel()->latest();
        QVERIFY(result);
        QCOMPARE(result->algorithmId, QStringLiteral("demo.plate.modal-ife"));
        QCOMPARE(result->sourceFrame->source.transport, core::TransportKind::Udp);
        const auto source =
            std::dynamic_pointer_cast<const core::WavelengthFrame>(result->sourceFrame->data);
        QVERIFY(source);
        double sentAt = -1;
        for (const auto &sample : sent)
            if (sample[0].toUInt() == source->sequence)
                sentAt = sample[1].toDouble();
        QVERIFY(sentAt >= 0);
        const auto modes = modesAt(sentAt, settings.motion);
        const double analyticTip = modes[0] + modes[1] + modes[2];
        QVERIFY(std::abs(result->displacementMap.value(1)[2] - analyticTip) < 0.2);
        QCOMPARE(result->displacementMap.value(0)[2], 0.0);
        QVERIFY(std::abs(result->displacementMap.value(1)[2]) > 0.1);
        const auto sentBefore = sent.count();
        QTest::qWait(80);
        QCOMPARE(sent.count(), sentBefore);
        calculation.storage().stop();
        QTRY_COMPARE(stopped.count(), 1);
        QFile file(output.path);
        QVERIFY(file.open(QIODevice::ReadOnly));
        const auto records = file.readAll();
        QVERIFY(records.contains("wavelength"));
        QVERIFY(records.contains("calibrated_hardware"));
        QVERIFY(records.contains("point_displacement"));
        QCOMPARE(errors.count(), 0);
        QCOMPARE(parsingErrors.count(), 0);
        QCOMPARE(storageErrors.count(), 0);
        QVERIFY(sent.count() >= 8);
        // A generous scheduling bound catches accidental busy loops/incorrect rate units.
        QVERIFY(sent.last()[1].toDouble() >= (sent.count() - 2) / 40.0);
    }
    void rejectsInvalidSenderSettings() {
        PlateUdpSender sender;
        PlateSenderSettings settings;
        settings.rateHz = 0;
        QString error;
        QVERIFY(!sender.start(settings, &error));
        QVERIFY(!error.isEmpty());
        QVERIFY(!sender.isRunning());
    }
    void standaloneSender() {
        QUdpSocket receiver;
        QVERIFY(receiver.bind(QHostAddress(QHostAddress::LocalHost), quint16(0)));
        QString executable = QCoreApplication::applicationDirPath() + "/plate-udp-sender";
#ifdef Q_OS_WIN
        executable += ".exe";
#endif
        QProcess process;
        process.start(executable, {"--port", QString::number(receiver.localPort()), "--rate", "20",
                                   "--duration", "0.25"});
        QVERIFY(process.waitForStarted(3000));
        QVERIFY(process.waitForFinished(4000));
        QCOMPARE(process.exitStatus(), QProcess::NormalExit);
        QCOMPARE(process.exitCode(), 0);
        int received = 0;
        protocol::MPWavelengthParser parser;
        while (receiver.hasPendingDatagrams()) {
            QByteArray packet(int(receiver.pendingDatagramSize()), '\0');
            receiver.readDatagram(packet.data(), packet.size());
            const auto frame =
                std::dynamic_pointer_cast<const core::WavelengthFrame>(parser.parse(packet));
            QVERIFY(frame);
            QCOMPARE(frame->recordCount, quint16(12));
            ++received;
        }
        QVERIFY(received >= 4);
        QVERIFY(received <= 8);
    }
};
QTEST_GUILESS_MAIN(PlateSimulationTests)
#include "tests.moc"
