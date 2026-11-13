#include "FOSD-Classic-QWidgets/application/ClassicWorkbench.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
int main(int argc, char *argv[]) {
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    ClassicWorkbench view;
    view.show();
    return app.exec();
}
