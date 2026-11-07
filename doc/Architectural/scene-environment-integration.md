# 场景环境与相机对接

`SceneEnvironment` 管理世界坐标轴、三平面网格、背景、地面/天空及相机；`VisualizationControl` 提供统一设置、渲染和错误信号。VTK 对象仍全部在 GUI 线程操作，环境不进入解算、存储或模型点数据。

## Control 配置入口

| 接口 | 作用 |
| --- | --- |
| `setAxesSettings` / `axesSettings` | 轴显隐、轴长、原点、标签及字体 |
| `setGridSettings` / `gridSettings` | XZ/XY/YZ 独立显隐、原点、半宽、密度、颜色、透明度及线宽 |
| `showAxes(bool)` / `showGrid(bool)` | 坐标轴总开关 / 全部网格开关 |
| `showGrid(GridPlane, bool)` | 单独控制某一平面 |
| `setBackgroundSettings` / `backgroundSettings` | 纯色或底部到顶部的渐变背景 |
| `setBackground(r,g,b)` | 设置纯色并关闭渐变 |
| `setModelColor` / `modelColor` | 模型基础 RGB 颜色，各分量为有限的 [0,1]；不改变标量云图的开启状态 |
| `setWorldSettings` / `worldSettings` | 可选地面、天空球、实色或图片纹理 |
| `setCameraPreset` | 等距、前/后/左/右/顶/底视图并适配模型 |
| `setCameraSettings` / `cameraSettings` | 透视/正交、视角、正交比例及鼠标锁 |
| `setCameraPose` / `cameraPose` | 显式世界坐标位置、焦点和向上向量 |
| `setCameraLocked` / `resetCamera` | 锁定鼠标相机交互 / 保持方向、适配模型 |

bool 设置接口失败时保留原设置，Control 通过 `errorOccurred(QString)` 上报。单组配置原子提交；应用同时修改多个独立配置组时应先保证各组参数有效。

## Classic 三维界面（2026-11-07）

`disp3dviewdockpanel.ui` 新增模型纯色、背景模式、背景/底色和渐变顶色控件。颜色按钮打开 `sceneColorDialog.ui` 定义的 Qt 色卡，确认后提交，取消保持原值。纯色模式禁用顶色按钮但保留其颜色，切回渐变时恢复使用；背景沿用 `BackgroundSettings` 持久化。模型基础颜色保存于运行时 INI 的 `[visualization.appearance] modelColor`，加载新模型时继续使用；关闭标量着色时显示基础颜色，开启时仍使用标量云图。

`disp3dwindow.ui` 定义 SelectedPoint 状态栏，显示顶点 ID、原始位置和变形显示位置（mm，包含显示倍率），跟随 `pointPicked/selectionUpdated` 更新；清除选择或卸载模型后恢复未选择提示。

坐标显示统一保留两位小数，包括点标签、关键点坐标及显示点表格；计算和存储数据精度保持不变。底部状态栏采用等宽字体，ID 预留 20 字符、每个坐标分量预留 12 字符并右对齐，未选择时也保留相同占位。超出预留长度的极大坐标使用两位小数的科学计数法，避免后续字段位移。

模型载入后需点击 Start 启动显示：运行时禁用 Start、启用 Pause/Stop；暂停后启用 Start/Stop、禁用 Pause；停止或复位后恢复待启动状态。未载入模型时禁用 Start/Pause/Stop/Reset。暂停与停止只影响变形显示，通信、解算和存储继续运行。

Release 全部 18 个测试目标通过，包含色卡确认/取消、颜色持久化、工具栏状态、选点坐标随变形与复位更新，并检查了三维界面截图。

## 坐标轴与网格

```cpp
using namespace fosd::visualization;
auto axes = control.axesSettings();
axes.visible = true;
axes.lengthMm = 100;
axes.originMm = {{0, 0, 0}};
axes.labelsVisible = true;
control.setAxesSettings(axes);

auto grid = control.gridSettings();
grid.visible = {{true, true, false}}; // XZ、XY、YZ
// 每个平面覆盖原点两侧 [-halfExtentMm,+halfExtentMm]。
grid.halfExtentMm = 1500;
grid.divisions = 50;                // 原点到边缘的分格数量，间距 30 mm
// 也可独立设 grid.color / opacity / lineWidth / originMm。
control.setGridSettings(grid);
```

XZ 对应固定 Y，XY 对应固定 Z，YZ 对应固定 X；平面通过 originMm。每个平面有 `2*(2*divisions+1)` 条线，直接合并为一个 PolyData/Actor，避免为每条线创建过滤器。

轴长、原点和网格范围使用 mm。旧版 SetAxesLength 的滑条值不是物理长度；如需复现旧滑条曲线，由应用层转换为 `50000*(size/100)^2` 后传入 lengthMm。网格默认线色 0.7/0.7/0.7、透明度 0.1、线宽 1，与旧版一致。divisions 限定 1～500，线宽 (0,20]，RGB/透明度 0～1，字体 1～256。修改密度、范围或原点保留显隐状态。

