#include "visualization/scene/SceneEnvironment.h"
#include "visualization/VisualizationControl.h"
#include <QtTest>
#include <QImage>
#include <QTemporaryDir>
#include <QFile>
#include <QSignalSpy>
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkPropCollection.h>
#include <vtkCamera.h>
#include <vtkTexture.h>
#include <vtkAutoInit.h>
#include <cmath>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
using namespace fosd;
using namespace fosd::visualization;
namespace {
vtkSmartPointer<vtkActor> model() {
    auto source = vtkSmartPointer<vtkSphereSource>::New();
    source->SetRadius(50);
    source->SetCenter(700, 20, 30);
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(source->GetOutputPort());
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    return actor;
}
} // namespace
class SceneEnvironmentTests final : public QObject {
    Q_OBJECT
private slots:
    void axesAndGridLifecycle() {
        auto first = vtkSmartPointer<vtkRenderer>::New(),
             second = vtkSmartPointer<vtkRenderer>::New();
        SceneEnvironment scene;
        scene.setRenderer(first);
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 0);
        AxesSettings axes;
        axes.visible = true;
        axes.lengthMm = 321;
        axes.labelsVisible = false;
        axes.originMm = {{10, 20, 30}};
        QVERIFY(scene.setAxesSettings(axes));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 1);
        auto *actor = vtkAxesActor::SafeDownCast(first->GetViewProps()->GetItemAsObject(0));
        QVERIFY(actor);
        QCOMPARE(actor->GetTotalLength()[0], 321.0);
        QVERIFY(!actor->GetPickable());
        QVERIFY(!actor->GetUseBounds());
        GridSettings grid;
        grid.visible = {{true, true, true}};
        grid.halfExtentMm = 100;
        grid.divisions = 2;
        QVERIFY(scene.setGridSettings(grid));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 4);
        auto *props = first->GetViewProps();
        props->InitTraversal();
        int index = 0;
        while (auto *prop = props->GetNextProp()) {
            if (vtkAxesActor::SafeDownCast(prop))
                continue;
            auto *line = vtkActor::SafeDownCast(prop);
            QVERIFY(line);
            QVERIFY(!line->GetPickable());
            QVERIFY(!line->GetUseBounds());
            auto *mapper = vtkPolyDataMapper::SafeDownCast(line->GetMapper());
            mapper->Update();
            auto *data = mapper->GetInput();
            QCOMPARE(data->GetNumberOfLines(), vtkIdType(10));
            const int normal = index == 0 ? 1 : index == 1 ? 2 : 0;
            for (vtkIdType i = 0; i < data->GetNumberOfPoints(); ++i)
                QCOMPARE(data->GetPoint(i)[normal], 0.0);
            ++index;
        }
        grid.halfExtentMm = 200;
        QVERIFY(scene.setGridSettings(grid));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 4);
        QVERIFY(scene.showGrid(GridPlane::XY, false));
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 3);
        scene.setRenderer(second);
        QCOMPARE(first->GetViewProps()->GetNumberOfItems(), 0);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 3);
        scene.setRenderer(second);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 3);
        scene.showGrid(false);
        scene.showAxes(false);
        QCOMPARE(second->GetViewProps()->GetNumberOfItems(), 0);
        grid.divisions = 0;
        QVERIFY(!scene.setGridSettings(grid));
        QCOMPARE(scene.gridSettings().divisions, 2);
        axes.lengthMm = -1;
        QVERIFY(!scene.setAxesSettings(axes));
        QCOMPARE(scene.axesSettings().lengthMm, 321.0);
        QVERIFY(!scene.showGrid(GridPlane(99), true));
        scene.clear();
    }
    void backgroundAndWorld() {
        auto renderer = vtkSmartPointer<vtkRenderer>::New();
        SceneEnvironment scene;
        scene.setRenderer(renderer);
        BackgroundSettings background;
        background.gradient = true;
        background.bottomColor = {{0.1, 0.2, 0.3}};
        background.topColor = {{0.8, 0.7, 0.6}};
        QVERIFY(scene.setBackgroundSettings(background));
        QVERIFY(renderer->GetGradientBackground());
        QCOMPARE(renderer->GetBackground2()[0], 0.8);
        QVERIFY(scene.setBackground(0.2, 0.3, 0.4));
        QVERIFY(!renderer->GetGradientBackground());
        QVERIFY(!scene.setBackground(2, 0, 0));
        QCOMPARE(renderer->GetBackground()[0], 0.2);
        QTemporaryDir directory;
        QImage image(2, 2, QImage::Format_RGB32);
        image.fill(Qt::green);
        QVERIFY(image.save(directory.filePath("ground.png")));
        WorldSettings world;
        world.groundVisible = true;
        world.skyVisible = true;
        world.groundTexturePath = directory.filePath("ground.png");
        QVERIFY(scene.setWorldSettings(world));
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 2);
        auto *ground = vtkActor::SafeDownCast(renderer->GetViewProps()->GetItemAsObject(0));
        QVERIFY(ground->GetTexture());
        QVERIFY(ground->GetTexture()->GetRepeat());
        QVERIFY(QFile::remove(
            directory.filePath("ground.png"))); // Cached texture survives file removal.
        world.skyTexturePath = "missing-sky-image.png";
        QVERIFY(!scene.setWorldSettings(world));
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 2);
        world.skyTexturePath.clear();
        world.groundVisible = false;
        world.skyVisible = false;
        QVERIFY(scene.setWorldSettings(world));
        QCOMPARE(renderer->GetViewProps()->GetNumberOfItems(), 0);
    }
    void cameraViewsProjectionAndFitting() {
        auto renderer = vtkSmartPointer<vtkRenderer>::New();
        auto actor = model();
        renderer->AddActor(actor);
        SceneEnvironment scene;
        scene.setRenderer(renderer);
        scene.setModelActor(actor);
        const CameraPreset presets[7] = {
            CameraPreset::Isometric, CameraPreset::Front, CameraPreset::Back,  CameraPreset::Left,
            CameraPreset::Right,     CameraPreset::Top,   CameraPreset::Bottom};
        const double expected[7][3] = {{-1, -1, 1}, {0, -1, 0}, {0, 1, 0}, {-1, 0, 0},
                                       {1, 0, 0},   {0, 0, 1},  {0, 0, -1}};
        for (int p = 0; p < 7; ++p) {
            QVERIFY(scene.setCameraPreset(presets[p]));
            const auto pose = scene.cameraPose();
            for (int axis = 0; axis < 3; ++axis) {
                const double direction = pose.positionMm[axis] - pose.focalPointMm[axis];
                if (expected[p][axis] == 0)
                    QVERIFY(std::abs(direction) < 1e-6);
                else
                    QVERIFY(direction * expected[p][axis] > 0);
            }
        }
        QVERIFY(scene.setCameraPreset(CameraPreset::Front));
        auto before = scene.cameraPose();
        const double nearBeforeEnvironment = renderer->GetActiveCamera()->GetClippingRange()[0];
        scene.showAxes(true);
        scene.showGrid(true);
        QVERIFY(scene.resetCamera());
        auto after = scene.cameraPose();
        QVERIFY(renderer->GetActiveCamera()->GetClippingRange()[0] <=
                nearBeforeEnvironment * 1.000001);
        for (int axis = 0; axis < 3; ++axis)
            QVERIFY(std::abs(before.positionMm[axis] - after.positionMm[axis]) < 1e-6);
        auto settings = scene.cameraSettings();
        settings.viewAngleDegrees = 60;
        settings.projection = CameraProjection::Parallel;
        settings.locked = true;
        QVERIFY(scene.setCameraSettings(settings));
        QVERIFY(scene.resetCamera());
        QVERIFY(renderer->GetActiveCamera()->GetParallelProjection());
        QCOMPARE(renderer->GetActiveCamera()->GetViewAngle(), 60.0);
        QVERIFY(scene.cameraSettings().locked);
        settings.viewAngleDegrees = 0;
        QVERIFY(!scene.setCameraSettings(settings));
        QCOMPARE(renderer->GetActiveCamera()->GetViewAngle(), 60.0);
        auto invalid = scene.cameraPose();
        invalid.focalPointMm = invalid.positionMm;
        QVERIFY(!scene.setCameraPose(invalid));
        invalid = scene.cameraPose();
        for (int axis = 0; axis < 3; ++axis)
            invalid.viewUp[axis] = invalid.positionMm[axis] - invalid.focalPointMm[axis];
        QVERIFY(!scene.setCameraPose(invalid));
        QVERIFY(!scene.setCameraPreset(CameraPreset(99)));
        WorldSettings world;
        world.skyVisible = true;
        QVERIFY(scene.setWorldSettings(world));
        QVERIFY(scene.resetCamera());
        QCOMPARE(scene.cameraPose().focalPointMm[0], 700.0);
        QVERIFY(renderer->GetActiveCamera()->GetClippingRange()[1] > world.skyRadiusMm);
    }
    void deferredSettingsAndControl() {
        CameraPose pose;
        pose.positionMm = {{3, -8, 7}};
        pose.viewUp = {{0, 0, 1}};
        SceneEnvironment pending;
        QVERIFY(pending.setCameraPose(pose));
        auto renderer = vtkSmartPointer<vtkRenderer>::New();
        pending.setRenderer(renderer);
        QCOMPARE(pending.cameraPose().positionMm[1], -8.0);
        pending.setRenderer(nullptr);
        VisualizationControl control;
        QSignalSpy errors(&control, &VisualizationControl::errorOccurred);
        auto axes = control.axesSettings();
        axes.visible = true;
        axes.lengthMm = 500;
        QVERIFY(control.setAxesSettings(axes));
        QCOMPARE(control.axesSettings().lengthMm, 500.0);
        QVERIFY(control.showGrid(GridPlane::YZ, true));
        QVERIFY(control.gridSettings().visible[2]);
        control.showGrid(false);
        QVERIFY(!control.gridSettings().visible[2]);
        QVERIFY(control.setBackground(0.1, 0.2, 0.3));
        control.setCameraLocked(true);
        QVERIFY(control.cameraSettings().locked);
        auto camera = control.cameraSettings();
        camera.locked = false;
        camera.projection = CameraProjection::Parallel;
        QVERIFY(control.setCameraSettings(camera));
        QVERIFY(!control.cameraSettings().locked);
        axes.lengthMm = 0;
        QVERIFY(!control.setAxesSettings(axes));
        QVERIFY(errors.count() > 0);
    }
};
QTEST_GUILESS_MAIN(SceneEnvironmentTests)
#include "scene_environment_tests.moc"
