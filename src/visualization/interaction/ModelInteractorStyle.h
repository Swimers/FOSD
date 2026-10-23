#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
namespace fosd { namespace visualization {
class PointPicker;
class ModelInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static ModelInteractorStyle *New();
    vtkTypeMacro(ModelInteractorStyle, vtkInteractorStyleTrackballCamera);
    void setPointPicker(PointPicker *picker);
    void setCameraLocked(bool locked);
    // Custom mouse overrides are deferred until business implementation.
protected:
    ModelInteractorStyle();
    ~ModelInteractorStyle() override;
private:
    ModelInteractorStyle(const ModelInteractorStyle &) = delete;
    ModelInteractorStyle &operator=(const ModelInteractorStyle &) = delete;
    PointPicker *m_picker = nullptr; // Non-owning sibling component.
    bool m_cameraLocked = false;
};
} }
