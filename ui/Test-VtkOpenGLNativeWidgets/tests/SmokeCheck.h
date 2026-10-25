#pragma once
#include <QObject>
#include <QElapsedTimer>
#include <QTimer>
#include <QTemporaryDir>
#include <QUdpSocket>
namespace fosd {
namespace vtk_test {
class VtkValidationWindow;
class VtkValidationPresenter;
class VtkValidationBackend;
// Optional visible OpenGL integration check; auto-exits after validation.
class SmokeCheck final : public QObject {
    Q_OBJECT
public:
    SmokeCheck(VtkValidationWindow *view, VtkValidationPresenter *presenter,
               VtkValidationBackend *backend, const QString &screenshot);
    void start();
private slots:
    void check();

private:
    void finish(bool success, const QString &message);
    void checkIntegration();
    int m_phase = 0;
    quint16 m_port = 0;
    QTemporaryDir m_directory;
    QUdpSocket m_sender;
    VtkValidationWindow *m_view;
    VtkValidationPresenter *m_presenter;
    VtkValidationBackend *m_backend;
    QString m_screenshot;
    QTimer m_timer;
    QElapsedTimer m_elapsed;
};
} // namespace vtk_test
} // namespace fosd
