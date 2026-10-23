#include "Test-VtkOpenGLNativeWidgets/VtkValidationWindow.h"
#include "Test-VtkOpenGLNativeWidgets/VtkValidationPresenter.h"
#include "VtkValidationBackend.h"
#include "SmokeCheck.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);

int main(int argc, char **argv) {
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    auto format = QVTKOpenGLNativeWidget::defaultFormat(); format.setSamples(0);
    QSurfaceFormat::setDefaultFormat(format); // Before QApplication or any OpenGL widget.
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("Test-VtkOpenGLNativeWidgets");
    QCommandLineParser parser; parser.addHelpOption();
    parser.addOption({"smoke-test", "Validate real OpenGL rendering and exit automatically."});
    parser.addOption({"screenshot", "Save the smoke-test framebuffer as a PNG.", "path"});
    parser.process(app);
    using namespace fosd::vtk_test;
    // Destruction order is essential: Presenter detaches, Backend destroys
    // VisualizationControl, then View releases its native widget/window.
    VtkValidationWindow view;
    VtkValidationBackend backend;
    VtkValidationPresenter presenter(&view, &backend);
    QObject::connect(&view, &VtkValidationWindow::attachRequested, &presenter, &VtkValidationPresenter::attach);
    QObject::connect(&view, &VtkValidationWindow::detachRequested, &presenter, &VtkValidationPresenter::detach);
    QObject::connect(&view, &VtkValidationWindow::demoRequested, &presenter, &VtkValidationPresenter::showDemo);
    QObject::connect(&view, &VtkValidationWindow::clearRequested, &presenter, &VtkValidationPresenter::clearDemo);
    QObject::connect(&view, &VtkValidationWindow::resetCameraRequested, &presenter, &VtkValidationPresenter::resetCamera);
    QObject::connect(&view, &VtkValidationWindow::renderRequested, &presenter, &VtkValidationPresenter::render);
    QObject::connect(&view, &VtkValidationWindow::actionRequested, &presenter, &VtkValidationPresenter::execute);
    QObject::connect(&backend, &VtkValidationBackend::statusChanged, &presenter, &VtkValidationPresenter::showStatus);
    presenter.attach(); view.show();
    SmokeCheck smoke(&view, &presenter, &backend, parser.value("screenshot"));
    if (parser.isSet("smoke-test")) smoke.start();
    return app.exec();
}
