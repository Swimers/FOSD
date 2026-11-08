#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include "FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.h"
#include "FOSD-Classic-QWidgets/tests/workbench_test_packets.h"
#include <QTemporaryDir>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtTest>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
using namespace fosd;
using namespace fosd::ui::classic;
class MemoryView : public IView {
  public:
    ViewEvents event;
    QVariantMap input, output;
    QString status;
    ViewEvents *events() const override {
        return const_cast<ViewEvents *>(&event);
    }
    QVariantMap values() const override {
        return input;
    }
    void applyFields(const QVariantMap &f) override {
        for (auto i = f.begin(); i != f.end(); ++i)
            output[i.key()] = i.value();
    }
    void clearSensitiveFields() override {
    }
    void clearInputs() override {
        input.clear();
    }
    void showStatus(const QString &s) override {
        status = s;
    }
};
class WorkbenchModelTests : public QObject {
    Q_OBJECT
  private slots:
    void calibrationAcknowledgementAndRejection() {
        QTemporaryDir dir;
        qputenv("FOSD_CONFIG_FILE", dir.filePath("calibration.ini").toUtf8());
        application::WorkbenchModel model;
        Calculation::CalculationConfiguration configuration;
        configuration.strainCalibration.A = 1;
        configuration.strainCalibration.originalWavelength.assign(12, 1550.0);
        for (int i = 0; i < 12; ++i) {
            configuration.channelCalibration.channelIndices.push_back(i);
            configuration.channelCalibration.calibrationFactors.push_back(1.5);
            configuration.channelCalibration.calibrationOffsets.push_back((i - 6) * 0.25);
        }
        QSignalSpy applied(&model, &application::WorkbenchModel::configurationApplied);
        QString error;
        QVERIFY(model.applyCalibration(configuration, &error));
        QVERIFY(model.state().configurationPending);
        QVERIFY(model.configuration().strainCalibration.originalWavelength.empty());
        QVERIFY(!model.applyCalibration(configuration, &error));
        QTRY_COMPARE(applied.count(), 1);
        QVERIFY(!model.state().configurationPending);
        QVERIFY(model.configuration().channelCalibration.calibrationOffsets ==
                configuration.channelCalibration.calibrationOffsets);
        auto invalid = configuration;
        invalid.channelCalibration.calibrationOffsets = {1.0};
        QVERIFY(!model.applyCalibration(invalid, &error));
        QVERIFY(!model.state().configurationPending);
        QCOMPARE(applied.count(), 1);
        Calculation::CalculationConfiguration saved;
        QVERIFY(config::ParameterStore().read(&saved, true, &error));
        QVERIFY(saved.channelCalibration.calibrationOffsets ==
                configuration.channelCalibration.calibrationOffsets);
        qunsetenv("FOSD_CONFIG_FILE");
    }
    void serialParametersAndInterfaceRefresh() {
        QTemporaryDir dir;
        qputenv("FOSD_CONFIG_FILE", dir.filePath("serial.ini").toUtf8());
        application::WorkbenchModel model;
        MemoryView view;
        ConnectionPresenter presenter(PanelId::Connection, &view);
        presenter.setModel(&model);
        auto &transport = model.protocol().communication();
        disconnect(&transport, &communication::ConnectControl::requestOpenSerial, nullptr, nullptr);
        QSignalSpy opens(&transport, &communication::ConnectControl::requestOpenSerial);
        QSignalSpy finished(&presenter, &PanelPresenter::actionFinished);
        view.input = {
            {"cbox_SerialNameChoose",
             QVariantMap{{"text", QString::fromUtf8("COM42:虚拟串口")}, {"data", "COM42"}}},
            {"cbox_SerialBaudChoose", QVariantMap{{"text", "230400"}}},
            {"edit_SerialDataBits", "7"},
            {"edit_SerialParity", "Even"},
            {"edit_SerialStopBits", "2"},
            {"edit_SerialFlowControl", "Hardware"}};
        presenter.execute("pbtn_OpenSerial");
        QVERIFY(finished.last()[1].toBool());
        QCOMPARE(opens.count(), 1);
        auto actual = qvariant_cast<communication::SerialConfig>(opens.last()[0]);
        QCOMPARE(actual.portName, QString("COM42"));
        QCOMPARE(actual.baudRate, 230400);
        QCOMPARE(actual.dataBits, QSerialPort::Data7);
        QCOMPARE(actual.parity, QSerialPort::EvenParity);
        QCOMPARE(actual.stopBits, QSerialPort::TwoStop);
        QCOMPARE(actual.flowControl, QSerialPort::HardwareControl);
        QString error;
        communication::SerialConfig saved;
        QVERIFY(config::ParameterStore().read(&saved, true, &error));
        QCOMPARE(saved.parity, QSerialPort::EvenParity);
        view.input["edit_SerialParity"] = "unknown";
        presenter.execute("pbtn_OpenSerial");
        QVERIFY(!finished.last()[1].toBool());
        QCOMPARE(opens.count(), 1);
        QVERIFY(config::ParameterStore().read(&saved, true, &error));
        QCOMPARE(saved.parity, QSerialPort::EvenParity);
        view.input = {{"cbox_SerialNameChoose", QVariantMap{{"text", "COM43:USB serial"}}},
                      {"cbox_SerialBaudChoose", QVariantMap{{"text", "115200"}}}};
        presenter.execute("pbtn_OpenSerial");
        QVERIFY(finished.last()[1].toBool());
        QCOMPARE(opens.count(), 2);
        actual = qvariant_cast<communication::SerialConfig>(opens.last()[0]);
        QCOMPARE(actual.portName, QString("COM43"));
        QCOMPARE(actual.parity, QSerialPort::EvenParity);
        presenter.execute("pbtn_RefreshSerial");
        const auto ports = view.output.value("cbox_SerialNameChoose").toMap();
        QCOMPARE(ports.value("items").toStringList().size(), ports.value("data").toList().size());
        QVERIFY(view.output.value("cbox_LocakIP")
                    .toMap()
                    .value("items")
                    .toStringList()
                    .contains("127.0.0.1"));
        qunsetenv("FOSD_CONFIG_FILE");
    }
    void tcpClientAndServerJcs() {
        for (int role = 0; role < 2; ++role) {
            QTemporaryDir dir;
            qputenv("FOSD_CONFIG_FILE", dir.filePath("tcp.ini").toUtf8());
            application::WorkbenchModel model;
            QTcpServer peer;
            QTcpSocket client;
            QVERIFY(peer.listen(QHostAddress::LocalHost, 0));
            const auto port = peer.serverPort();
            if (role == 1)
                peer.close();
            communication::TcpConfig cfg;
            cfg.host = "127.0.0.1";
            cfg.port = port;
            cfg.role = role ? communication::TcpRole::Server : communication::TcpRole::Client;
            cfg.listenAddress = QHostAddress::LocalHost;
            model.openTcp(cfg);
            QTcpSocket *socket = nullptr;
            if (!role) {
                QTRY_VERIFY(peer.hasPendingConnections());
                socket = peer.nextPendingConnection();
            } else {
                QTest::qWait(100);
                client.connectToHost(QHostAddress::LocalHost, port);
                QTRY_COMPARE(client.state(), QAbstractSocket::ConnectedState);
                socket = &client;
            }
            QSignalSpy frames(&model.protocol(), &protocol::ProtocolControl::frameParsed);
            QByteArray bytes(24, '\0');
            bytes.replace(0, 4, QByteArray::fromHex("fafbfafb"));
            bytes[19] = 1;
            bytes[20] = 1;
            bytes += QByteArray::fromHex("00c0c144");
            socket->write(bytes.left(13));
            socket->flush();
            QTest::qWait(30);
            QCOMPARE(frames.count(), 0);
            socket->write(bytes.mid(13) + bytes);
            socket->flush();
            QTRY_COMPARE(frames.count(), 2);
            auto frame =
                std::dynamic_pointer_cast<const core::WavelengthFrame>(model.frame()->data);
            QVERIFY(frame);
            QCOMPARE(frame->records[0].channel, quint8(16));
            QCOMPARE(frame->records[0].wavelength, 1550.f);
            model.closeChannel(communication::Channel::Tcp);
            if (!role)
                delete socket;
            qunsetenv("FOSD_CONFIG_FILE");
        }
    }
    void meanBaselineAndControl() {
        QTemporaryDir dir;
        qputenv("FOSD_CONFIG_FILE", dir.filePath("parameters.ini").toUtf8());
        application::WorkbenchModel model;
        QSignalSpy frames(&model.protocol(), &protocol::ProtocolControl::frameParsed),
            configs(&model, &application::WorkbenchModel::configurationApplied);
        auto feed = [&](int n) {
            model.protocol().parseFrame(communication::Channel::Udp,
                                        workbench_test::packet(n, n * .001));
        };
        feed(1);
        QTRY_COMPARE(frames.count(), 1);
        QString error;
        QVERIFY(model.baseline(&error));
        for (int i = 0; i < 20; ++i) {
            feed(i + 2);
            QTRY_COMPARE(frames.count(), i + 2);
        }
        QTRY_COMPARE(configs.count(), 1);
        QVERIFY(!model.state().configurationPending);
        Calculation::CalculationConfiguration c;
        QVERIFY(config::ParameterStore().read(&c, true, &error));
        QCOMPARE(c.strainCalibration.originalWavelength.size(), size_t(12));
        double mean = 0;
        for (int i = 2; i <= 21; ++i)
            mean += double(float(1550.0 + i * .001)) / 20;
        QVERIFY(qAbs(c.strainCalibration.originalWavelength[0] - mean) < 1e-8);
        const auto before = c.strainCalibration.originalWavelength;
        MemoryView view;
        ConnectionPresenter presenter(PanelId::Connection, &view);
        presenter.setModel(&model);
        view.input = {{"ckbox_CH16", true}};
        presenter.execute("pbtn_SelectCH");
        QVERIFY(config::ParameterStore().read(&c, true, &error));
        QVERIFY(c.strainCalibration.originalWavelength == before);
        QSignalSpy results(&model.calculation(), &Calculation::CalculationControl::resultReady);
        core::ModelData geometry;
        geometry[0] = {100, 0, 0};
        model.calculation().setModel(geometry);
        feed(30);
        QTRY_VERIFY(results.count() > 0);
        QVERIFY(model.setCalculationRunning(false, &error));
        QTest::qWait(50);
        const int count = results.count();
        feed(31);
        QTest::qWait(100);
        QCOMPARE(results.count(), count);
        QVERIFY(model.setCalculationRunning(true, &error));
        feed(32);
        QTRY_VERIFY(results.count() > count);
        QSignalSpy started(&model.calculation().storage(), &storage::StorageControl::started),
            stopped(&model.calculation().storage(), &storage::StorageControl::stopped);
        const auto output = dir.filePath("wavelength.csv");
        QVERIFY(model.setRecording(true, output, &error));
        QCOMPARE(model.state().recording, application::RecordingState::Starting);
        QVERIFY(!model.setRecording(true, output, &error));
        QTRY_COMPARE(started.count(), 1);
        QCOMPARE(model.state().recording, application::RecordingState::Recording);
        int previous = frames.count();
        feed(33);
        QTRY_COMPARE(frames.count(), previous + 1);
        QVERIFY(model.setRecording(false, QString(), &error));
        QCOMPARE(model.state().recording, application::RecordingState::Stopping);
        QTRY_COMPARE(stopped.count(), 1);
        QCOMPARE(model.state().recording, application::RecordingState::Stopped);
        QVERIFY(QFileInfo(output).size() > 0);
        qunsetenv("FOSD_CONFIG_FILE");
    }
};
QTEST_GUILESS_MAIN(WorkbenchModelTests)
#include "workbench_model_tests.moc"
