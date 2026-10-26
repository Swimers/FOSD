#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
namespace fosd {
namespace visualization {
class PointPicker;
// Mouse-event adapter only: no mesh, actors, Qt widgets or selection state.
class ModelInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static ModelInteractorStyle *New();
    vtkTypeMacro(ModelInteractorStyle, vtkInteractorStyleTrackballCamera);
    void setPointPicker(PointPicker *picker);
    void setCameraLocked(bool locked);
    void setClickTolerancePixels(int pixels);
    void OnMouseMove() override;
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMiddleButtonDown() override;
    void OnMiddleButtonUp() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    void OnMouseWheelForward() override;
    void OnMouseWheelBackward() override;
    void Rotate() override;
    void Spin() override;
    void Pan() override;
    void Dolly() override;

protected:
    ModelInteractorStyle();
    ~ModelInteractorStyle() override;

private:
    ModelInteractorStyle(const ModelInteractorStyle &) = delete;
    ModelInteractorStyle &operator=(const ModelInteractorStyle &) = delete;
    void otherInteraction();
    void trackMovement();
    PointPicker *m_picker = nullptr;
    bool m_cameraLocked = false;
    bool m_leftDown = false;
    bool m_baseLeftDown = false;
    bool m_clickAllowed = false;
    int m_pressX = 0, m_pressY = 0;
    int m_clickTolerancePixels = 4;
};
} // namespace visualization
} // namespace fosd
