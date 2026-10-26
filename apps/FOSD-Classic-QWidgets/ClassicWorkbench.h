#pragma once
#include "FOSD-Classic-QWidgets/apps/WorkbenchView.h"
#include <memory>
#include <vtkSmartPointer.h>
class vtkGenericOpenGLRenderWindow;
namespace fosd {
namespace application {
class WorkbenchRuntime;
}

namespace ui {
namespace classic {
class WorkbenchPresenter;
class WorkbenchBackend;
class IBackend;
} // namespace classic
} // namespace ui
} // namespace fosd
class ClassicWorkbench : public fosd::ui::classic::WorkbenchView {
    Q_OBJECT
public:
    explicit ClassicWorkbench(QWidget *parent = nullptr);
    ~ClassicWorkbench() override;
    void setUiBackend(fosd::ui::classic::FeatureId feature, fosd::ui::classic::IBackend *backend);
    fosd::application::WorkbenchRuntime &runtime() const {
        return *m_runtime;
    }

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    std::unique_ptr<fosd::application::WorkbenchRuntime> m_runtime;
    std::unique_ptr<fosd::ui::classic::WorkbenchBackend> m_backends;
    std::unique_ptr<fosd::ui::classic::WorkbenchPresenter> m_presenter;
};
