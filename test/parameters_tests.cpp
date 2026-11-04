#include "config/ParameterStore.h"
#include "communication/ConnectControl.h"
#include "calculation/CalculationControl.h"
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QUuid>
#include <QtTest>

using namespace fosd;
namespace {
QByteArray bytes(const QString &path) { QFile f(path); if (!f.open(QIODevice::ReadOnly)) return {}; return f.readAll(); }
}
class ParametersTests : public QObject {
    Q_OBJECT
    QTemporaryDir m_directory;
    QByteArray m_original;
    bool m_hadOverride = false;
private slots:
    void init() {
        m_hadOverride = qEnvironmentVariableIsSet("FOSD_CONFIG_FILE"); m_original = qgetenv("FOSD_CONFIG_FILE");
        qputenv("FOSD_CONFIG_FILE", m_directory.filePath(QUuid::createUuid().toString(QUuid::WithoutBraces) + ".ini").toUtf8());
    }
    void cleanup() { if (m_hadOverride) qputenv("FOSD_CONFIG_FILE", m_original); else qunsetenv("FOSD_CONFIG_FILE"); }
    void defaultsAndProtection() {
        config::ParameterStore store;
        communication::TcpConfig tcp;
        QString error;
        QVERIFY2(store.initialize(&tcp, &error), qPrintable(error));
        config::IniConfigStore templateStore(":/fosd-config/core-defaults.ini");
        config::ConfigData actual; QVERIFY(templateStore.readAll(&actual, &error));
        QVERIFY2(config::validateCoreDefaults(actual, &error), qPrintable(error));
        const auto expected = config::defaultCoreParameters();
        QCOMPARE(actual.size(), expected.size());
        // Compare textual parameter values because INI scalars are text on reload.
        for (auto section=expected.constBegin(); section!=expected.constEnd(); ++section)
            for (auto value=section.value().constBegin(); value!=section.value().constEnd(); ++value) {
                const auto read = actual.value(section.key()).value(value.key());
                if (value.value().type() == QVariant::StringList) QCOMPARE(read.toStringList(), value.value().toStringList());
                else QCOMPARE(read.toString(), value.value().toString());
            }
        config::IniConfigStore runtime(config::ParameterStore::runtimePath());
        config::ConfigData runtimeData; QVERIFY(runtime.readAll(&runtimeData)); QCOMPARE(runtimeData, actual);
        config::ParameterStore protectedStore(m_directory.filePath("core-defaults.ini"));
        QVERIFY(!protectedStore.write(tcp, false, &error));
        QVERIFY(!QFile::exists(m_directory.filePath("core-defaults.ini")));
    }
    void preserveAndValidate() {
        config::ParameterStore store;
        communication::TcpConfig tcp;
        QVERIFY(store.initialize(&tcp)); tcp.port=30001; tcp.host="test.example";
        QVERIFY(store.write(tcp));
        communication::TcpConfig restored;
        QVERIFY(store.initialize(&restored)); QCOMPARE(restored.port, quint16(30001)); QCOMPARE(restored.host, tcp.host);
        config::IniConfigStore raw(config::ParameterStore::runtimePath());
        const auto before = bytes(raw.filePath());
        tcp.role=communication::TcpRole(99); QString error;
        QVERIFY(!store.write(tcp, true, &error)); QVERIFY(!error.isEmpty()); QCOMPARE(bytes(raw.filePath()), before);
        config::ConfigSection values; QVERIFY(raw.readModule("communication.tcp", &values)); values["port"]="70000";
        QVERIFY(raw.writeModule("communication.tcp", values)); const auto invalid = bytes(raw.filePath());
        QVERIFY(!store.initialize(&restored, &error)); QCOMPARE(restored.port, quint16(30001)); QCOMPARE(bytes(raw.filePath()), invalid);
        values["port"]=30001; values["role"]=0; QVERIFY(raw.writeModule("communication.tcp", values));
        config::IniConfigStore defaults(":/fosd-config/core-defaults.ini");
        config::ConfigData data; QVERIFY(defaults.readAll(&data));
        QVERIFY(raw.ensureDefaults(data)); QVERIFY(store.read(&restored)); QCOMPARE(restored.host, QStringLiteral("test.example"));
    }
    void moduleConstructionAndUpdates() {
        config::ParameterStore store;
        const auto source = bytes(FOSD_PARAMETER_TEMPLATE_SOURCE);
        {
            communication::ConnectControl connection;
            Calculation::CalculationControl calculation;
            QSignalSpy errors(&connection, &communication::ConnectControl::errorOccurred);
            communication::TcpConfig invalid; invalid.role=communication::TcpRole(99);
            const auto before = bytes(config::ParameterStore::runtimePath());
            connection.openTcp(invalid); QCOMPARE(errors.count(), 1); QCOMPARE(bytes(config::ParameterStore::runtimePath()), before);
            communication::UdpConfig udp; udp.bindAddress=QHostAddress::LocalHost;
            connection.openUdp(udp);
            communication::UdpConfig saved; QVERIFY(store.read(&saved)); QCOMPARE(saved.bindAddress, udp.bindAddress);
            Calculation::CalculationConfiguration config;
            config.strainCalibration.A=1;
            config.strainCalibration.originalWavelength={1500,1501};
            config.channelCalibration.channelIndices={0,1}; config.channelCalibration.calibrationFactors={1,1};
            QSignalSpy configured(&calculation, &Calculation::CalculationControl::configurationAccepted);
            calculation.setConfiguration(config); QTRY_COMPARE(configured.count(),1);
            Calculation::CalculationConfiguration restored; QVERIFY(store.read(&restored,true));
            QCOMPARE(restored.strainCalibration.originalWavelength, config.strainCalibration.originalWavelength);
            storage::StorageConfiguration recording; recording.path=m_directory.filePath("data.csv");
            QSignalSpy started(&calculation.storage(), &storage::StorageControl::started);
            calculation.storage().start(recording); QTRY_COMPARE(started.count(),1);
            storage::StorageConfiguration savedRecording; QVERIFY(store.read(&savedRecording,true)); QCOMPARE(savedRecording.path,recording.path);
        }
        QCOMPARE(bytes(FOSD_PARAMETER_TEMPLATE_SOURCE), source);
        communication::ConnectControl reconstructed;
        communication::UdpConfig restored; QVERIFY(store.read(&restored)); QCOMPARE(restored.bindAddress, QHostAddress(QHostAddress::LocalHost));
    }
};
QTEST_GUILESS_MAIN(ParametersTests)
#include "parameters_tests.moc"