全部环境 Props 均不可拾取，且不参与相机适配边界。默认隐藏、按需创建/挂载；重复应用不会叠加 Actor。

## 相机坐标与镜头

迁移旧版 Z-up 的六向视图约定，表中的方向是“从模型中心看向相机”的方向：

| 视图 | 相机所在方向 | ViewUp |
| --- | --- | --- |
| Front / Back | -Y / +Y | +Z |
| Left / Right | -X / +X | +Z |
| Top / Bottom | +Z / -Z | +Y |
| Isometric | (-X,-Y,+Z) | +Z |

```cpp
control.setCameraPreset(CameraPreset::Isometric);
auto camera = control.cameraSettings();
camera.projection = CameraProjection::Parallel;
camera.parallelScaleMm = 500; // 可见区域垂直半高，整个高度为 1000 mm
camera.viewAngleDegrees = 45; // 透视模式的垂直视角
control.setCameraSettings(camera);
```

`resetCamera()` 保持当前方向和垂直视角，依据当前显示模型边界及窗口宽高比计算适配距离/正交比例。避免调用 VTK 8 的 ResetCamera(bounds) 将透视视角强制恢复为 30°。切换预设会适配模型，因此重新计算正交比例；手动设置比例后不会自动适配，便于独立控制缩放。

CameraPose 要求位置/焦点不同，ViewUp 非零且不平行于视线，所有坐标有限；向上向量在相机中正交化。viewAngleDegrees 范围 1～170，parallelScaleMm 大于零。设置可在 Renderer 绑定前缓存，绑定后应用；cameraSettings / cameraPose 读取当前相机实际值，包含用户鼠标缩放和旋转后的状态。

鼠标锁与已有 ModelInteractorStyle 对接，显式切换视图、设置姿态、resetCamera 仍可执行；锁定时仍可选点。相机操作保留当前选中点和标记，不改变模型坐标。新模型加载时适配，变形/倍率刷新只更新剪裁范围，不逐帧重置用户镜头。

## 背景与地面/天空

```cpp
BackgroundSettings background;
background.gradient = true;
background.bottomColor = {{0.08,0.12,0.18}};
background.topColor = {{0.3,0.45,0.7}};
control.setBackgroundSettings(background);

WorldSettings world;
world.groundVisible = true;
world.skyVisible = true;
world.centerMm = {{0,0,0}};
world.groundHalfExtentMm = 10000;
world.groundYmm = -100;
world.skyRadiusMm = 20000;
world.groundTexturePath = "D:/assets/ground.jpg";
world.skyTexturePath = "D:/assets/sky.jpg";
control.setWorldSettings(world);
```

地面沿用旧版 XZ 平面，Y 固定为 centerMm.y + groundYmm；这一约定与模型相机 Z-up 独立。天空是以内表面渲染的纹理球，中心为 centerMm。World 尺寸为显式 mm 参数，不再固定旧版飞机场景的超大倍率与坐标。图片留空则使用 groundColor / skyColor；不需要外部图片即可使用实色环境。

纹理通过 Qt QImage 解码 PNG/JPEG 等可用格式，支持绝对路径、当前工作目录相对路径和 `:/` 资源路径；不搜索父目录。地面启用 Repeat，天空不重复，均启用插值。Qt 图片转换为 RGBA 并翻转行顺序交给 VTK，支持中文路径。解码失败返回错误并保留现有 World；路径未变时复用已加载纹理，切换显隐无需重新读取文件。

World 默认关闭，验证前端先提供坐标轴/网格、纯色/渐变及相机参数控件；地面/天空纹理由 Control 接口配置。

## 渲染与生命周期

场景组件不直接调用 Render；Control 统一渲染。Renderer StartEvent 观察器更新剪裁范围，覆盖鼠标交互和结果刷新两条路径。环境可以扩展远剪裁面，但保留能显示模型的近剪裁面，防止大网格/天空把小模型裁掉。镜头适配始终只依据模型 Actor；没有绑定模型时，使用可见的非环境 Props，支持现有示例球体。

Scene 借用 Renderer，模型 Actor 使用弱指针；Control 在模型卸载前解除绑定。切换 Renderer、clear 或析构时移除自己的 Props 和观察器，不删除模型或其他业务 Actor。窗口解绑/重新绑定沿用同一个 Renderer，因此配置和相机姿态保留。

## 验证

```powershell
cmake --preset vtk-native-widgets -DFOSD_BUILD_VISUALIZATION_TESTS=ON
cmake --build --preset vtk-native-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
ctest --preset vtk-native-widgets-release
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe --smoke-test | Out-Host
```

`test_scene_environment` 覆盖轴设置、三平面网格几何、显隐/重建、Renderer 迁移、配置校验、背景、纹理解码与缓存、七种视图、投影、镜头适配和姿态校验。真实 OpenGL 冒烟检查额外验证 Presenter 到场景/镜头参数的传递、开启环境不改变模型适配距离、正交投影，以及环境 Props 不拦截模型选点。Debug 测试运行需要匹配的 Debug VTK DLL。
