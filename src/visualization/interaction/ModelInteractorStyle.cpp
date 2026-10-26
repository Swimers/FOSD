#include "ModelInteractorStyle.h"
#include "PointPicker.h"
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
namespace fosd {
namespace visualization {
vtkStandardNewMacro(ModelInteractorStyle);
ModelInteractorStyle::ModelInteractorStyle() = default;
ModelInteractorStyle::~ModelInteractorStyle() = default;
void ModelInteractorStyle::setPointPicker(PointPicker *picker) {
    m_picker = picker;
}

void ModelInteractorStyle::setClickTolerancePixels(int pixels) {
    if (pixels >= 0)
        m_clickTolerancePixels = pixels;
}

void ModelInteractorStyle::setCameraLocked(bool locked) {
    if (m_cameraLocked == locked)
        return;
    m_cameraLocked = locked;
    if (locked && this->Interactor && this->Interactor->GetRenderWindow() &&
        this->State != VTKIS_NONE) {
        this->StopState();
        this->ReleaseFocus();
    }
    m_leftDown = false;
    m_baseLeftDown = false;
    m_clickAllowed = false;
}

void ModelInteractorStyle::otherInteraction() {
    m_clickAllowed = false;
    if (m_picker)
        m_picker->cameraInteractionStarted();
}

void ModelInteractorStyle::trackMovement() {
    if (!m_leftDown || !m_clickAllowed || !this->Interactor)
        return;
    const int *xy = this->Interactor->GetEventPosition();
    const double dx = double(xy[0]) - m_pressX, dy = double(xy[1]) - m_pressY;
    if (dx * dx + dy * dy > double(m_clickTolerancePixels) * m_clickTolerancePixels)
        otherInteraction();
}

void ModelInteractorStyle::OnMouseMove() {
    trackMovement();
    if (!m_cameraLocked)
        this->Superclass::OnMouseMove();
}

void ModelInteractorStyle::OnLeftButtonDown() {
    if (!this->Interactor)
        return;
    const int *xy = this->Interactor->GetEventPosition();
    m_pressX = xy[0];
    m_pressY = xy[1];
    m_leftDown = true;
    m_clickAllowed = !this->Interactor->GetShiftKey() && !this->Interactor->GetControlKey() &&
                     !this->Interactor->GetAltKey();
    m_baseLeftDown = !m_cameraLocked;
    if (!m_clickAllowed && !m_cameraLocked && m_picker)
        m_picker->cameraInteractionStarted();
    if (m_baseLeftDown)
        this->Superclass::OnLeftButtonDown();
}

void ModelInteractorStyle::OnLeftButtonUp() {
    if (!this->Interactor)
        return;
    trackMovement();
    const bool click = m_leftDown && m_clickAllowed && !this->Interactor->GetShiftKey() &&
                       !this->Interactor->GetControlKey() && !this->Interactor->GetAltKey();
    m_leftDown = false;
    m_clickAllowed = false;
    if (m_baseLeftDown)
        this->Superclass::OnLeftButtonUp();
    m_baseLeftDown = false;
    if (click && m_picker) {
        const int *xy = this->Interactor->GetEventPosition();
        m_picker->pick(xy[0], xy[1]);
    }
}

void ModelInteractorStyle::OnMiddleButtonDown() {
    otherInteraction();
    if (!m_cameraLocked)
        this->Superclass::OnMiddleButtonDown();
}

void ModelInteractorStyle::OnMiddleButtonUp() {
    this->Superclass::OnMiddleButtonUp();
}

void ModelInteractorStyle::OnRightButtonDown() {
    otherInteraction();
    if (!m_cameraLocked)
        this->Superclass::OnRightButtonDown();
}

void ModelInteractorStyle::OnRightButtonUp() {
    this->Superclass::OnRightButtonUp();
}

void ModelInteractorStyle::OnMouseWheelForward() {
    if (!m_cameraLocked) {
        otherInteraction();
        this->Superclass::OnMouseWheelForward();
    }
}

void ModelInteractorStyle::OnMouseWheelBackward() {
    if (!m_cameraLocked) {
        otherInteraction();
        this->Superclass::OnMouseWheelBackward();
    }
}

void ModelInteractorStyle::Rotate() {
    if (!m_cameraLocked)
        this->Superclass::Rotate();
}

void ModelInteractorStyle::Spin() {
    if (!m_cameraLocked)
        this->Superclass::Spin();
}

void ModelInteractorStyle::Pan() {
    if (!m_cameraLocked)
        this->Superclass::Pan();
}

void ModelInteractorStyle::Dolly() {
    if (!m_cameraLocked)
        this->Superclass::Dolly();
}
} // namespace visualization
} // namespace fosd
