#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
namespace fosd {
namespace visualization {
class PointPicker;

class ModelInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static ModelInteractorStyle *New();

    vtkTypeMacro(ModelInteractorStyle, vtkInteractorStyleTrackballCamera);

    void setPointPicker(PointPicker *picker);
    void setCameraLocked(bool locked);

    // Interaction extension points. Keep VTK's trackball behavior until
    // model-specific picking and camera rules are implemented.
    void OnMouseMove() override;          // 鼠标移动
    void OnLeftButtonDown() override;     // 鼠标左键按下
    void OnLeftButtonUp() override;       // 鼠标左键释放
    void OnMiddleButtonDown() override;   // 鼠标中键按下
    void OnMiddleButtonUp() override;     // 鼠标中键释放
    void OnRightButtonDown() override;    // 鼠标右键按下
    void OnRightButtonUp() override;      // 鼠标右键释放
    void OnMouseWheelForward() override;  // 鼠标滚轮前进
    void OnMouseWheelBackward() override; // 鼠标滚轮后退

    void OnKeyDown() override; // 键盘按键按下
    void OnKeyUp() override;   // 键盘按键释放
    void OnChar() override;    // 关键按键"r/w/s/p/f/3/e/q/u/j/t/a/c"

    void Rotate() override; //鼠标旋转事件
    void Spin() override;   //鼠标惯性旋转
    void Pan() override;    //相机平移
    void Dolly() override;  //相机拉伸

protected:
    ModelInteractorStyle();
    ~ModelInteractorStyle() override;

private:
    ModelInteractorStyle(const ModelInteractorStyle &) = delete;
    ModelInteractorStyle &operator=(const ModelInteractorStyle &) = delete;

    PointPicker *m_picker = nullptr; // Non-owning sibling component.
    bool m_cameraLocked = false;
};

} // namespace visualization
} // namespace fosd
