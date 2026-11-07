#include "ParameterTestIsolation.h"
#include "visualization/marker/KeyPointCreator.h"
#include "visualization/marker/KeyPointManager.h"
#include "visualization/interaction/PointPicker.h"
#include "visualization/interaction/ModelInteractorStyle.h"
#include "visualization/VisualizationControl.h"
#include <QtTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QFile>
#include <limits>
#include <vtkActor.h>
#include <vtkBillboardTextActor3D.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkRenderer.h>
#include <vtkPropCollection.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
using namespace fosd;
using namespace fosd::visualization;
namespace {
core::ModelDataPtr model() {
    return std::make_shared<const core::ModelData>(
        core::ModelData{{0, {100, 0, 0}}, {1, {700, 50, 0}}, {2, {1300, 0, 0}}});
}

vtkSmartPointer<vtkPolyData> mesh() {
    auto data = vtkSmartPointer<vtkPolyData>::New();
    auto points = vtkSmartPointer<vtkPoints>::New();
    const auto original = model();
    for (const auto &xyz : *original)
        points->InsertNextPoint(xyz[0], xyz[1], xyz[2]);
    data->SetPoints(points);
    return data;
}

KeyPointDefinition definition(vtkIdType id, const QString &label = {}) {
    KeyPointDefinition p;
    p.pointId = id;
    p.label = label;
    return p;
}

DisplayedPointDefinition display(vtkIdType id) {
    DisplayedPointDefinition point;
    point.pointId = id;
    return point;
}
} // namespace
class KeyPointInteractionTests final : public QObject {
    Q_OBJECT
    ParameterTestIsolation m_parameterIsolation;
private slots:
    void init() {
        m_parameterIsolation.reset();
    }

