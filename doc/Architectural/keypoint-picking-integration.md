# 关键点、选中点与持续显示点对接

## 三类点的约束

| 集合 | 默认状态 | 数据及显示 |
| --- | --- | --- |
| KeyPoints | 每次载入模型时包含全部模型点 | 仅保存模型 ID 与名称；不创建 Actor |
| SelectedPoints | 空；左键单击保持一个当前选中点 | 必须属于 KeyPoints；独立的球和文本，支持清除及变形随动 |
| DisplayedPoints | 空 | 可添加任意 KeyPoint，或具有独立 ID、固定 XYZ 坐标的模型外点；独立持续显示的球和文本 |

移除关键点会同步清除对应模型点的选中标记和持续显示标记；外部固定点保留。删除持续显示点不修改关键点集合或当前选择。切换、卸载模型清空所有组，成功载入后重新初始化全部 KeyPoints。

外部点 ID 必须是非负整数，且不能与当前模型的任何点 ID 相同，避免绕过关键点约束。重复 ID、无效模型点、非关键模型点、非有限外部坐标和未知来源类型均拒绝整批输入，保留原配置。

## 分层与所有权

| 组件 | 职责 |
| --- | --- |
| `KeyPointCreator` | 根据点 ID、世界坐标、文本和样式统一创建球体 Actor 与 Billboard Label；统一修改半径、颜色、字体、偏移和位置，不持有 Renderer |
| `KeyPointManager` | 分开保存关键点集合、持续显示点定义/Actor 与单个选中 Actor；管理 Renderer Props、显隐及随动位置 |
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

DisplayedPointDefinition tipDisplay;
tipDisplay.pointId = 25;
DisplayedPointDefinition reference;
reference.pointId = 1000000; // 必须不与模型 ID 重复。
reference.source = DisplayedPointSource::External;
reference.positionMm = {{100.0, 200.0, 300.0}};
reference.label = "reference";
control.setDisplayedPoints({tipDisplay, reference});

KeyPointStyle style = control.keyPointStyle();
style.radiusMm = 5.0;
style.color = {{1.0, 0.3, 0.1}};
style.labelColor = {{1.0, 1.0, 1.0}};
style.fontSize = 14;
style.labelOffsetMm = {{0.0, 0.0, 8.0}};
style.labelsVisible = true;
control.setDisplayedPointStyle(style);
control.showDisplayedPoints(true);
```

`setKeyPoints` 替换允许选择的模型点集合；`setDisplayedPoints` 替换持续显示集合，传入空数组分别清空对应集合。`keyPoints()`、`displayedPoints()` 返回定义副本。`displayedPointInfos()` 返回持续显示点的原始/显示坐标和不乘倍率的物理位移；外部点的位移恒为零。

原有 `setKeyPointStyle` / `keyPointStyle` / `showKeyPoints` 保留为持续显示样式/显隐的兼容接口；INI 样式分区仍使用 `visualization.keyPoint`。

样式半径单位 mm，文本大小为像素，labelOffsetMm 为世界坐标偏移。RGB 各分量在 0～1；半径必须有限且大于零；字体 1～256，球体分辨率 4～128。`setSelectionStyle` / `selectionStyle()` 单独控制选中标记样式，默认青色、半径 7 mm。普通关键点与选中点共用 Creator，不复制生成逻辑。

球体和 Label 均 PickableOff、UseBoundsOff，不拦截模型拾取，也不撑大 ResetCamera 的模型边界。选中与持续显示 Actor 独立，允许同一 ID 同时存在。文本显示名称（未命名时显示 ID）及当前世界 XYZ 坐标，随球体同步刷新。

`showDisplayedPoints(false)` 只隐藏持续显示标记，选中高亮独立保留；隐藏期间仍刷新坐标。`clearSelection()` 只清除当前选择。恢复原始形状时保留三类定义，模型标记恢复原始位置，外部标记保持固定坐标。

## 鼠标交互和选点配置

- 无修饰键左键单击在抬起时选点；空白位置或未命中允许选择的点时清除选择。
- 左键拖动旋转；Shift/Ctrl 等组合、VTK 默认中键平移、右键和滚轮缩放保留默认相机行为。
- 单击允许移动阈值默认 4 个 VTK 设备像素；拖动超过阈值后，即使鼠标又回到按下位置也不触发选点。
- 相机旋转、平移、缩放默认保留选中标记。`setCameraLocked(true)` 阻止鼠标相机移动，但仍允许左键单击选点；显式 ResetCamera 不受鼠标锁影响。

```cpp
PickingSettings picking = control.pickingSettings();
picking.enabled = true;
// onlyKeyPoints 仅为旧 INI 兼容字段；无论其值如何，始终仅允许选择 KeyPoints。
picking.tolerance = 0.009;          // VTK 窗口对角线的比例，范围 (0,1]
picking.clickTolerancePixels = 4;  // 0～100，Qt/QVTK 已转换的设备像素
picking.clearOnCameraInteraction = false;
control.setPickingSettings(picking);

