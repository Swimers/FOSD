#pragma once
#include "IVtkValidationView.h"
#include <QWidget>
#include <vtkSmartPointer.h>
#include <QVariantMap>
#include <memory>
class QVTKOpenGLNativeWidget;
class vtkGenericOpenGLRenderWindow;
namespace Ui { class VtkValidationWindow; }
namespace fosd { namespace vtk_test {
class VtkValidationWindow final : public QWidget, public IVtkValidationView {
    Q_OBJECT
public:
    explicit VtkValidationWindow(QWidget *parent = nullptr);
    ~VtkValidationWindow() override;
    vtkRenderWindow *renderWindow() const override;
    bool contextReady() const override;
    void showBinding(bool bound, vtkRenderWindow *backendWindow) override;
    void showStatus(const QString &message) override;
    QString chooseFile(bool save) override;
    QVTKOpenGLNativeWidget *vtkWidget() const;
signals:
    void attachRequested();
    void detachRequested();
    void demoRequested();
    void clearRequested();
    void resetCameraRequested();
    void renderRequested();
    void actionRequested(const QString &action, const QVariantMap &parameters);
private slots:
    void requestAction();
private:
    std::unique_ptr<Ui::VtkValidationWindow> m_ui;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_window;
};
} }
