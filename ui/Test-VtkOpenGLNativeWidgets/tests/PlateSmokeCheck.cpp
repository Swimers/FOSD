#include "PlateSmokeCheck.h"
#include "Test-VtkOpenGLNativeWidgets/backend/VtkValidationBackend.h"
#include "Test-VtkOpenGLNativeWidgets/presenters/VtkValidationPresenter.h"
#include "Test-VtkOpenGLNativeWidgets/views/VtkValidationWindow.h"
#include "visualization/VisualizationControl.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QLabel>
#include <QImage>
#include <QVTKOpenGLNativeWidget.h>
#include <QUdpSocket>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkActorCollection.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <cmath>
namespace fosd {
namespace vtk_test {
PlateSmokeCheck::PlateSmokeCheck(VtkValidationWindow *view, VtkValidationPresenter *presenter,
                                 VtkValidationBackend *backend, const QString &screenshot)
    : m_view(view), m_presenter(presenter), m_backend(backend), m_screenshot(screenshot) {
    m_timer.setInterval(50);
    connect(&m_timer, &QTimer::timeout, this, &PlateSmokeCheck::check);
}

void PlateSmokeCheck::start() {
    m_elapsed.start();
    m_timer.start();
}

void PlateSmokeCheck::finish(bool success, const QString &message) {
    m_timer.stop();
    QTextStream(stdout) << message << '\n';
    QCoreApplication::exit(success ? 0 : 1);
}

void PlateSmokeCheck::check() {
    if (m_elapsed.elapsed() > 10000) {
        finish(false, "Plate smoke timeout: " + m_view->findChild<QLabel *>("statusLabel")->text());
        return;
    }
    if (!m_view->contextReady())
        return;
    if (m_phase == 0) {
        QUdpSocket reservation;
        if (!reservation.bind(QHostAddress(QHostAddress::LocalHost), quint16(0))) {
            finish(false, "Cannot reserve UDP port");
            return;
        }
        const quint16 port = reservation.localPort();
        reservation.close();
        m_presenter->execute("plateStartButton", {{"port", port}, {"plateRate", 50.0}});
        m_phase = 1;
        return;
    }
    auto result = m_backend->latestResult();
    if (!result || result->algorithmId != "demo.plate.modal-ife" || result->revision < 15)
        return;
    if (m_phase == 1) {
        m_presenter->execute("plateStopButton", {});
        m_phase = 2;
        m_stoppedAt = m_elapsed.elapsed();
        return;
    }
    if (m_elapsed.elapsed() - m_stoppedAt < 250)
        return; // Drain ingress and GUI latest-result polling.
    result = m_backend->latestResult();
    auto *window = m_view->renderWindow();
    if (m_backend->boundWindow() != window || m_view->vtkWidget()->GetRenderWindow() != window) {
        finish(false, "Plate RenderWindow pointer mismatch");
        return;
    }
    auto *scene = window->GetRenderers()->GetFirstRenderer();
    auto *actor = scene ? scene->GetActors()->GetLastActor() : nullptr;
    auto *mapper = actor ? vtkPolyDataMapper::SafeDownCast(actor->GetMapper()) : nullptr;
    auto *mesh = mapper ? mapper->GetInput() : nullptr;
    if (!mesh || mesh->GetNumberOfPoints() != 1394 || mesh->GetNumberOfCells() != 2784 ||
        result->displacementMap.size() != mesh->GetNumberOfPoints() ||
        !mapper->GetScalarVisibility() || !mesh->GetPointData()->GetArray("FOSD.ColorScalars")) {
        finish(false, "Plate geometry/result/color field mismatch");
        return;
    }
    double maximum = 0;
    for (auto point = result->deformedModel.constBegin(); point != result->deformedModel.constEnd();
         ++point) {
        double xyz[3];
        mesh->GetPoint(vtkIdType(point.key()), xyz);
        for (int axis = 0; axis < 3; ++axis)
            if (std::abs(xyz[axis] - point.value()[axis]) > 1e-7) {
                finish(false, "Plate display differs from calculated displacement");
                return;
            }
        const auto delta = result->displacementMap.value(point.key());
        maximum = qMax(maximum, std::abs(delta[2]));
        if (m_backend->visualization()->originalModel()->value(point.key())[0] == 0)
            for (double displacement : delta)
                if (std::abs(displacement) > 1e-10) {
                    finish(false, "Clamped plate edge moved");
                    return;
                }
    }
    if (maximum < 0.5) {
        finish(false, "Plate simulation did not move");
        return;
    }
    const QImage image = m_view->vtkWidget()->grabFramebuffer();
    if (image.isNull() || (!m_screenshot.isEmpty() && !image.save(m_screenshot))) {
        finish(false, "Cannot capture plate framebuffer");
        return;
    }
    // Restart and return to HYY also exercise worker replacement and cleanup.
    m_presenter->execute("plateExitButton", {});
    if (m_backend->visualization()->originalModel()) {
        finish(false, "Plate mode cleanup failed");
        return;
    }
    finish(true, "Plate UDP -> calibration -> modal IFE -> 1394 VTK points + colors passed");
}
} // namespace vtk_test
} // namespace fosd
