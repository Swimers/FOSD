#pragma once
#include "visualization/VisualizationTypes.h"
#include <QObject>
#include <vtkSmartPointer.h>
class vtkPointPicker;
class vtkPointLocator;
class vtkRenderer;
class vtkPolyData;
class vtkActor;
namespace fosd {
namespace visualization {
class KeyPointManager;
class PointPicker final : public QObject {
    Q_OBJECT
public:
    explicit PointPicker(QObject *parent = nullptr);              // 构造：创建 vtkPointPicker，开启拾取列表
    ~PointPicker() override;                                      // 析构：成员均为智能指针/裸借用，无额外释放
    void setRenderer(vtkRenderer *renderer);                      // 绑定渲染器，pick() 需要用它做屏幕坐标拾取
    void setKeyPointManager(KeyPointManager *manager);            // 注入关键点管理器，用于只选关键点模式与选中高亮
    void setMesh(vtkPolyData *displayMesh,                        // 设置拾取目标：变形后网格（坐标来源）
                 core::ModelDataPtr originalModel,                // 原始模型坐标表（按顶点 ID 查原始位置）
                 vtkActor *actor = nullptr);                      // 模型 actor，限定只拾取该 actor 且加入拾取列表
    void setResult(core::CalculationResultPtr result);            // 保存最新计算结果，提供未缩放的位移值
    bool setSettings(const PickingSettings &settings, QString *error = nullptr); // 校验并应用拾取设置，失败回填 error
    PickingSettings settings() const {                            // 读取当前拾取设置
        return m_settings;
    }
    PickedPointInfo pick(int displayX, int displayY);             // 按设备像素坐标拾取最近顶点并选中
    PickedPointInfo selectPoint(vtkIdType id);                    // 按顶点 ID 选中（无效 ID 则清空选择）
    PickedPointInfo selection() const;                            // 组装当前选中点信息（原始/显示/位移）
    void refreshSelection();                                      // 数据变化后重定位选中标记并广播 selectionUpdated
    void cameraInteractionStarted();                              // 相机操作开始：按设置决定是否清空选择
    QVector<vtkIdType> nearbyPoints(const std::array<double, 3> &positionMm, double radiusMm) const; // 查询指定坐标半径内的顶点
    void clearSelection();                                        // 清空选中状态并通知关键点管理器

signals:
    void pointPicked(const fosd::visualization::PickedPointInfo &info);     // 用户点选结果（无效点也发送）
    void selectionUpdated(const fosd::visualization::PickedPointInfo &info); // 选中点因变形/缩放而更新位置

private:
    bool selectable(vtkIdType id) const;                          // 判断顶点是否可选中（含 onlyKeyPoints 过滤）
    vtkRenderer *m_renderer = nullptr;
    vtkPolyData *m_displayMesh = nullptr;
    vtkActor *m_modelActor = nullptr;
    KeyPointManager *m_keyPoints = nullptr;
    vtkSmartPointer<vtkPointPicker> m_picker;
    vtkSmartPointer<vtkPointLocator> m_locator;
    core::ModelDataPtr m_originalModel;
    core::CalculationResultPtr m_result;
    PickingSettings m_settings;
    vtkIdType m_selectedId = -1;
};
} // namespace visualization
} // namespace fosd
