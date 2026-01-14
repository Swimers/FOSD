#include "mainwindow.h"
#include <QApplication>
#include <vtkOutputWindow.h>
#include<vtkAutoInit.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    VTK_MODULE_INIT(vtkRenderingOpenGL2);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);
    vtkOutputWindow::SetGlobalWarningDisplay(0); //加入

    return a.exec();
}
