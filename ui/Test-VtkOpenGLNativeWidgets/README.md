# Qt/VTK 全后端集成验证前端

本程序通过 `VtkValidationWindow.ui` 构建 `QVTKOpenGLNativeWidget`、通信参数、标定参数、存储选择及渲染按钮。C++ View 只执行 setupUi、收集字段、显示状态及管理控件持有的 RenderWindow。按钮和数据经 Presenter 调用应用适配器，View 不直接访问业务 Control。

## View / Presenter 结构

该目录采用被动 View + Presenter 结构，并通过两个抽象接口与应用层解耦：

```text
VtkValidationWindow.ui
        ↓ setupUi
VtkValidationWindow ──实现──> IVtkValidationView
        │ 信号                    ↑ Presenter 读取窗口、更新界面
        ↓                         │
VtkValidationPresenter ──调用──> IVtkValidationBackend
                                      ↑
                           apps 中的 VtkValidationBackend
```

View 负责 Qt 控件、文件对话框、字段采集和界面状态，不直接包含通信、解算、存储或可视化业务逻辑。Presenter 负责检查绑定状态和 OpenGL 上下文、转发命令、处理成功或失败结果。Backend 接口代表应用能力，具体实现位于 `apps/Test-VtkOpenGLNativeWidgets`，因此 Presenter 可以通过 Fake View / Fake Backend 独立测试。

`vtkRenderWindow*` 的所有权始终属于 View：`VtkValidationWindow` 创建 `vtkGenericOpenGLRenderWindow` 并交给 `QVTKOpenGLNativeWidget`，Presenter 读取同一裸指针并传给 Backend。Backend 和 `VisualizationControl` 只借用该指针，不创建替代窗口，也不负责释放它。Presenter 绑定后会比较前后端指针，防止错误窗口被接入。

## 源码文件职责

| 文件 | 主要功能 |
| --- | --- |
| `VtkValidationWindow.ui` | 定义验证窗口布局，包含 `QVTKOpenGLNativeWidget`、通信配置、标定参数、模型导入、存储配置、形变倍率以及绑定、示例球体、相机和刷新按钮。只描述界面，不包含业务代码。 |
| `IVtkValidationView.h` | Presenter 面向的 View 抽象接口。提供文件选择、获取前端 `vtkRenderWindow*`、检查 OpenGL 上下文、显示绑定信息和状态文本等能力。 |
| `VtkValidationWindow.h` | 声明实际 QWidget View。持有生成的 UI 对象和 `vtkGenericOpenGLRenderWindow`，实现 `IVtkValidationView`，并以 Qt 信号公开用户操作。 |
| `VtkValidationWindow.cpp` | 初始化 `.ui` 和原生 VTK 窗口，将 RenderWindow 设置给 `QVTKOpenGLNativeWidget`；连接按钮；汇总输入字段为 `QVariantMap`；负责文件对话框、状态显示、按钮启用状态和前后端指针地址显示。析构时先释放原生控件，再释放 RenderWindow。 |
| `IVtkValidationBackend.h` | Presenter 面向的应用后端端口。声明窗口绑定/解绑、查询已绑定窗口、示例球体、清除、相机重置、刷新以及通用命令执行接口；所有窗口指针均为同步借用指针。 |
| `VtkValidationPresenter.h` | 声明 View 与 Backend 之间的协调器及其槽函数，不依赖具体 QWidget、通信或计算 Control。 |
| `VtkValidationPresenter.cpp` | 实现窗口绑定与指针一致性校验、解绑、OpenGL 上下文门控、模型/存储文件选择、通用按钮参数转发、渲染命令调用和状态回显；析构时主动通知 Backend 解绑。 |
| `tests/PresenterTests.cpp` | 使用 Fake View 和 Fake Backend 验证 Presenter，不创建真实 OpenGL 窗口。覆盖参数透传、文件选择取消、窗口指针传递、上下文未就绪、重复命令、绑定失败和析构解绑。 |
| `CMakeLists.txt` | 定义 Presenter 静态库、QVTK/Qt 依赖接口库和实际 Widgets 静态库；查找匹配 Debug/Release 的 `vtkGUISupportQt`，并在启用测试时注册 Presenter 单元测试。 |
| `README.md` | 记录模块结构、数据链路、操作流程、构建运行方法和自动验证范围。 |

主要交互分为两类：

- 专用渲染信号：绑定、解绑、示例球体、清除、重置相机和刷新，分别进入 Presenter 对应槽函数。
- 通用业务信号：通信、模型、标定、存储和形变倍率按钮统一由 `requestAction()` 收集参数，发送 `actionRequested(action, parameters)`，再由 Presenter 转交 Backend。

## 入口和数据链路

