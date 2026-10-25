#include "VtkValidationWindow.h"
#include "ui_VtkValidationWindow.h"
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QFileDialog>
namespace fosd { 
namespace vtk_test {

VtkValidationWindow::VtkValidationWindow(QWidget *parent)
    : QWidget(parent), m_ui(new Ui::VtkValidationWindow),
      m_window(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()) {
    m_ui->setupUi(this);
    m_window->SetMultiSamples(0);
    m_ui->vtkOpenGLNativeWidget->SetRenderWindow(m_window);
    connect(m_ui->attachButton, &QPushButton::clicked, this, &VtkValidationWindow::attachRequested);
    connect(m_ui->detachButton, &QPushButton::clicked, this, &VtkValidationWindow::detachRequested);
    connect(m_ui->demoButton, &QPushButton::clicked, this, &VtkValidationWindow::demoRequested);
    connect(m_ui->clearButton, &QPushButton::clicked, this, &VtkValidationWindow::clearRequested);
    connect(m_ui->resetCameraButton, &QPushButton::clicked, this, &VtkValidationWindow::resetCameraRequested);
    connect(m_ui->renderButton, &QPushButton::clicked, this, &VtkValidationWindow::renderRequested);
    for (auto *button : {m_ui->openButton, m_ui->closeButton, m_ui->modelButton,
         m_ui->baselineButton, m_ui->storageStartButton, m_ui->storageStopButton,
         m_ui->scaleButton, m_ui->resetDeformationButton})
        connect(button, &QPushButton::clicked, this, &VtkValidationWindow::requestAction);
    showBinding(false, nullptr);
}
VtkValidationWindow::~VtkValidationWindow() {
    // Release native widget before the frontend-owned VTK window.
    delete m_ui->vtkOpenGLNativeWidget;
}
vtkRenderWindow *VtkValidationWindow::renderWindow() const { return m_window; }
QVTKOpenGLNativeWidget *VtkValidationWindow::vtkWidget() const { return m_ui->vtkOpenGLNativeWidget; }
bool VtkValidationWindow::contextReady() const { return vtkWidget()->isValid(); }
void VtkValidationWindow::showBinding(bool bound, vtkRenderWindow *backendWindow) {
    m_ui->bindingLabel->setText(tr("前端：0x%1    VisualizationControl：0x%2    %3")
        .arg(quintptr(renderWindow()), 0, 16).arg(quintptr(backendWindow), 0, 16)
        .arg(bound ? tr("已绑定") : tr("未绑定")));
    for (auto *button : {m_ui->detachButton, m_ui->demoButton, m_ui->clearButton,
         m_ui->resetCameraButton, m_ui->renderButton, m_ui->modelButton,
         m_ui->scaleButton, m_ui->resetDeformationButton}) button->setEnabled(bound);
}
void VtkValidationWindow::showStatus(const QString &message) { m_ui->statusLabel->setText(message); }
QString VtkValidationWindow::chooseFile(bool save) {
    if (!save) 
        return QFileDialog::getOpenFileName(this, tr("导入 STL 模型（坐标单位 mm）"), {}, "STL (*.stl)");
    return QFileDialog::getSaveFileName(this, tr("存储所有数据"), {},
        m_ui->storageFormat->currentIndex() == 0 ? "CSV (*.csv)" : "SQLite (*.sqlite)");
}
void VtkValidationWindow::requestAction() {
    QVariantMap values;
    values["transport"] = m_ui->transport->currentIndex();
    values["address"] = m_ui->address->text(); values["port"] = m_ui->port->value();
    values["serial"] = m_ui->serial->text(); values["baud"] = m_ui->baud->value();
    values["factor"] = m_ui->factor->value(); values["indices"] = m_ui->indices->text();
    values["A"] = m_ui->calibrationA->value(); values["B"] = m_ui->calibrationB->value();
    values["format"] = m_ui->storageFormat->currentIndex(); values["scale"] = m_ui->scale->value();
    emit actionRequested(sender()->objectName(), values);
}
} }
