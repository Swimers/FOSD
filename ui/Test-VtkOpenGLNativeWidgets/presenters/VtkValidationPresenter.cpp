#include "VtkValidationPresenter.h"
namespace fosd {
namespace vtk_test {
VtkValidationPresenter::VtkValidationPresenter(IVtkValidationView *view,
                                               IVtkValidationBackend *backend, QObject *parent)
    : QObject(parent), m_view(view), m_backend(backend) {
}

VtkValidationPresenter::~VtkValidationPresenter() {
    m_backend->detach();
}

void VtkValidationPresenter::showStatus(const QString &message) {
    m_view->showStatus(message);
}

void VtkValidationPresenter::showSelection(const QString &message) {
    m_view->showSelection(message);
}

void VtkValidationPresenter::execute(const QString &action, const QVariantMap &parameters) {
    QVariantMap values = parameters;
    if (action == "modelButton" || action == "storageStartButton") {
        if (action == "modelButton" && !ready())
            return;
        const QString path = m_view->chooseFile(action == "storageStartButton");
        if (path.isEmpty())
            return;
        values["path"] = path;
    }
    if ((action == "scaleButton" || action == "resetDeformationButton" ||
         action == "scalarApplyButton" || action == "keyPointsApplyButton" || action == "clearSelectionButton" ||
         action == "sceneApplyButton" || action == "cameraViewButton" ||
         action == "cameraApplyButton") &&
        !ready())
        return;
    QString error;
    report(m_backend->execute(action, values, &error), error, tr("请求已提交，等待后端状态"));
}

void VtkValidationPresenter::attach() {
    auto *window = m_view->renderWindow();
    QString error;
    bool success = window && m_backend->attach(window, &error);
    if (success && m_backend->boundWindow() != window) {
        m_backend->detach();
        success = false;
        error = tr("后端持有的 RenderWindow 与前端不同");
    }
    if (!success)
        m_backend->detach();
    m_view->showBinding(success, m_backend->boundWindow());
    report(success, error, tr("RenderWindow 已传递至 VisualizationControl"));
}

void VtkValidationPresenter::detach() {
    m_backend->detach();
    m_view->showBinding(false, nullptr);
    m_view->showStatus(tr("已解绑，控件仍由前端持有"));
}

bool VtkValidationPresenter::ready() {
    if (!m_backend->boundWindow() || m_backend->boundWindow() != m_view->renderWindow()) {
        m_view->showStatus(tr("请先绑定渲染窗口"));
        return false;
    }
    if (!m_view->contextReady()) {
        m_view->showStatus(tr("OpenGL 上下文尚未就绪，请等待窗口显示后重试"));
        return false;
    }
    return true;
}

void VtkValidationPresenter::report(bool success, const QString &error, const QString &message) {
    m_view->showStatus(success ? message
                               : (error.isEmpty() ? tr("操作失败：未提供有效渲染窗口") : error));
}

void VtkValidationPresenter::showDemo() {
    if (!ready())
        return;
    QString error;
    const bool ok = m_backend->showDemo(&error);
    report(ok, error, tr("示例球体已显示，可用鼠标旋转、平移和缩放"));
}

void VtkValidationPresenter::clearDemo() {
    if (!ready())
        return;
    QString error;
    const bool ok = m_backend->clearDemo(&error);
    report(ok, error, tr("示例球体已清除"));
}

void VtkValidationPresenter::resetCamera() {
    if (!ready())
        return;
    QString error;
    const bool ok = m_backend->resetCamera(&error);
    report(ok, error, tr("相机已重置"));
}

void VtkValidationPresenter::render() {
    if (!ready())
        return;
    QString error;
    const bool ok = m_backend->render(&error);
    report(ok, error, tr("渲染刷新完成"));
}
} // namespace vtk_test
} // namespace fosd
