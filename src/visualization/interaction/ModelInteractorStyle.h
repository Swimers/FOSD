#pragma once
#include <vtkInteractorStyleTrackballCamera.h>
namespace fosd {
namespace visualization {
class PointPicker;

class ModelInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
    static ModelInteractorStyle *New(); // 静态工厂方法，由 .cpp 中的 vtkStandardNewMacro 实现

    // VTK 运行时类型信息宏：声明 Superclass 类型别名、GetClassName()、
    // IsTypeOf()/IsA() 与 SafeDownCast()。每个 vtkObject 子类都必须声明，
    // 用于通过基类指针识别具体类型并安全向下转型。
    vtkTypeMacro(ModelInteractorStyle, vtkInteractorStyleTrackballCamera);

    void setPointPicker(PointPicker *picker);   // 传递 Picker
    void setCameraLocked(bool locked);          // 设置交互锁定
    void setClickTolerancePixels(int pixels);   // 设定点击/拖拽阈值

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
    ModelInteractorStyle(); // 默认构造
    ~ModelInteractorStyle() override;   //默认析构

private:
    ModelInteractorStyle(const ModelInteractorStyle &) = delete;    // 禁用拷贝
    ModelInteractorStyle &operator=(const ModelInteractorStyle &) = delete; // 禁用赋值

    void otherInteraction();    // 限制点击，调整picker进行交互
    void trackMovement();   // 点击/拖拽判断，超过阈值改为拖拽交互

    PointPicker *m_picker = nullptr;
    bool m_cameraLocked = false;    // 相机锁，禁止相机鼠标交互
    bool m_leftDown = false;        // 左键当前是否按住
    bool m_baseLeftDown = false;    // 本次按下是否调用过父类
    bool m_clickAllowed = false;    // 点击是否合法
    int m_pressX = 0, m_pressY = 0; // 按下坐标基准
    int m_clickTolerancePixels = 4; // 点击/拖拽阈值
};
} // namespace visualization
} // namespace fosd
