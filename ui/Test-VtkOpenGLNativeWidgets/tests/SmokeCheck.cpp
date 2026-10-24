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
#include <QtEndian>
#include <QFile>
#include <QLabel>
#include <cmath>
#include <cstring>
namespace fosd { namespace vtk_test {
SmokeCheck::SmokeCheck(VtkValidationWindow *view, VtkValidationPresenter *presenter,
    VtkValidationBackend *backend, const QString &screenshot)
    : m_view(view), m_presenter(presenter), m_backend(backend), m_screenshot(screenshot) {
    m_timer.setInterval(50); connect(&m_timer, &QTimer::timeout, this, &SmokeCheck::check);
}
void SmokeCheck::start() { m_elapsed.start(); m_timer.start(); }
void SmokeCheck::finish(bool success, const QString &message) {
    m_timer.stop(); QTextStream(stdout) << message << '\n';
    QCoreApplication::exit(success ? 0 : 1);
}
void SmokeCheck::check() {
    if (m_phase) { checkIntegration(); return; }
    if (!m_view->contextReady()) {
        if (m_elapsed.elapsed() > 8000) finish(false, "OpenGL context was not initialized");
        return;
    }
    auto *window = m_view->renderWindow();
    if (m_view->vtkWidget()->GetRenderWindow() != window || m_backend->boundWindow() != window ||
        window->GetRenderers()->GetNumberOfItems() != 1) {
        finish(false, "Frontend/VisualizationControl window binding mismatch"); return;
    }
    m_presenter->attach(); // Idempotent bind must not add duplicate renderers.
    m_presenter->showDemo();
    auto *renderer = window->GetRenderers()->GetFirstRenderer();
    if (window->GetRenderers()->GetNumberOfItems() != 1 || renderer->GetActors()->GetNumberOfItems() != 1) {
        finish(false, "Demo renderer/actor was not attached exactly once"); return;
    }
    m_presenter->showDemo(); m_presenter->resetCamera(); m_presenter->render();
    const QImage image = m_view->vtkWidget()->grabFramebuffer();
    if (image.isNull() || image.width() < 32 || image.height() < 32) {
        finish(false, "Native widget framebuffer is empty"); return;
    }
    const QColor center = image.pixelColor(image.width() / 2, image.height() / 2);
    if (center.red() <= center.blue() + 30 || renderer->GetActors()->GetNumberOfItems() != 1) {
        finish(false, "Framebuffer does not contain the rendered yellow sphere"); return;
    }
    if (!m_screenshot.isEmpty() && !image.save(m_screenshot)) {
        finish(false, "Failed to save framebuffer screenshot"); return;
    }
    m_presenter->clearDemo();
    if (renderer->GetActors()->GetNumberOfItems() != 0) { finish(false, "Clear demo failed"); return; }
    m_presenter->detach();
    if (m_backend->boundWindow() || window->GetRenderers()->GetNumberOfItems() != 0) {
        finish(false, "Detach failed"); return;
    }
    m_presenter->attach(); m_presenter->showDemo();
    const bool rebound = m_backend->boundWindow() == window &&
        window->GetRenderers()->GetNumberOfItems() == 1 &&
        window->GetRenderers()->GetFirstRenderer()->GetActors()->GetNumberOfItems() == 1;
    if (!rebound) { finish(false, "Reattach failed"); return; }
    m_presenter->clearDemo();
    QUdpSocket reservation;
    if (!reservation.bind(QHostAddress(QHostAddress::LocalHost), quint16(0))) { finish(false, "Cannot reserve UDP port"); return; }
    m_port = reservation.localPort(); reservation.close();
    QVariantMap parameters; parameters["transport"] = 0;
    parameters["address"] = "127.0.0.1"; parameters["port"] = m_port;
    QString error;
    if (!m_backend->execute("openButton", parameters, &error)) { finish(false, error); return; }
    m_phase = 1; m_elapsed.restart();
}
void SmokeCheck::checkIntegration() {
    if (m_elapsed.elapsed() > 10000) {
        finish(false, QString("Integration timed out at phase %1: %2").arg(m_phase)
            .arg(m_view->findChild<QLabel *>("statusLabel")->text())); return;
    }
    if (m_phase <= 2) {
        QByteArray packet = QByteArray::fromHex("5aa501010c0000000000");
        qToLittleEndian<quint32>(quint32(m_phase), reinterpret_cast<uchar *>(packet.data() + 6));
        for (int i = 0; i < 12; ++i) {
            packet.append(char(i + 1)); packet.append(char(1));
            const float value = float(1500 + i + (m_phase == 2 ? 0.01 : 0.0));
            quint32 bits; std::memcpy(&bits, &value, 4); char encoded[4];
            qToLittleEndian(bits, reinterpret_cast<uchar *>(encoded)); packet.append(encoded, 4);
        }
        m_sender.writeDatagram(packet, QHostAddress::LocalHost, m_port);
    }
    const auto frame = m_backend->latestFrame();
    if (m_phase == 1 && frame) {
        QFile mesh(m_directory.filePath("model.stl"));
        if (!mesh.open(QIODevice::WriteOnly)) { finish(false, mesh.errorString()); return; }
        mesh.write("solid test\nfacet normal 0 0 1\nouter loop\nvertex 100 0 0\nvertex 700 50 0\nvertex 1300 0 0\nendloop\nendfacet\nendsolid test\n"); mesh.close();
        QVariantMap p; p["path"] = mesh.fileName(); QString error;
        if (!m_backend->execute("modelButton", p, &error)) { finish(false, error); return; }
        p["path"] = m_directory.filePath("output.csv"); p["format"] = 0;
        if (!m_backend->execute("storageStartButton", p, &error)) { finish(false, error); return; }
        p["factor"] = 0.00075; p["indices"] = "0,1,2,3,4,5,6,7,8,9,10,11";
        if (!m_backend->execute("baselineButton", p, &error)) { finish(false, error); return; }
        m_phase = 2; return;
    }
    const auto result = m_backend->latestResult();
    const auto wavelength = result && result->sourceFrame ?
        std::dynamic_pointer_cast<const core::WavelengthFrame>(result->sourceFrame->data) : nullptr;
    if (m_phase == 2 && wavelength && wavelength->sequence == 2) {
        m_presenter->render();
        auto *actor = m_view->renderWindow()->GetRenderers()->GetFirstRenderer()->GetActors()->GetLastActor();
        auto *mapper = actor ? vtkPolyDataMapper::SafeDownCast(actor->GetMapper()) : nullptr;
        auto *mesh = mapper ? mapper->GetInput() : nullptr;
        if (!mesh || mesh->GetNumberOfPoints() != result->deformedModel.size()) { finish(false, "Displayed model point count mismatch"); return; }
        bool nonzero = false;
        for (auto point = result->deformedModel.constBegin(); point != result->deformedModel.constEnd(); ++point) {
            double xyz[3]; mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis) {
                // GUI refresh polls at 30 Hz; wait until this result is applied.
                if (std::abs(xyz[axis] - point.value()[axis]) > 1e-7) return;
                nonzero = nonzero || std::abs(result->displacementMap.value(point.key())[axis]) > 1e-9;
            }
        }
        if (!nonzero) { finish(false, "Calculation produced no displacement"); return; }
        m_presenter->execute("scaleButton", {{"scale", 2.0}});
        for (auto point = result->deformedModel.constBegin(); point != result->deformedModel.constEnd(); ++point) {
            double xyz[3]; mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis)
                if (std::abs(xyz[axis] - point.value()[axis] - result->displacementMap.value(point.key())[axis]) > 1e-7) {
                    finish(false, "Display deformation scale mismatch"); return;
                }
        }
        m_presenter->execute("resetDeformationButton", {});
        for (auto point = result->deformedModel.constBegin(); point != result->deformedModel.constEnd(); ++point) {
            double xyz[3]; mesh->GetPoint(vtkIdType(point.key()), xyz);
            for (int axis = 0; axis < 3; ++axis)
                if (std::abs(xyz[axis] - point.value()[axis] + result->displacementMap.value(point.key())[axis]) > 1e-7) {
                    finish(false, "Reset did not restore the original model"); return;
                }
        }
        QString error; m_backend->execute("storageStopButton", {}, &error);
        m_phase = 3;
    }
    if (m_phase == 3) {
        QFile output(m_directory.filePath("output.csv"));
        if (!output.open(QIODevice::ReadOnly)) return;
        const auto data = output.readAll();
        if (!data.contains("point_displacement") || !data.contains("wavelength") || !data.contains("calibrated_hardware")) return;
        finish(true, "Native window binding + UDP -> protocol -> HYY -> storage -> VTK deformation passed");
    }
}
} }
