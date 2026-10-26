# 关键点与选点交互对接

## 分层与所有权

| 组件 | 职责 |
| --- | --- |
| `KeyPointCreator` | 根据点 ID、世界坐标、文本和样式统一创建球体 Actor 与 Billboard Label；统一修改半径、颜色、字体、偏移和位置，不持有 Renderer |
| `KeyPointManager` | 持有批量配置标记与单个选中标记；添加/移除 Renderer Props，控制显隐，更新随动位置 |
| `PointPicker` | 仅在当前模型 Actor 的拾取白名单中选点；处理 ID、原始/显示坐标、实际位移、关键点限制、选中状态及邻域查询 |
| `ModelInteractorStyle` | 处理鼠标按下/抬起/移动/滚轮，区分单击与拖动，转交 PointPicker；相机动作委托 VTK Trackball，不持有模型或 Actor |
| `VisualizationControl` | 组织上述组件，绑定模型、结果、事件、显示倍率与生命周期，对外提供统一接口 |

全部 VTK 操作及接口调用位于 GUI 线程。Creator 返回带智能指针的 `KeyPointActors`；Manager 管理这些 Props，Renderer 由 Control 持有。Picker 借用当前显示网格和模型 Actor，原始模型和结果通过只读 shared_ptr 保持有效。重新加载/卸载前先清除拾取白名单和旧标记，避免悬空指针。

## 关键点配置

```cpp
using namespace fosd::visualization;
KeyPointDefinition root; root.pointId = 0; root.label = "root";
KeyPointDefinition tip; tip.pointId = 25; tip.label = "tip";
control.setKeyPoints({root, tip});

KeyPointStyle style = control.keyPointStyle();
style.radiusMm = 5.0;
style.color = {{1.0, 0.3, 0.1}};
style.labelColor = {{1.0, 1.0, 1.0}};
style.fontSize = 14;
style.labelOffsetMm = {{0.0, 0.0, 8.0}};
style.labelsVisible = true;
control.setKeyPointStyle(style);
control.showKeyPoints(true);
```

`setKeyPoints` 替换当前整批定义，传入空数组清空配置。点 ID 必须在已加载模型中存在，重复或无效 ID 拒绝整批输入并保留原配置，Control 发出 errorOccurred。label 为空时显示 ID。`keyPoints()` 返回当前定义。

样式半径单位 mm，文本大小为像素，labelOffsetMm 为世界坐标偏移。RGB 各分量在 0～1；半径必须有限且大于零；字体 1～256，球体分辨率 4～128。`setSelectionStyle` / `selectionStyle()` 单独控制选中标记样式，默认青色、半径 7 mm。普通关键点与选中点共用 Creator，不复制生成逻辑。

球体和 Label 均 PickableOff、UseBoundsOff，不拦截模型拾取，也不撑大 ResetCamera 的模型边界。选中的 ID 已属于配置关键点时，临时隐藏其普通球体/标签，使用选中样式；取消选择后恢复，避免重叠文字。

`showKeyPoints(false)` 只隐藏配置的普通标记，选中高亮独立保留。`clearSelection()` 只清除选择，不删除配置关键点。加载新模型、卸载模型时清空两类标记；恢复原始形状则保留配置和选择，将位置恢复到原始模型。

## 鼠标交互和选点配置

- 无修饰键左键单击在抬起时选点；空白位置或未命中允许选择的点时清除选择。
- 左键拖动旋转；Shift/Ctrl 等组合、VTK 默认中键平移、右键和滚轮缩放保留默认相机行为。
- 单击允许移动阈值默认 4 个 VTK 设备像素；拖动超过阈值后，即使鼠标又回到按下位置也不触发选点。
- 相机旋转、平移、缩放默认保留选中标记。`setCameraLocked(true)` 阻止鼠标相机移动，但仍允许左键单击选点；显式 ResetCamera 不受鼠标锁影响。

```cpp
PickingSettings picking = control.pickingSettings();
picking.enabled = true;
picking.onlyKeyPoints = false;      // true 时仅允许 setKeyPoints 配置过的点 ID
picking.tolerance = 0.009;          // VTK 窗口对角线的比例，范围 (0,1]
picking.clickTolerancePixels = 4;  // 0～100，Qt/QVTK 已转换的设备像素
picking.clearOnCameraInteraction = false;
control.setPickingSettings(picking);

control.selectPoint(25); // 程序按 ID 选择，与鼠标选择共用业务约束
const auto current = control.selectedPoint();
const auto ids = control.nearbyPoints({{700.0, 0.0, 0.0}}, 20.0);
control.clearSelection();
```

邻域查询依据当前显示网格的世界坐标与半径（mm），按 ID 排序；模型变形后按需重建 PointLocator。`onlyKeyPoints` 限制选点，不限制邻域查询。

## 输出与随动

`pointPicked(PickedPointInfo)` 表示用户/程序选择或清除；`selectionUpdated(PickedPointInfo)` 在变形、倍率或复位后更新当前选择的数据。`valid=false` 表示无选择。

- `pointId`：当前模型点 ID。
- `originalPositionMm`：原始模型坐标。
- `displacementMm`：真实解算位移，不乘显示倍率。
- `displayPositionMm`：VTK 当前显示坐标。

Manager 直接从显示网格取位置，因此配置标记、选中标记和 Label 随模型同步变化，不重复计算或累计位移。相机旋转不改变它们的世界坐标，Billboard Label 自动朝向相机。模型版本切换清除选择和旧位移缓存。

## 验证前端与测试

验证程序仍使用 `.ui`，输入逗号分隔的关键点 ID 后点击“应用关键点/交互”。同一行可配置半径、关键点/标签显隐、启用拾取、仅选关键点及相机锁；“清除选择”单独清除高亮。选点信息经 Control → 应用 Backend → Presenter → View 的独立 selectionLabel 展示，不被接收统计覆盖。

专项测试沿用可视化运行库选项：

```powershell
cmake --preset vtk-native-widgets -DFOSD_BUILD_VISUALIZATION_TESTS=ON
cmake --build --preset vtk-native-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
ctest --preset vtk-native-widgets-release
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe --smoke-test | Out-Host
```

`test_keypoint_interaction` 覆盖 Creator、批量管理/显隐/重复配置、Renderer 切换、样式校验、选择约束、邻域重建、拖动往返门控，以及 Control 的倍率/复位/卸载行为。Presenter 测试覆盖参数转发和选中信息回显；原生 OpenGL 冒烟检查通过真实模型投影坐标触发鼠标回调，检查模型拾取、标记不截获选择、相机缩放/拖动保留选择、锁定相机仍可选点、空白取消及倍率数据更新。

Debug 运行这些 VTK 测试需要 Debug DLL。仅编译不需要运行 DLL；Release DLL 不能用于 Debug 运行。背景/网格/坐标轴及模型材质迁移仍独立进行。
