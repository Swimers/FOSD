#pragma once
#include "Test-VtkOpenGLNativeWidgets/views/IVtkValidationView.h"
#include "Test-VtkOpenGLNativeWidgets/backend/IVtkValidationBackend.h"
#include <QObject>
namespace fosd {
namespace vtk_test {
class VtkValidationPresenter final : public QObject {
    Q_OBJECT
public:
    VtkValidationPresenter(IVtkValidationView *view, IVtkValidationBackend *backend,
                           QObject *parent = nullptr);
    ~VtkValidationPresenter() override;
public slots:
    void attach();
    void detach();
    void showDemo();
    void clearDemo();
    void resetCamera();
    void render();
    void execute(const QString &action, const QVariantMap &parameters);
    void showStatus(const QString &message);

private:
    bool ready();
    void report(bool success, const QString &error, const QString &message);
    IVtkValidationView *m_view;
    IVtkValidationBackend *m_backend;
};
} // namespace vtk_test
} // namespace fosd
