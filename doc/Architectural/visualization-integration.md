# 三维可视化迁移与标量颜色映射

## 分步迁移安排

| 阶段 | 组件职责与 Control 接口 | 状态 |
| --- | --- | --- |
| 1. 位移/通用标量颜色映射 | ScalarColorMapper：数值选择、变换、色表；VisualizationControl：配置、结果和模型版本组织 | 已实现 |
| 2. 点击选点交互 | PointPicker / ModelInteractorStyle：模型专用拾取、原始/显示坐标与实际位移、鼠标门控及相机锁 | 已实现 |
| 3. 关键点配置渲染 | KeyPointCreator / KeyPointManager：统一球体和 Label 创建、样式、批量显隐及变形随动 | 已实现 |
| 4. 背景和场景环境 | SceneEnvironment：背景、网格、坐标轴与相机；沿用 showGrid / showAxes 等接口 | 后续迁移 |
| 5. 模型外观 | 统一模型颜色、边线、透明度及材质设置；新增独立外观配置结构和 Control 设置入口 | 后续迁移 |

颜色映射与材质颜色相互独立：关闭标量着色时使用 Actor 原有材质颜色，不改写它。后续阶段仍应保持 GUI 线程操作 VTK、Presenter 传递界面请求、应用适配器组合各 Control 的结构。

## 旧版行为及本次对应

依据旧版 `core/model/disp3dmodel.cpp` 的 SetupColorMapping / SetNormalizeMode / SetCustomRange / normalizeScalarArray，以及 `app/disp3d/disp3dwindow.cpp` 的 ApplyDisplayFrame。

- X/Y/Z 直接读取 `CalculationResult::displacementMap` 中的 dx/dy/dz，单位 mm，保留正负号；总位移为 `sqrt(dx² + dy² + dz²)`，以 hypot 避免中间平方溢出。
- `Normalized` 与旧版一致：每个当前帧、当前选定分量计算最小/最大值，使用 `(v-min)/(max-min)`，色表范围固定 0～1；跨度小于 `1e-12` 时所有色值置零。
- `Physical` 对应旧版取消归一化后的实际物理数值映射，**不是取 fabs**。范围可自动跟随当前数据或手动指定，例如 -10～10 mm；超出范围使用色表端点色。
- 独立 `ScalarValueMode::Absolute` 才表示先对标量取绝对值，再归一化或作物理值映射；默认 Signed 保留旧行为。
- 默认关闭着色；默认归一化、手动物理范围 0～50、256 色、Hue 0.6～0.1，与旧版设置一致。色相端点可配置为 0～1。

显示倍率只改变几何坐标，所有颜色均从真实位移计算。着色不修改解算结果和存储数据。切换分量、模式、范围或开关立即重新映射当前缓存帧，不等待新结果。

## 统一管线与接口

```text
只读 CalculationResult → 选择 X/Y/Z/总位移 ┐
                                        ├→ PointScalarField
外部点应变/温度等（显式模型版本与点 ID） ┘       ↓
                                     Signed / Absolute
                                               ↓
                              Normalized / Physical + 范围
                                               ↓
                              vtkLookupTable → vtkPolyDataMapper
```

`ScalarColorSettings` 集中定义开关、分量、映射方式、数值变换、范围策略和色相。推荐通过原子配置接口一次提交，避免连续多次触发渲染：

```cpp
using namespace fosd::visualization;
ScalarColorSettings settings = control.scalarColorSettings();
settings.enabled = true;
settings.field = ScalarField::DisplacementZ;
settings.mappingMode = ScalarMappingMode::Physical;
settings.valueMode = ScalarValueMode::Signed;
settings.rangeMode = ColorRangeMode::Manual;
settings.minimum = -10.0;
settings.maximum = 10.0;
control.setScalarColorSettings(settings); // false 时配置保留，并发 errorOccurred
```

也支持单项接口 `setScalarField`、`setScalarColoringEnabled`、`setScalarMappingMode`、`setScalarValueMode`、`setColorRangeMode`、`setColorRange`。设置分量本身不会自动开启着色。`scalarColorSettingsChanged` 可用于界面同步；`scalarColorRange()` 提供变换后数据的最小/最大值与实际色表范围，方便后续图例。

归一化模式始终使用当前帧最小/最大值及色表 0～1，暂存的手动范围在切回 Physical 后生效。物理值自动范围遇到常量场时扩展出非零色表区间；原始标量保持不变。

范围必须有限且 minimum < maximum，Hue 必须在 0～1，非法配置不覆盖有效配置。字段必须覆盖模型所有点，不能用缺失点补零。VTK 数组 `FOSD.RawScalars` 保留选定分量的原值，`FOSD.ColorScalars` 为变换后的显示值；均不会改写共享 Repository。

## 接入后续标量场

不需要为每个新物理量复制色表逻辑，也不应把传感器应变数组直接当成模型点应变：调用者须先完成节点对应或空间插值，然后提交统一点标量结构。

```cpp
PointScalarField field;
field.id = "strain.xx";
field.label = "X strain";
field.unit = "microstrain";
field.modelRevision = acceptedModelRevision;
field.values = nodalStrains; // QMap<core::PointId, double>，覆盖全部模型点
control.setPointScalarField(field); // 选择 Custom；着色开关独立设置
control.setScalarColoringEnabled(true);
```

字段携带 id、名称、单位及模型版本。Control 检查模型版本、点 ID、数量与有限值，复制值类型输入，不保留调用者可变指针；当前只缓存一个自定义字段。自定义场与位移结果独立，需由提供者主动更新；切回内置位移场后仍可使用当前缓存解算帧。

模型重新加载、卸载、更换模型版本、恢复原始形状时清理缓存和 VTK 色值，避免旧数据染到新模型。恢复原始形状后，新位移帧会重新显示；自定义场需重新提交。关闭着色保留当前源数据，可重新开启。所有 Control / Mapper 接口在 GUI 线程调用。

## 验证前端与测试

`ui/Test-VtkOpenGLNativeWidgets/views/VtkValidationWindow.ui` 新增颜色配置控件，值经 Presenter → Backend → VisualizationControl 传递；应用按钮一次提交配置。分量顺序为 X/Y/Z/总位移，物理范围单位为 mm。应变等自定义场暂由后端接口输入。

无窗口 VTK 单元测试需要完整运行库，仓库裁剪 SDK 不含 DLL，因此通过独立选项开启，不改变默认无 VTK DLL 的测试运行条件：

```powershell
cmake --preset vtk-native-widgets -DFOSD_BUILD_VISUALIZATION_TESTS=ON
cmake --build --preset vtk-native-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
ctest --preset vtk-native-widgets-release
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe --smoke-test | Out-Host
```

`test_visualization` 检查四种位移值、带符号/绝对值、归一化/物理值、自动/手动范围、色表方向、常量/零场、超大数值、非法输入、自定义场、Control 即时切换、倍率独立性和模型生命周期。OpenGL 冒烟检查在真实 UDP 到解算结果的链路上额外验证四分量颜色数组、物理范围和复位清理。

Debug 可编译同一测试，但运行须提供 Debug VTK DLL，不能使用 Release DLL。完成专项验证后，可用 `cmake --preset vtk-native-widgets -DFOSD_BUILD_VISUALIZATION_TESTS=OFF` 还原默认测试集合。

关键点和鼠标交互接口详见 [关键点与选点对接](keypoint-picking-integration.md)。
