#pragma once
#include <QString>
class vtkRenderWindow;
namespace fosd {
namespace vtk_test {
class IVtkValidationView {
public:
    virtual ~IVtkValidationView() = default;
    virtual QString chooseFile(bool) {
        return QString();
    }
    virtual vtkRenderWindow *renderWindow() const = 0;
    virtual bool contextReady() const = 0;
    virtual void showBinding(bool bound, vtkRenderWindow *backendWindow) = 0;
    virtual void showStatus(const QString &message) = 0;
};
} // namespace vtk_test
} // namespace fosd
