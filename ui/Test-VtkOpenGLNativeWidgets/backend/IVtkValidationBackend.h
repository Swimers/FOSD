#pragma once
#include <QString>
#include <QVariantMap>
class vtkRenderWindow;
namespace fosd {
namespace vtk_test {
// Synchronous GUI-thread validation port. Pointers are borrowed, never queued.
class IVtkValidationBackend {
public:
    virtual ~IVtkValidationBackend() = default;
    virtual bool attach(vtkRenderWindow *window, QString *error) = 0;
    virtual void detach() = 0;
    virtual vtkRenderWindow *boundWindow() const = 0;
    virtual bool showDemo(QString *error) = 0;
    virtual bool clearDemo(QString *error) = 0;
    virtual bool resetCamera(QString *error) = 0;
    virtual bool render(QString *error) = 0;
    virtual bool execute(const QString &, const QVariantMap &, QString *error) {
        if (error)
            *error = QStringLiteral("Unsupported integration action");
        return false;
    }
};
} // namespace vtk_test
} // namespace fosd
