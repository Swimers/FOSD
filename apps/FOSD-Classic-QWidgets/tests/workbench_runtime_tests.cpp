#include "backend/WorkbenchRuntime.h"
#include "FOSD-Classic-QWidgets/apps/WorkbenchBackend.h"
#include "workbench_test_packets.h"
#include <QSignalSpy>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTemporaryDir>
#include <QtTest>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
using namespace fosd;
using namespace fosd::ui::classic;
class WorkbenchRuntimeTests : public QObject {
    Q_OBJECT
private slots:
    void unavailableUsesFeatureBackend();
    void plateConfigurationAndSharedControls();
    void clearModelStopsInterpolation();
    void storage_data();
    void storage();
};
static UiRequest request(PanelId panel, const char *action) {
    UiRequest r;
    r.id = 73;
    r.panel = panel;
    r.action = QString::fromLatin1(action);
    return r;
}

void WorkbenchRuntimeTests::unavailableUsesFeatureBackend() {
    application::WorkbenchRuntime runtime;
    WorkbenchBackend backends;
    backends.setServices(&runtime);
    const QVector<UiRequest> requests{request(PanelId::Connection, "pbtn_CmdStart"),
                                      request(PanelId::ModelPanel, "pbtn_ModelMerge"),
                                      request(PanelId::ScenePanel, "cbox_FellowAble"),
                                      request(PanelId::ColorPicker, "accepted"),
                                      request(PanelId::TrackingManager, "pbtn_LoadFile"),
                                      request(PanelId::Login, "pbtn_Login")};
    for (const auto &r : requests) {
        auto *backend = backends.backend(featureForPanel(r.panel));
        QSignalSpy completed(backend, &IBackend::completed);
        backend->execute(r);
        QCOMPARE(completed.size(), 1);
        const auto response = qvariant_cast<UiResponse>(completed.first()[0]);
        QVERIFY(!response.success);
        QCOMPARE(response.id, r.id);
        QVERIFY(response.message.contains(r.action));
        QVERIFY(response.message.contains(QString::fromUtf8("尚未实现")));
    }
}

void WorkbenchRuntimeTests::plateConfigurationAndSharedControls() {
    application::WorkbenchRuntime runtime;
    QSignalSpy configured(&runtime.calculation(),
                          &Calculation::CalculationControl::configurationAccepted);
    QSignalSpy models(&runtime.calculation(), &Calculation::CalculationControl::modelAccepted);
    QSignalSpy hardware(&runtime.calculation(), &Calculation::CalculationControl::calibratedReady);
    QSignalSpy solved(&runtime.calculation(), &Calculation::CalculationControl::resultReady);
    QString error;
    QVERIFY2(runtime.loadPlate(&error), qPrintable(error));
    QTRY_COMPARE(configured.size(), 1);
    QTRY_COMPARE(models.size(), 1);
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(1));
    QTRY_COMPARE(solved.size(), 1);
    QTRY_COMPARE(hardware.size(), 1);
    const auto result = runtime.calculation().dataModel()->latest();
    QVERIFY(result);
    QCOMPARE(result->algorithmId, QStringLiteral("demo.plate.modal-ife"));
    QVERIFY(result->deformedModel.size() > 100);
    double maximum = 0;
    for (const auto &d : result->displacementMap)
        maximum = qMax(maximum, std::abs(d[2]));
    QVERIFY(maximum > 10 && maximum < 15);
    UiResponse response;
    auto r = request(PanelId::ScenePanel, "cbox_AxisAble");
    r.fields[r.action] = true;
    QVERIFY(runtime.handleRequest(r, &response));
    QVERIFY(response.success);
    QVERIFY(runtime.visualization().axesSettings().visible);
    r = request(PanelId::ModelPanel, "pbtn_ModelExport");
    QVERIFY(!runtime.handleRequest(r, &response));
    r = request(PanelId::Connection, "pbtn_ParaSet");
    r.fields = {{"textEdit_APara", "0"}, {"textEdit_BPara", "0"}};
    QVERIFY(runtime.handleRequest(r, &response));
    QVERIFY(!response.success);
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(2));
    QTRY_VERIFY(solved.size() >= 2);
    QCOMPARE(runtime.calculation().dataModel()->latest()->algorithmId,
             QStringLiteral("demo.plate.modal-ife"));
    r = request(PanelId::Visualization, "actionHYY");
    QVERIFY(runtime.handleRequest(r, &response));
    QVERIFY(response.success);
    QTRY_COMPARE(configured.size(), 2);
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(3));
    QTRY_COMPARE(std::static_pointer_cast<const core::WavelengthFrame>(
                     runtime.calculation().dataModel()->latest()->sourceFrame->data)
                     ->sequence,
                 quint32(3));
    QCOMPARE(runtime.calculation().dataModel()->latest()->algorithmId, QStringLiteral("HYY.IFE"));
}