control.selectPoint(25); // 程序按 ID 选择，与鼠标选择共用业务约束
const auto current = control.selectedPoint();
const auto ids = control.nearbyPoints({{700.0, 0.0, 0.0}}, 20.0);
control.clearSelection();
```

邻域查询依据当前显示网格的世界坐标与半径（mm），按 ID 排序；模型变形后按需重建 PointLocator。邻域查询不受关键点集合限制，查询结果不表示已经选中。

## 输出与随动

`pointPicked(PickedPointInfo)` 表示用户/程序选择或清除；`selectionUpdated(PickedPointInfo)` 在变形、倍率或复位后更新当前选择的数据。`valid=false` 表示无选择。

- `pointId`：当前模型点 ID。
- `originalPositionMm`：原始模型坐标。
- `displacementMm`：真实解算位移，不乘显示倍率。
- `displayPositionMm`：VTK 当前显示坐标。

Manager 直接从显示网格取模型标记位置，不重复计算或累计位移；外部标记使用固定位置。相机旋转不改变世界坐标，Billboard Label 自动朝向相机。`keyPointsChanged` / `displayedPointsChanged` 通知集合变化，`displayedPointsUpdated` 通知变形、倍率和复位后的数据刷新。模型版本切换清除选择和旧位移缓存。

## Classic 界面与 MVP

- `disp3dkeypointedit.ui`：全部加入/清空、按查询 ID 加入/移除、原始坐标包围盒加入/移除（包含边界，角点顺序不限）、查询模型点坐标。
- `disp3dkeypointselect.ui`：从关键点下拉列表添加持续显示点；输入外部点 ID、XYZ 与名称；加入全部关键点、按表格选中行删除、清空持续显示、独立显隐、原始坐标/物理位移切换。
- View 只收集控件值和选中 ID，经 ModelPresenter → ModelBackend → WorkbenchRuntime 的 `WorkbenchMarkers.cpp` → VisualizationControl 处理。后端通过 presentationChanged 返回表格和计数。
- 表格按 ID 保留选中行，删除请求在点击时捕获 ID，坐标刷新或删除前发生集合更新时不依赖旧行号；表格仅展示持续显示点，不再将整个关键点集合用于显示列表。

## 验证前端与测试

独立 VTK 验证程序仍使用 `.ui`；其“应用关键点/交互”预览动作显式将输入 ID 同时写入 KeyPoints 和 DisplayedPoints，便于观察样式。“仅选关键点”固定勾选且不可关闭；“清除选择”单独清除高亮。

`test_workbench_markers` 使用真实的两个 View、ModelPresenter、ModelBackend 和 WorkbenchRuntime，验证集合独立性、移除级联、外部点、包围盒、实时表格刷新与按 ID 删除。`test_keypoint_interaction` 补充默认全部关键点但零 Actor、非法整批输入不改变状态、固定点不随模型移动、两种球和文本同步刷新的测试。

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
