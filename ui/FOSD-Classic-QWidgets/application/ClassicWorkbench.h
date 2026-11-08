#pragma once
#include "FOSD-Classic-QWidgets/application/WorkbenchView.h"
#include <memory>
#include <vtkSmartPointer.h>
class vtkGenericOpenGLRenderWindow;
namespace fosd {
namespace application {
class WorkbenchModel;
}

namespace ui {
namespace classic {
class WorkbenchPresenter;
} // namespace classic
} // namespace ui
} // namespace fosd
class ClassicWorkbench : public fosd::ui::classic::WorkbenchView {
    Q_OBJECT
public:
    explicit ClassicWorkbench(QWidget *parent = nullptr);
    ~ClassicWorkbench() override;
    fosd::application::WorkbenchModel &model() const {
        return *m_model;
    }

private:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    std::unique_ptr<fosd::application::WorkbenchModel> m_model;
    std::unique_ptr<fosd::ui::classic::WorkbenchPresenter> m_presenter;
};
