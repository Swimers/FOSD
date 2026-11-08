#include "FOSD-Classic-QWidgets/application/ClassicWorkbench.h"
#include "FOSD-Classic-QWidgets/model/WorkbenchModel.h"
#include "FOSD-Classic-QWidgets/application/WorkbenchPresenter.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QTimer>
#include "ui_WorkbenchRenderSurface.h"

ClassicWorkbench::ClassicWorkbench(QWidget *parent)
    : fosd::ui::classic::WorkbenchView(parent),
      m_renderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
      m_model(new fosd::application::WorkbenchModel),
      m_presenter(new fosd::ui::classic::WorkbenchPresenter(this, m_model.get(), this)) {
    auto *surface = new QWidget(renderHost());
    Ui::WorkbenchRenderSurface renderUi;
    renderUi.setupUi(surface);
    renderUi.workbenchVtkSurface->SetRenderWindow(m_renderWindow);
    renderHost()->setSurface(surface);
    m_model->attach(m_renderWindow);
    QTimer::singleShot(0, m_presenter.get(), &fosd::ui::classic::WorkbenchPresenter::initialize);
}

ClassicWorkbench::~ClassicWorkbench() {
    m_model->visualization().detach();
}
