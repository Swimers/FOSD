#pragma once
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
namespace fosd {
namespace vtk_test {
class VtkValidationWindow;
class VtkValidationPresenter;
class VtkValidationBackend;
class PlateSmokeCheck final : public QObject {
    Q_OBJECT
public:
    PlateSmokeCheck(VtkValidationWindow *view, VtkValidationPresenter *presenter,
                    VtkValidationBackend *backend, const QString &screenshot);
    void start();
private slots:
    void check();

private:
    void finish(bool success, const QString &message);
    VtkValidationWindow *m_view;
    VtkValidationPresenter *m_presenter;
    VtkValidationBackend *m_backend;
    QString m_screenshot;
    QTimer m_timer;
    QElapsedTimer m_elapsed;
    int m_phase = 0;
    qint64 m_stoppedAt = 0;
};
} // namespace vtk_test
} // namespace fosd