```text
VtkValidationWindow.ui → View → VtkValidationPresenter → VtkValidationBackend
                                                     ├─ ProtocolControl
                                                     │    └─ ConnectControl（托管在协议线程）
                                                     ├─ CalculationControl（解算线程）
                                                     │    └─ StorageControl（托管在解算线程）
                                                     └─ VisualizationControl（GUI 线程）

UDP / TCP → 拆包 → MP / JCS 波长解析 → FrameRepository（只读快照）
                                      ├─ frameParsed → 原始波长存储
                                      └─ 解算线程轮询 → 标定 / HYY / 逐点插值
                                                         ├─ 标定数据和位移存储
                                                         └─ CalculationRepository
                                                              ↓ GUI 每 33 ms 读取最新值
                                                         VTK 网格变形与刷新
```

完整构建 communication、core、protocol、calculation、storage、visualization。验证程序与主程序仍使用不同构建目录，主程序无需随验证工具启动。独立 `fosd-widgets` 预设仍为纯工作台预览。

前端创建 `vtkGenericOpenGLRenderWindow`，传给 QVTK 控件；同一指针经 Presenter / Backend 传入 `VisualizationControl(nullptr, window)`，后端不另建窗口。界面显示前后端地址。VTK 操作只发生在 GUI 线程，后台通过只读 Repository 交换数值数据。

## 使用流程

1. 导入 STL 模型，坐标单位为毫米；VTK 点下标作为模型点 ID。原始坐标提交到 CalculationControl，收到 modelAccepted 后绑定模型版本；旧模型结果不会更新新模型。
2. 选择 UDP 本地监听或 TCP 客户端，填写地址、端口并打开。串口可打开和关闭，但仍按指令通道处理，不作为波长数据源。
3. 收到波长后设置通道排列、标定系数、A/B，点击“以最新波长设基线 / 开始解算”。排列是波长记录数组的零基下标，默认为 0～11，通道倍率为 1。A/B 默认 1/0，仅是可编辑的验证初值，实际参数应由设备标定提供。
4. 继续接收数据时，真实 HYY 输出经逐点插值、Repository 更新 VTK 模型。显示倍率只影响显示坐标，不修改原始模型、解算结果或存储数据。恢复原始形状后，下一帧新结果恢复实时变形。
5. 选择 CSV / SQLite，点击“选择文件 / 开始存储”。原始波长、标定数据、所有模型点位移均交给现有 StorageControl 写入。点击“停止存储”关闭写入。

解绑只断开渲染窗口，后台继续接收、计算和存储；重新绑定后显示最新结果。关闭应用先停止通信/协议，再由 CalculationControl 排空任务并关闭存储，最后销毁可视化对象与前端窗口。

模型加载及变形显示由生产模块 MeshManager / DeformationRenderer / VisualizationControl 实现，保持原始拓扑，每次从原始坐标加位移，避免累计变形。标量着色、选点、坐标轴等预留接口尚未实现，也未接到此界面按钮。示例球体仍只作原生 OpenGL 基础验证。

## 配置、构建和测试

```powershell
cmake --preset vtk-native-widgets
cmake --build --preset vtk-native-widgets-release
ctest --preset vtk-native-widgets-release
```

该预设开启全部后端及全项目测试，关闭主程序和其他独立应用。也提供 `vtk-native-widgets-debug` 构建 / 测试预设。输出为 `build/vtk-native-widgets/bin/<配置>/Test-VtkOpenGLNativeWidgets.exe`。

运行需要与 Debug / Release 匹配的 VTK DLL；仓库中的 SDK 只有头文件和导入库。本机 Release 示例：

```powershell
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe
```

可用 `-DFOSD_VTK_NATIVE_RUNTIME_DIR=<对应配置的 DLL 目录>` 配置 Visual Studio 的调试 PATH。真实 QVTK 显示需要桌面 OpenGL 上下文，不使用 offscreen 平台验证。

## 自动验证

CTest 包含通信、拆包、MP / JCS 解析、解算、CSV / SQLite 存储、全链路、工作台以及 Presenter 测试。Presenter 测试检查指针转交、上下文门控、按钮参数透传、选择文件取消和后端状态回显。

原生 OpenGL 和完整后端组合另有可见窗口冒烟检查，不加入默认无窗口 CTest：

```powershell
# 按前文设置 Release DLL 的 PATH。
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe --smoke-test `
  --screenshot ./build/vtk-native-widgets/native-smoke.png | Out-Host
if ($LASTEXITCODE -ne 0) { throw '集成验证失败' }
```

先检查 RenderWindow 指针一致、实际球体像素、重复绑定和解绑，然后绑定真实本地 UDP，发送基线及变化波长，加载临时 STL，运行真实标定和 HYY、写入临时 CSV，并逐点比较 VTK 显示坐标与解算后的模型坐标，要求存在非零位移。检查失败或超时返回非零退出码；临时文件自动清理。`--screenshot` 保存基础球体检查的实际帧缓冲。
