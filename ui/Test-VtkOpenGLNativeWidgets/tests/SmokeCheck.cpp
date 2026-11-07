#include "SmokeCheck.h"
#include "Test-VtkOpenGLNativeWidgets/backend/VtkValidationBackend.h"
#include "Test-VtkOpenGLNativeWidgets/presenters/VtkValidationPresenter.h"
#include "Test-VtkOpenGLNativeWidgets/views/VtkValidationWindow.h"
#include <QCoreApplication>
#include <QImage>
#include <QTextStream>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkActorCollection.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include "visualization/VisualizationControl.h"
#include "visualization/interaction/ModelInteractorStyle.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <QtEndian>
#include <QFile>
#include <QLabel>
#include <cmath>
#include <cstring>
namespace fosd {
namespace vtk_test {
SmokeCheck::SmokeCheck(VtkValidationWindow *view, VtkValidationPresenter *presenter,
                       VtkValidationBackend *backend, const QString &screenshot)
    : m_view(view), m_presenter(presenter), m_backend(backend), m_screenshot(screenshot) {
    m_timer.setInterval(50);
    connect(&m_timer, &QTimer::timeout, this, &SmokeCheck::check);
}

void SmokeCheck::start() {
    m_elapsed.start();
    m_timer.start();
}

void SmokeCheck::finish(bool success, const QString &message) {
    m_timer.stop();
    QTextStream(stdout) << message << '\n';
    QCoreApplication::exit(success ? 0 : 1);
}

void SmokeCheck::check() {
    if (m_phase) {
        checkIntegration();
        return;
    }
    if (!m_view->contextReady()) {
        if (m_elapsed.elapsed() > 8000)
            finish(false, "OpenGL context was not initialized");
        return;
    }
    auto *window = m_view->renderWindow();
    if (m_view->vtkWidget()->GetRenderWindow() != window || m_backend->boundWindow() != window ||
        window->GetRenderers()->GetNumberOfItems() != 1) {
        finish(false, "Frontend/VisualizationControl window binding mismatch");
        return;
    }
    m_presenter->attach(); // Idempotent bind must not add duplicate renderers.
    m_presenter->showDemo();
    auto *renderer = window->GetRenderers()->GetFirstRenderer();
    if (window->GetRenderers()->GetNumberOfItems() != 1 ||
        renderer->GetActors()->GetNumberOfItems() != 1) {
        finish(false, "Demo renderer/actor was not attached exactly once");
        return;
    }
    m_presenter->showDemo();
    m_presenter->resetCamera();
    m_presenter->render();
    const QImage image = m_view->vtkWidget()->grabFramebuffer();
    if (image.isNull() || image.width() < 32 || image.height() < 32) {
        finish(false, "Native widget framebuffer is empty");
        return;
    }
    const QColor center = image.pixelColor(image.width() / 2, image.height() / 2);
    if (center.red() <= center.blue() + 30 || renderer->GetActors()->GetNumberOfItems() != 1) {
        finish(false, "Framebuffer does not contain the rendered yellow sphere");
        return;
    }
    if (!m_screenshot.isEmpty() && !image.save(m_screenshot)) {
        finish(false, "Failed to save framebuffer screenshot");
        return;
    }
    m_presenter->clearDemo();
    if (renderer->GetActors()->GetNumberOfItems() != 0) {
        finish(false, "Clear demo failed");
        return;
    }
    m_presenter->detach();
    if (m_backend->boundWindow() || window->GetRenderers()->GetNumberOfItems() != 0) {
        finish(false, "Detach failed");
        return;
    }
    m_presenter->attach();
    m_presenter->showDemo();
    const bool rebound =
        m_backend->boundWindow() == window && window->GetRenderers()->GetNumberOfItems() == 1 &&
        window->GetRenderers()->GetFirstRenderer()->GetActors()->GetNumberOfItems() == 1;
    if (!rebound) {
        finish(false, "Reattach failed");
        return;
    }
    m_presenter->clearDemo();
    QUdpSocket reservation;
    if (!reservation.bind(QHostAddress(QHostAddress::LocalHost), quint16(0))) {
        finish(false, "Cannot reserve UDP port");
        return;
    }
    m_port = reservation.localPort();
    reservation.close();
    QVariantMap parameters;
    parameters["transport"] = 0;
    parameters["address"] = "127.0.0.1";
    parameters["port"] = m_port;
    QString error;
    if (!m_backend->execute("openButton", parameters, &error)) {
        finish(false, error);
        return;
    }
    m_phase = 1;
    m_elapsed.restart();
}

void SmokeCheck::checkIntegration() {
    if (m_elapsed.elapsed() > 10000) {
        finish(false, QString("Integration timed out at phase %1: %2")
                          .arg(m_phase)
                          .arg(m_view->findChild<QLabel *>("statusLabel")->text()));
        return;
    }
    if (m_phase <= 2) {
        QByteArray packet = QByteArray::fromHex("5aa501010c0000000000");
        qToLittleEndian<quint32>(quint32(m_phase), reinterpret_cast<uchar *>(packet.data() + 6));
        for (int i = 0; i < 12; ++i) {
            packet.append(char(i + 1));
            packet.append(char(1));
            const float value = float(1500 + i + (m_phase == 2 ? 0.01 : 0.0));
            quint32 bits;
            std::memcpy(&bits, &value, 4);
            char encoded[4];
            qToLittleEndian(bits, reinterpret_cast<uchar *>(encoded));
            packet.append(encoded, 4);
        }
        m_sender.writeDatagram(packet, QHostAddress::LocalHost, m_port);
    }
    const auto frame = m_backend->latestFrame();
    if (m_phase == 1 && frame) {
        QFile mesh(m_directory.filePath("model.stl"));
        if (!mesh.open(QIODevice::WriteOnly)) {
            finish(false, mesh.errorString());
            return;
        }
        mesh.write("solid test\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 "
                   "0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid test\n");
        mesh.close();
        QVariantMap p;
        p["path"] = mesh.fileName();
        QString error;
        if (!m_backend->execute("modelButton", p, &error)) {
            finish(false, error);
            return;
        }
        p["path"] = m_directory.filePath("output.csv");
        p["format"] = 0;
        if (!m_backend->execute("storageStartButton", p, &error)) {
            finish(false, error);
            return;
        }
        p["factor"] = 0.00075;
        p["indices"] = "0,1,2,3,4,5,6,7,8,9,10,11";
        if (!m_backend->execute("baselineButton", p, &error)) {
            finish(false, error);
            return;
        }
        m_phase = 2;
        return;
    }
    const auto result = m_backend->latestResult();
    const auto wavelength =
        result && result->sourceFrame
            ? std::dynamic_pointer_cast<const core::WavelengthFrame>(result->sourceFrame->data)
            : nullptr;
    if (m_phase == 2 && wavelength && wavelength->sequence == 2) {
        m_presenter->render();
        auto *actor =
            m_view->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        auto *mapper = actor ? vtkPolyDataMapper::SafeDownCast(actor->GetMapper()) : nullptr;
        auto *mesh = mapper ? mapper->GetInput() : nullptr;
        if (!mesh || mesh->GetNumberOfPoints() != result->deformedModel.size()) {
            finish(false, "Displayed model point count mismatch");
            return;
        }
        bool nonzero = false;
        for (auto point = result->deformedModel.constBegin();
             point != result->deformedModel.constEnd(); ++point) {
            double xyz[3];
            mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis) {
                // GUI refresh polls at 30 Hz; wait until this result is applied.
                if (std::abs(xyz[axis] - point.value()[axis]) > 1e-7)
                    return;
                nonzero =
                    nonzero || std::abs(result->displacementMap.value(point.key())[axis]) > 1e-9;
            }
        }
        if (!nonzero) {
            finish(false, "Calculation produced no displacement");
            return;
        }
        // Exercise the real model-only picker through mouse callbacks. Markers
        // overlap vertices deliberately; they must never intercept a pick.
        auto *control = m_backend->visualization();
        visualization::KeyPointDefinition first, middle, last;
        first.pointId = 0;
        last.pointId = 2;
        middle.pointId = 1;
        first.label = "root";
        last.label = "tip";
        visualization::DisplayedPointDefinition displayedFirst, displayedLast;
        displayedFirst.pointId = 0;
        displayedLast.pointId = 2;
        if (!control->setKeyPoints({first, middle, last}) ||
            !control->setDisplayedPoints({displayedFirst, displayedLast})) {
            finish(false, "Key point configuration failed");
            return;
        }
        auto *renderer = m_view->renderWindow()->GetRenderers()->GetFirstRenderer();
        auto *interactor = m_view->renderWindow()->GetInteractor();
        auto *style =
            visualization::ModelInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
        if (!style) {
            finish(false, "Model mouse adapter was not installed");
            return;
        }
        double world[3];
        mesh->GetPoint(1, world);
        renderer->SetWorldPoint(world[0], world[1], world[2], 1);
        renderer->WorldToDisplay();
        const double *screen = renderer->GetDisplayPoint();
        interactor->SetEventInformation(int(std::round(screen[0])), int(std::round(screen[1])), 0,
                                        0);
        style->OnLeftButtonDown();
        style->OnLeftButtonUp();
        if (!control->selectedPoint().valid || control->selectedPoint().pointId != 1 ||
            !m_view->findChild<QLabel *>("selectionLabel")->text().contains("ID 1")) {
            finish(false, "Left click did not select the model vertex through Presenter");
            return;
        }
        const int clickX = interactor->GetEventPosition()[0],
                  clickY = interactor->GetEventPosition()[1];
        style->OnLeftButtonDown();
        interactor->SetEventPosition(clickX + 30, clickY + 15);
        style->OnMouseMove();
        interactor->SetEventPosition(clickX, clickY);
        style->OnLeftButtonUp();
        style->OnMouseWheelForward();
        if (!control->selectedPoint().valid || control->selectedPoint().pointId != 1) {
            finish(false, "Camera interaction discarded selection");
            return;
        }
        control->setCameraLocked(true);
        double cameraPosition[3];
        renderer->GetActiveCamera()->GetPosition(cameraPosition);
        style->OnMouseWheelForward();
        for (int axis = 0; axis < 3; ++axis)
            if (cameraPosition[axis] != renderer->GetActiveCamera()->GetPosition()[axis]) {
                finish(false, "Camera lock did not block wheel zoom");
                return;
            }
        auto picking = control->pickingSettings();
        if (!control->setKeyPoints({first, last}) || control->selectPoint(1).valid) {
            finish(false, "Non-key vertex remained selectable");
            return;
        }
        picking.onlyKeyPoints = true;
        control->setPickingSettings(picking);
        mesh->GetPoint(0, world);
        renderer->SetWorldPoint(world[0], world[1], world[2], 1);
        renderer->WorldToDisplay();
        screen = renderer->GetDisplayPoint();
        interactor->SetEventInformation(int(std::round(screen[0])), int(std::round(screen[1])), 0,
                                        0);
        style->OnLeftButtonDown();
        style->OnLeftButtonUp();
        if (!control->selectedPoint().valid || control->selectedPoint().pointId != 0) {
            finish(false, "Marker intercepted picking or locked camera prevented selection");
            return;
        }
        interactor->SetEventPosition(-10, -10);
        style->OnLeftButtonDown();
        style->OnLeftButtonUp();
        if (control->selectedPoint().valid) {
            finish(false, "Background click did not clear selection");
            return;
        }
        picking.onlyKeyPoints = false;
        control->setPickingSettings(picking);
        control->setCameraLocked(false);
        control->resetCamera();
        const auto cameraBeforeEnvironment = control->cameraPose();
        m_presenter->execute("sceneApplyButton", {{"axesVisible", true},
                                                  {"axesLength", 100.0},
                                                  {"gridXZ", true},
                                                  {"gridXY", true},
                                                  {"gridYZ", true},
                                                  {"gridExtent", 1500.0},
                                                  {"gridDivisions", 10},
                                                  {"gradientBackground", true},
                                                  {"backgroundBottom", "0.08,0.12,0.18"},
                                                  {"backgroundTop", "0.3,0.45,0.7"}});
        control->resetCamera();
        const auto cameraAfterEnvironment = control->cameraPose();
        if (!control->axesSettings().visible || !control->gridSettings().visible[2] ||
            !renderer->GetGradientBackground()) {
            finish(false, "Scene parameters were not forwarded through Presenter");
            return;
        }
        for (int axis = 0; axis < 3; ++axis)
            if (std::abs(cameraBeforeEnvironment.positionMm[axis] -
                         cameraAfterEnvironment.positionMm[axis]) > 1e-7) {
                finish(false, "Environment props changed model camera fitting");
                return;
            }
        m_presenter->execute("cameraViewButton", {{"cameraView", 5}}); // Top (+Z).
        m_presenter->execute("cameraApplyButton", {{"cameraProjection", 1},
                                                   {"cameraAngle", 45.0},
                                                   {"cameraScale", 700.0},
                                                   {"cameraLocked", false}});
        visualization::WorldSettings environment;
        environment.skyVisible = true;
        if (!control->setWorldSettings(environment) ||
            !renderer->GetActiveCamera()->GetParallelProjection()) {
            finish(false, "Scene world or orthographic camera failed");
            return;
        }
        mesh->GetPoint(0, world);
        renderer->SetWorldPoint(world[0], world[1], world[2], 1);
        renderer->WorldToDisplay();
        screen = renderer->GetDisplayPoint();
        interactor->SetEventInformation(int(std::round(screen[0])), int(std::round(screen[1])), 0,
                                        0);
        style->OnLeftButtonDown();
        style->OnLeftButtonUp();
        if (!control->selectedPoint().valid || control->selectedPoint().pointId != 0) {
            finish(false, "Scene props intercepted model picking");
            return;
        }
        environment.skyVisible = false;
        control->setWorldSettings(environment);
        control->showAxes(false);
        control->showGrid(false);
        if (!control->setKeyPoints({first, middle, last}) || !control->selectPoint(1).valid) {
            finish(false, "Restored key-point membership did not allow selection");
            return;
        }
        QVariantMap colorSettings{{"scalarEnabled", true},
                                  {"scalarMapping", 0},
                                  {"scalarRangeMode", 0},
                                  {"scalarMinimum", -1.0},
                                  {"scalarMaximum", 1.0}};
        for (int field = 0; field < 4; ++field) {
            colorSettings["scalarField"] = field;
            m_presenter->execute("scalarApplyButton", colorSettings);
            auto *values = mesh->GetPointData()->GetArray("FOSD.ColorScalars");
            if (!mapper->GetScalarVisibility() || !values ||
                values->GetNumberOfTuples() != mesh->GetNumberOfPoints()) {
                finish(false, "Displacement scalar mapping was not applied");
                return;
            }
            for (vtkIdType id = 0; id < values->GetNumberOfTuples(); ++id)
                if (values->GetTuple1(id) < 0 || values->GetTuple1(id) > 1) {
                    finish(false, "Normalized scalar outside [0,1]");
                    return;
                }
        }
        colorSettings["scalarMapping"] = 1;
        colorSettings["scalarRangeMode"] = 1;
        m_presenter->execute("scalarApplyButton", colorSettings);
        if (mapper->GetScalarRange()[0] != -1 || mapper->GetScalarRange()[1] != 1) {
            finish(false, "Manual physical color range was not applied");
            return;
        }
        m_presenter->execute("scaleButton", {{"scale", 2.0}});
        const auto selected = control->selectedPoint();
        if (std::abs(selected.displayPositionMm[0] - result->deformedModel.value(1)[0] -
                     result->displacementMap.value(1)[0]) > 1e-7 ||
            selected.displacementMm[0] != result->displacementMap.value(1)[0]) {
            finish(false, "Selected point data did not follow deformation scale");
            return;
        }
        for (auto point = result->deformedModel.constBegin();
             point != result->deformedModel.constEnd(); ++point) {
            double xyz[3];
            mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis)
                if (std::abs(xyz[axis] - point.value()[axis] -
                             result->displacementMap.value(point.key())[axis]) > 1e-7) {
                    finish(false, "Display deformation scale mismatch");
                    return;
                }
        }
        m_presenter->execute("resetDeformationButton", {});
        if (!control->selectedPoint().valid || control->selectedPoint().displacementMm[0] != 0) {
            finish(false, "Reset did not refresh selection data");
            return;
        }
        if (mapper->GetScalarVisibility() || mesh->GetPointData()->GetArray("FOSD.ColorScalars")) {
            finish(false, "Reset retained stale colors");
            return;
        }
        for (auto point = result->deformedModel.constBegin();
             point != result->deformedModel.constEnd(); ++point) {
            double xyz[3];
            mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis)
                if (std::abs(xyz[axis] - point.value()[axis] +
                             result->displacementMap.value(point.key())[axis]) > 1e-7) {
                    finish(false, "Reset did not restore the original model");
                    return;
                }
        }
        QString error;
        m_backend->execute("storageStopButton", {}, &error);
        m_phase = 3;
    }
    if (m_phase == 3) {
        QFile output(m_directory.filePath("output.csv"));
        if (!output.open(QIODevice::ReadOnly))
            return;
        const auto data = output.readAll();
        if (!data.contains("point_displacement") || !data.contains("wavelength") ||
            !data.contains("calibrated_hardware"))
            return;
        finish(true,
               "UDP -> HYY -> storage -> VTK colors/picking + axes/grid/background/camera passed");
    }
}
} // namespace vtk_test
} // namespace fosd