void WorkbenchRuntimeTests::clearModelStopsInterpolation() {
    application::WorkbenchRuntime runtime;
    QString error;
    QSignalSpy configured(&runtime.calculation(),
                          &Calculation::CalculationControl::configurationAccepted);
    QSignalSpy models(&runtime.calculation(), &Calculation::CalculationControl::modelAccepted);
    QSignalSpy hardware(&runtime.calculation(), &Calculation::CalculationControl::calibratedReady);
    QSignalSpy solved(&runtime.calculation(), &Calculation::CalculationControl::resultReady);
    QVERIFY2(runtime.loadPlate(&error), qPrintable(error));
    QTRY_COMPARE(configured.size(), 1);
    QTRY_COMPARE(models.size(), 1);
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(1));
    QTRY_COMPARE(solved.size(), 1);
    UiResponse response;
    QVERIFY(runtime.handleRequest(request(PanelId::ModelPanel, "pbtn_ModelClear"), &response));
    QVERIFY(response.success);
    QTRY_VERIFY(!runtime.calculation().dataModel()->latest());
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(2));
    QTRY_VERIFY(!hardware.isEmpty() &&
                std::static_pointer_cast<const core::WavelengthFrame>(
                    qvariant_cast<core::CalibratedDataPtr>(hardware.last()[0])->sourceFrame->data)
                        ->sequence == 2);
    QCOMPARE(solved.size(), 1);
    QVERIFY(!runtime.visualization().originalModel());
    QTemporaryDir directory;
    const auto path = directory.filePath("plate.stl");
    QVERIFY(QFile::copy(":/workbench/plate.stl", path));
    // Queue load/clear/load without pumping GUI events: stale acknowledgements
    // must not restore the cleared model or bind results to its replacement.
    QVERIFY(runtime.visualization().loadModel(path));
    QVERIFY(runtime.handleRequest(request(PanelId::ModelPanel, "pbtn_ModelClear"), &response));
    QVERIFY(runtime.visualization().loadModel(path));
    QTRY_COMPARE(models.size(), 3);
    QTRY_VERIFY(runtime.calculation().dataModel()->latest() &&
                runtime.calculation().dataModel()->latest()->modelRevision == 3);
    runtime.visualization().applyResult(runtime.calculation().dataModel()->latest());
    const auto result = runtime.calculation().dataModel()->latest();
    for (auto point = result->displacementMap.constBegin();
         point != result->displacementMap.constEnd(); ++point)
        if (std::abs(point.value()[2]) > 10) {
            const auto selected = runtime.visualization().selectPoint(vtkIdType(point.key()));
            QVERIFY(selected.valid);
            QCOMPARE(selected.displacementMm[2], point.value()[2]);
            break;
        }
}

void WorkbenchRuntimeTests::storage_data() {
    QTest::addColumn<QString>("suffix");
    QTest::newRow("CSV") << QStringLiteral("csv");
    QTest::newRow("SQLite") << QStringLiteral("sqlite");
}

