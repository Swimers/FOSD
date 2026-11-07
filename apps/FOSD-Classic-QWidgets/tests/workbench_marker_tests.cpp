#include "backend/WorkbenchRuntime.h"
#include "FOSD-Classic-QWidgets/apps/model/backends/ModelBackend.h"
#include "FOSD-Classic-QWidgets/apps/model/presenters/ModelPresenter.h"
#include "FOSD-Classic-QWidgets/apps/model/views/disp3dkeypointedit.h"
#include "FOSD-Classic-QWidgets/apps/model/views/disp3dkeypointselect.h"
#include "../../../test/ParameterTestIsolation.h"
#include <QComboBox>
#include <QFile>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSignalSpy>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QtTest>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

using namespace fosd;
using namespace fosd::ui::classic;

class WorkbenchMarkerTests final : public QObject {
    Q_OBJECT
    ParameterTestIsolation m_parameters;

private slots:
    void init() {
        m_parameters.reset();
    }

    void editAndDisplayThroughPresenters() {
        QTemporaryDir directory;
        QFile file(directory.filePath("model.stl"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("solid t\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 "
                   "0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid t\n");
        file.close();
        application::WorkbenchRuntime runtime;
        ModelBackend backend;
        backend.setServices(&runtime);
        KeyPointEditView edit;
        KeyPointSelectView display;
        ModelPresenter editPresenter(PanelId::KeyPointEdit, &edit);
        ModelPresenter displayPresenter(PanelId::KeyPointSelect, &display);
        editPresenter.setBackend(&backend);
        displayPresenter.setBackend(&backend);
        auto *combo = display.findChild<QComboBox *>("cbox_KeyPointID");
        auto *source = display.findChild<QComboBox *>("cbox_PointSource");
        auto *table = display.findChild<QTableWidget *>("tablew_PointPara");
        QVERIFY(combo && source && table);
        QVERIFY(runtime.visualization().loadModel(file.fileName()));
        QTRY_COMPARE(combo->count(), 3);
        QCOMPARE(runtime.visualization().keyPoints().size(), 3);
        QVERIFY(runtime.visualization().displayedPoints().isEmpty());

        combo->setCurrentText("1");
        display.findChild<QPushButton *>("pbtn_AddPoint")->click();
        QTRY_COMPARE(runtime.visualization().displayedPoints().size(), 1);
        QCOMPARE(runtime.visualization().keyPoints().size(), 3);
        QVERIFY(runtime.visualization().selectPoint(1).valid);
        edit.findChild<QLineEdit *>("edit_IDKeyPoint")->setText("1");
        edit.findChild<QPushButton *>("pbtn_RemoveKeyPoint")->click();
        QTRY_COMPARE(runtime.visualization().keyPoints().size(), 2);
        QVERIFY(!runtime.visualization().selectedPoint().valid);
        QVERIFY(runtime.visualization().displayedPoints().isEmpty());
        QTRY_COMPARE(combo->count(), 2);

        combo->setCurrentText("2");
        display.findChild<QPushButton *>("pbtn_AddPoint")->click();
        QTRY_COMPARE(table->rowCount(), 1);
        source->setCurrentIndex(1);
        displayPresenter.execute("cbox_PointSource");
        QTRY_VERIFY(display.findChild<QLineEdit *>("edit_ExternalX")->isEnabled());
        display.findChild<QLineEdit *>("edit_ExternalPointID")->setText("99");
        display.findChild<QLineEdit *>("edit_ExternalX")->setText("2000");
        display.findChild<QLineEdit *>("edit_ExternalY")->setText("3000");
        display.findChild<QLineEdit *>("edit_ExternalZ")->setText("4000");
        display.findChild<QPushButton *>("pbtn_AddPoint")->click();
        QTRY_COMPARE(table->rowCount(), 2);
        QCOMPARE(runtime.visualization().keyPoints().size(), 2);
        QCOMPARE(runtime.visualization().displayedPointInfos()[1].displayPositionMm[0], 2000.0);

        // A non-key model vertex cannot be disguised as an external point.
        QSignalSpy responses(&backend, &ModelBackend::completed);
        display.findChild<QLineEdit *>("edit_ExternalPointID")->setText("1");
        display.findChild<QPushButton *>("pbtn_AddPoint")->click();
        QTRY_COMPARE(responses.count(), 1);
        QVERIFY(!qvariant_cast<UiResponse>(responses.last()[0]).success);
        QCOMPARE(table->rowCount(), 2);

        // Deformation refresh preserves selected row IDs, enabling safe deletion during updates.
        table->selectRow(0);
        runtime.visualization().setExpectedModelRevision(7);
        core::CalculationResult result;
        result.revision = 1;
        result.modelRevision = 7;
        result.displacementMap = {{0, {1, 2, 3}}, {1, {2, 3, 4}}, {2, {3, 4, 5}}};
        runtime.visualization().applyResult(
            std::make_shared<const core::CalculationResult>(result));
        display.findChild<QRadioButton *>("radioButton")->click();
        QTRY_COMPARE(table->item(0, 1)->text(), QString("3"));
        QTRY_VERIFY(display.values()
                        .value("tablew_PointPara.selectedIds")
                        .toList()
                        .contains(QVariant(qlonglong(2))));
        display.findChild<QPushButton *>("pbtn_DeletePoint")->click();
        QTRY_COMPARE(runtime.visualization().displayedPoints().size(), 1);
        QCOMPARE(runtime.visualization().displayedPoints()[0].pointId, vtkIdType(99));
        QCOMPARE(runtime.visualization().keyPoints().size(), 2);

        edit.findChild<QPushButton *>("pbtn_ClearAll")->click();
        QTRY_VERIFY(runtime.visualization().keyPoints().isEmpty());
        QCOMPARE(runtime.visualization().displayedPoints().size(), 1);
        // Region editing uses original coordinates, even after the mesh has deformed.
        const char *names[] = {"edit_Box1X", "edit_Box1Y", "edit_Box1Z",
                               "edit_Box2X", "edit_Box2Y", "edit_Box2Z"};
        const char *values[] = {"99", "-1", "-1", "101", "1", "1"};
        for (int i = 0; i < 6; ++i)
            edit.findChild<QLineEdit *>(names[i])->setText(values[i]);
        edit.findChild<QPushButton *>("pbtn_AddBox")->click();
        QTRY_COMPARE(runtime.visualization().keyPoints().size(), 1);
        QCOMPARE(runtime.visualization().keyPoints()[0].pointId, vtkIdType(0));
        edit.findChild<QPushButton *>("pbtn_RemoveBox")->click();
        QTRY_VERIFY(runtime.visualization().keyPoints().isEmpty());
        edit.findChild<QPushButton *>("pbtn_SelectAll")->click();
        QTRY_COMPARE(runtime.visualization().keyPoints().size(), 3);
        display.findChild<QPushButton *>("pbtn_DisplayAllKeys")->click();
        QTRY_COMPARE(runtime.visualization().displayedPoints().size(), 4);
        display.findChild<QPushButton *>("pbtn_ClearDisplayed")->click();
        QTRY_VERIFY(runtime.visualization().displayedPoints().isEmpty());
        QCOMPARE(runtime.visualization().keyPoints().size(), 3);
    }
};

QTEST_MAIN(WorkbenchMarkerTests)
#include "workbench_marker_tests.moc"