    void creator() {
        KeyPointStyle style;
        auto actors = KeyPointCreator::create(42, {{1, 2, 3}}, "tip", style);
        QVERIFY(actors.sphere);
        QVERIFY(actors.label);
        QCOMPARE(actors.pointId, vtkIdType(42));
        QCOMPARE(actors.source->GetRadius(), 5.0);
        QVERIFY(QString::fromUtf8(actors.label->GetInput()).startsWith("tip\nX: 1"));
        QCOMPARE(actors.sphere->GetPosition()[0], 1.0);
        QCOMPARE(actors.label->GetPosition()[2], 11.0);
        QVERIFY(!actors.sphere->GetPickable());
        QVERIFY(!actors.label->GetPickable());
        style.radiusMm = 9;
        style.fontSize = 21;
        style.labelOffsetMm = {{3, 4, 5}};
        KeyPointCreator::configure(actors, style);
        KeyPointCreator::setPosition(actors, {{7, 8, 9}}, style);
        QCOMPARE(actors.source->GetRadius(), 9.0);
        QCOMPARE(actors.label->GetPosition()[0], 10.0);
        QCOMPARE(actors.label->GetTextProperty()->GetFontSize(), 21);
        style.radiusMm = -1;
        QVERIFY(!KeyPointCreator::validateStyle(style));
        QVERIFY(!KeyPointCreator::create(1, {{0, 0, 0}}, {}, style).sphere);
    }
    void managerLifecycle() {
        auto first = vtkSmartPointer<vtkRenderer>::New(),
             second = vtkSmartPointer<vtkRenderer>::New();
        auto data = mesh();
        KeyPointManager manager;
        manager.setRenderer(first);
        manager.setModel(model(), data);
        QCOMPARE(manager.keyPoints().size(), 3);
        QVERIFY(manager.contains(1));
        QVERIFY(manager.displayedPoints().isEmpty());
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 0);
        QVERIFY(manager.setKeyPoints({definition(0, "root"), definition(2, "tip")}));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 0);
        QVERIFY(manager.setDisplayedPoints({display(0), display(2)}));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 4);
        QVERIFY(!manager.setKeyPoints({definition(0), definition(0)}));
        QCOMPARE(manager.keyPoints().size(), 2);
        QVERIFY(!manager.setKeyPoints({definition(99)}));
        QVERIFY(manager.setSelectedPoint(0));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 6);
        QVERIFY(manager.setSelectedPoint(0));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 6);
        manager.clearSelection();
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 4);
        manager.setVisible(false);
        auto *props = first->GetViewProps();
        props->InitTraversal();
        while (auto *prop = props->GetNextProp())
            QVERIFY(!prop->GetVisibility());
        manager.setVisible(true);
        auto style = manager.style();
        style.labelsVisible = false;
        QVERIFY(manager.setStyle(style));
        data->GetPoints()->SetPoint(0, 111, 222, 333);
        data->Modified();
        manager.updatePositions();
        props->InitTraversal();
        bool found = false;
        while (auto *prop = props->GetNextProp())
            if (auto *sphere = vtkActor::SafeDownCast(prop)) {
                if (sphere->GetPosition()[0] == 111) {
                    found = true;
                    QCOMPARE(sphere->GetPosition()[2], 333.0);
                }
            }
        QVERIFY(found);
        manager.setRenderer(second);
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 0);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 4);
        manager.setRenderer(second);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 4);
        manager.setModel({}, nullptr);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 0);
    }
    void selectionAndLocator() {
        auto data = mesh();
        KeyPointManager manager;
        manager.setModel(model(), data);
        QVERIFY(manager.setKeyPoints({definition(0), definition(2)}));
        PointPicker picker;
        picker.setKeyPointManager(&manager);
        picker.setMesh(data, model());
        QSignalSpy picked(&picker, &PointPicker::pointPicked),
            updated(&picker, &PointPicker::selectionUpdated);
        auto info = picker.selectPoint(2);
        QVERIFY(info.valid);
        QCOMPARE(info.pointId, vtkIdType(2));
        QCOMPARE(info.originalPositionMm[0], 1300.0);
        core::CalculationResult result;
        result.displacementMap.insert(2, {2, 3, 4});
        picker.setResult(std::make_shared<const core::CalculationResult>(result));
        data->GetPoints()->SetPoint(2, 1304, 6, 8);
        data->GetPoints()->Modified();
        picker.refreshSelection();
        info = picker.selection();
        QCOMPARE(info.displacementMm[0], 2.0);
        QCOMPARE(info.displayPositionMm[0], 1304.0);
        QCOMPARE(updated.count(), 1);
        const auto nearby = picker.nearbyPoints({{100, 0, 0}}, 1);
        QCOMPARE(data->GetPoint(0)[0], 100.0);
        QCOMPARE(nearby.size(), 1);
        QCOMPARE(nearby.value(0), vtkIdType(0));
        data->GetPoints()->SetPoint(0, 1500, 0, 0);
        data->GetPoints()->Modified();
        data->Modified();
        QVERIFY(picker.nearbyPoints({{100, 0, 0}}, 1).isEmpty());
        const auto movedNearby = picker.nearbyPoints({{1500, 0, 0}}, 1);
        QCOMPARE(movedNearby.size(), 1);
        QCOMPARE(movedNearby.value(0), vtkIdType(0));
        picker.cameraInteractionStarted();
        QVERIFY(picker.selection().valid);
        auto settings = picker.settings();
        QVERIFY(manager.setKeyPoints({definition(0)}));
        picker.refreshSelection();
        QVERIFY(!picker.selectPoint(2).valid); // Cannot bypass membership with onlyKeyPoints=false.
        settings.onlyKeyPoints = true;
        QVERIFY(picker.setSettings(settings));
        QVERIFY(!picker.selection().valid);
        QVERIFY(!picker.selectPoint(2).valid);
        QVERIFY(picker.selectPoint(0).valid);
        settings.clearOnCameraInteraction = true;
        QVERIFY(picker.setSettings(settings));
        picker.cameraInteractionStarted();
        QVERIFY(!picker.selection().valid);
        settings.tolerance = -1;
        QVERIFY(!picker.setSettings(settings));
        QVERIFY(picker.settings().tolerance > 0);
        settings = picker.settings();
        settings.enabled = false;
        QVERIFY(picker.setSettings(settings));
        QVERIFY(!picker.selectPoint(0).valid);
        QCOMPARE(manager.keyPoints().size(), 1);
        QVERIFY(picked.count() > 0);
        picker.setMesh(nullptr, {});
    }
    void dragDoesNotDispatchClick() {
        auto data = mesh();
        KeyPointManager manager;
        manager.setModel(model(), data);
        PointPicker picker;
        picker.setKeyPointManager(&manager);
        picker.setMesh(data, model());
        auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        auto style = vtkSmartPointer<ModelInteractorStyle>::New();
        style->setPointPicker(&picker);
        style->SetInteractor(interactor);
        style->setCameraLocked(true);
        QVERIFY(picker.selectPoint(0).valid);
        QSignalSpy events(&picker, &PointPicker::pointPicked);
        interactor->SetEventPosition(10, 10);
        style->OnLeftButtonDown();
        interactor->SetEventPosition(40, 40);
        style->OnMouseMove();
        interactor->SetEventPosition(10, 10);
        style->OnLeftButtonUp();
        QCOMPARE(events.count(), 0);
        QVERIFY(picker.selection().valid); // Moving out and back is a drag.
        style->OnLeftButtonDown();
        style->OnLeftButtonUp();
        QCOMPARE(events.count(), 1); // A click attempts a pick (no renderer -> clear).
        style->SetInteractor(nullptr);
    }
    void controlDeformationAndReset() {
        QTemporaryDir directory;
        QFile file(directory.filePath("test.stl"));
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("solid t\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 "
                   "0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid t\n");
        file.close();
        VisualizationControl control;
        QVERIFY(control.loadModel(file.fileName()));
        QCOMPARE(control.keyPoints().size(), 3);
        QVERIFY(control.displayedPoints().isEmpty());
        control.setExpectedModelRevision(7);
        QVERIFY(control.setKeyPoints({definition(0), definition(2)}));
        auto external = display(100);
        external.source = DisplayedPointSource::External;
        external.positionMm = {{50, 60, 70}};
        QVERIFY(control.setDisplayedPoints({display(2), external}));
        QVERIFY(control.selectPoint(2).valid);
        core::CalculationResult result;
        result.revision = 1;
        result.modelRevision = 7;
        result.displacementMap = {{0, {1, 2, 3}}, {1, {2, 3, 4}}, {2, {3, 4, 5}}};
        control.applyResult(std::make_shared<const core::CalculationResult>(result));
        auto info = control.selectedPoint();
        QCOMPARE(info.displacementMm[0], 3.0);
        QCOMPARE(info.displayPositionMm[0], info.originalPositionMm[0] + 3);
        QCOMPARE(control.displayedPointInfos()[0].displayPositionMm[0], info.displayPositionMm[0]);
        QCOMPARE(control.displayedPointInfos()[1].displayPositionMm[0], 50.0);
        QCOMPARE(control.displayedPointInfos()[1].displacementMm[0], 0.0);
        control.setDeformationScale(10);
        info = control.selectedPoint();
        QCOMPARE(info.displayPositionMm[0], info.originalPositionMm[0] + 30);
        QCOMPARE(info.displacementMm[0], 3.0);
        QCOMPARE(control.displayedPointInfos()[0].displayPositionMm[0], info.displayPositionMm[0]);
        QCOMPARE(control.displayedPointInfos()[0].displacementMm[0], 3.0);
        QCOMPARE(control.displayedPointInfos()[1].displayPositionMm[0], 50.0);
        control.resetDeformation();
        info = control.selectedPoint();
        QVERIFY(info.valid);
        QCOMPARE(info.displacementMm[0], 0.0);
        QCOMPARE(info.displayPositionMm[0], info.originalPositionMm[0]);
        auto invalid = control.keyPointStyle();
        invalid.radiusMm = -1;
        QVERIFY(!control.setKeyPointStyle(invalid));
        QCOMPARE(control.keyPointStyle().radiusMm, 5.0);
        control.clearSelection();
        QVERIFY(!control.selectedPoint().valid);
        QCOMPARE(control.keyPoints().size(), 2);
        QVERIFY(control.selectPoint(2).valid);
        QVERIFY(control.setKeyPoints({definition(0)}));
        QVERIFY(!control.selectedPoint().valid);
        QCOMPARE(control.displayedPoints().size(), 1);
        QCOMPARE(control.displayedPoints()[0].pointId, vtkIdType(100));
        control.unloadModel();
        QVERIFY(control.keyPoints().isEmpty());
        QVERIFY(control.displayedPoints().isEmpty());
        QVERIFY(!control.selectedPoint().valid);
    }

    void displayedConstraintsAndFixedCoordinates() {
        auto renderer = vtkSmartPointer<vtkRenderer>::New();
        auto data = mesh();
        KeyPointManager manager;
        manager.setRenderer(renderer);
        manager.setModel(model(), data);
        QVERIFY(manager.setKeyPoints({definition(0)}));
        auto external = display(99);
        external.source = DisplayedPointSource::External;
        external.positionMm = {{2000, 3000, 4000}};
        QVERIFY(manager.setDisplayedPoints({display(0), external}));
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 4);
        QVERIFY(!manager.setDisplayedPoints({display(1)}));
        QVERIFY(!manager.setSelectedPoint(1));
        QVERIFY(!manager.setSelectedPoint(99));
        auto impostor = external;
        impostor.pointId = 1;
        QVERIFY(!manager.setDisplayedPoints({impostor}));
        external.positionMm[0] = std::numeric_limits<double>::quiet_NaN();
        QVERIFY(!manager.setDisplayedPoints({external}));
        QVERIFY(!manager.setDisplayedPoints({display(0), display(0)}));
        QCOMPARE(manager.displayedPoints().size(), 2);
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 4);
        QVERIFY(manager.setSelectedPoint(0));
        data->GetPoints()->SetPoint(0, 111, 222, 333);
        data->Modified();
        manager.updatePositions();
        auto *props = renderer->GetViewProps();
        props->InitTraversal();
        int movedSpheres = 0, fixedSpheres = 0, updatedLabels = 0;
        while (auto *prop = props->GetNextProp()) {
            if (auto *sphere = vtkActor::SafeDownCast(prop)) {
                if (sphere->GetPosition()[0] == 111)
                    ++movedSpheres;
                if (sphere->GetPosition()[0] == 2000)
                    ++fixedSpheres;
            }
            if (auto *label = vtkBillboardTextActor3D::SafeDownCast(prop))
                if (QString::fromUtf8(label->GetInput()).contains("X: 111"))
                    ++updatedLabels;
        }
        QCOMPARE(movedSpheres, 2); // Selected and persistent markers refresh independently.
        QCOMPARE(fixedSpheres, 1);
        QCOMPARE(updatedLabels, 2);
        manager.setVisible(false);
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 6);
        manager.clearSelection();
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 4);
        QVERIFY(manager.setKeyPoints({}));
        QCOMPARE(manager.displayedPoints().size(), 1); // External display survives key edits.
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 2);
    }
};
QTEST_GUILESS_MAIN(KeyPointInteractionTests)
#include "keypoint_interaction_tests.moc"