void WorkbenchRuntimeTests::storage() {
    QFETCH(QString, suffix);
    QTemporaryDir directory;
    QVERIFY(directory.isValid());
    application::WorkbenchRuntime runtime;
    QSignalSpy configured(&runtime.calculation(),
                          &Calculation::CalculationControl::configurationAccepted);
    QSignalSpy models(&runtime.calculation(), &Calculation::CalculationControl::modelAccepted);
    QSignalSpy started(&runtime.calculation().storage(), &storage::StorageControl::started);
    QSignalSpy stopped(&runtime.calculation().storage(), &storage::StorageControl::stopped);
    QSignalSpy batches(&runtime.calculation().storage(), &storage::StorageControl::batchWritten);
    QString error;
    QVERIFY2(runtime.loadPlate(&error), qPrintable(error));
    QTRY_COMPARE(configured.size(), 1);
    QTRY_COMPARE(models.size(), 1);
    UiResponse response;
    auto r = request(PanelId::Connection, "pbtn_SaveDynamic");
    r.fields = {{"pbtn_SaveDynamic", true}, {"filePath", directory.filePath("frames." + suffix)}};
    QVERIFY(runtime.handleRequest(r, &response));
    QVERIFY(response.success);
    QTRY_COMPARE(started.size(), 1);
    runtime.protocol().parseFrame(communication::Channel::Udp, workbench_test::packet(2));
    QTRY_VERIFY(batches.size() >= 3);
    r.fields["pbtn_SaveDynamic"] = false;
    QVERIFY(runtime.handleRequest(r, &response));
    QTRY_COMPARE(stopped.size(), 1);
    auto snapshotRequest = request(PanelId::Connection, "pbtn_SaveStatic");
    const QString snapshotPath = directory.filePath("snapshot." + suffix);
    snapshotRequest.fields["filePath"] = snapshotPath;
    QVERIFY(runtime.handleRequest(snapshotRequest, &response));
    QVERIFY(response.success);
    QTRY_COMPARE(started.size(), 2);
    QTRY_COMPARE(stopped.size(), 2);
    if (suffix == "csv") {
        QFile file(directory.filePath("frames.csv"));
        QVERIFY(file.open(QIODevice::ReadOnly));
        const auto contents = file.readAll();
        QVERIFY(contents.contains("wavelength"));
        QVERIFY(contents.contains("calibrated_hardware"));
        QVERIFY(contents.contains("point_displacement"));
        QFile snapshot(snapshotPath);
        QVERIFY(snapshot.open(QIODevice::ReadOnly));
        QCOMPARE(snapshot.readAll().count('\n'), 13);
    } else {
        const QString connection = "workbench-test";
        {
            auto db = QSqlDatabase::addDatabase("QSQLITE", connection);
            db.setDatabaseName(directory.filePath("frames.sqlite"));
            QVERIFY(db.open());
            QSqlQuery query(db);
            QVERIFY(query.exec("SELECT COUNT(DISTINCT data_type) FROM fosd_records"));
            QVERIFY(query.next());
            QCOMPARE(query.value(0).toInt(), 3);
            db.close();
        }
        QSqlDatabase::removeDatabase(connection);
        {
            auto db = QSqlDatabase::addDatabase("QSQLITE", connection);
            db.setDatabaseName(snapshotPath);
            QVERIFY(db.open());
            QSqlQuery query(db);
            QVERIFY(query.exec("SELECT COUNT(*) FROM fosd_records"));
            QVERIFY(query.next());
            QCOMPARE(query.value(0).toInt(), 12);
            db.close();
        }
        QSqlDatabase::removeDatabase(connection);
    }
}

QTEST_GUILESS_MAIN(WorkbenchRuntimeTests)
#include "workbench_runtime_tests.moc"
