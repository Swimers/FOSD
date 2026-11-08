#include "FOSD-Classic-QWidgets/application/ClassicWorkbench.h"
#include "FOSD-Classic-QWidgets/application/runtime/WorkbenchRuntime.h"
#include "FOSD-Classic-QWidgets/application/WorkbenchPresenter.h"
#include "FOSD-Classic-QWidgets/controls/renderhostwidget.h"
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QTimer>
#include "ui_WorkbenchRenderSurface.h"

ClassicWorkbench::ClassicWorkbench(QWidget *parent)
    : fosd::ui::classic::WorkbenchView(parent),
      m_renderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
      m_runtime(new fosd::application::WorkbenchRuntime),
      m_backends(new fosd::ui::classic::WorkbenchBackend),
      m_presenter(new fosd::ui::classic::WorkbenchPresenter(this)) {
    auto *surface = new QWidget(renderHost());
    Ui::WorkbenchRenderSurface renderUi;
    renderUi.setupUi(surface);
    renderUi.workbenchVtkSurface->SetRenderWindow(m_renderWindow);
    renderHost()->setSurface(surface);
    m_runtime->attach(m_renderWindow);
    m_backends->setServices(m_runtime.get());
    m_presenter->setBackends(m_backends.get());
    QTimer::singleShot(0, m_runtime.get(), &fosd::application::WorkbenchRuntime::initialize);
}

ClassicWorkbench::~ClassicWorkbench() {
    m_runtime->visualization().detach();
}

void ClassicWorkbench::setUiBackend(fosd::ui::classic::FeatureId feature,
                                    fosd::ui::classic::IBackend *backend) {
    m_backends->setBackend(feature, backend);
}
