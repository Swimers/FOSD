# FOSD-Classic-QWidgets 业务闭环对接

> 2026-11-08：Classic 已迁移为三层 MVP；旧 Backend 请求转发链已删除，当前边界与测试命名见 [MVP 迁移指南](classic-mvp-ui-backend-connection.md)。

Classic 业务闭环由 `ui/FOSD-Classic-QWidgets/application` 组合，`apps/FOSD-Classic-QWidgets` 只保留启动入口与 EXE 构建，使用 `fosd-widgets` 预设，入口为 `build/fosd-widgets/bin/Release/FOSD-Classic-QWidgets.exe`。`apps/FOSD-Win` 继续使用共享界面库，不接入 Classic 应用服务。

## 架构与线程

```text
.ui / View → FormBinding → 功能 Presenter → 功能 FeatureBackend
                                                   ↓ 注入 WorkbenchServices
                                         WorkbenchModel（应用适配层）
                                          ├─ ProtocolControl
                                          │   └─ ConnectWorker + ProtocolWorker：同一通信线程
                                          │       └─ FrameRepository：只读波长快照
                                          ├─ CalculationControl
                                          │   └─ CalculationWorker + StorageWorker：同一解算线程
                                          │       └─ CalculationRepository：只读逐点位移结果
                                          └─ VisualizationControl：GUI 线程读取最新结果
                                               ↑ 同一个 vtkRenderWindow
                                   WorkbenchRenderSurface.ui / QVTKOpenGLNativeWidget
```

Presenter 只处理界面流程、文件选择、请求及响应。七类 Backend 保留各自的 panel 范围检查，通过 `WorkbenchBackend::setServices()` 使用同一套应用服务；ClassicWorkbench 可通过 `setUiBackend()` 替换单个 Backend。默认适配器与 WorkbenchModel 均在 GUI 线程，端口不应跨线程直接调用。

`WorkbenchServices` 只依赖 Qt Core，不向 View 或 Presenter 暴露 Control、VTK 或工作线程。运行服务分为生命周期、通信、显示、数据展示四个实现文件。没有服务或服务不识别的操作统一交回 `FeatureBackend::reportUnavailable()`；已实现操作的输入/执行错误返回具体错误。

成功响应表示请求通过同步校验并提交。通信实际状态、配置/模型确认、存储启停及异步错误，通过 `presentationChanged` 回显。通信、拆包、解析、解算、存储及可视化错误统一显示在主状态栏；连续相同错误不反复刷新。

计算线程每 10 ms 读取最新快照，可能跳过中间帧。原始帧由 ProtocolControl 的 `frameParsed` 连接到 StorageControl；标定和位移记录仍在解算线程中直接交给 StorageWorker。上层表格每 100 ms 更新，渲染默认 25 Hz，可独立调整。界面最多显示 2048 条波长、256 行模型观察数据；存储和逐点解算不受显示行数限制。调试接收区保留最近 65536 个字符，串口显示 UTF-8，UDP/TCP 显示十六进制；手动发送区按 UTF-8 原样发送。

## 控件对接范围

| 界面 | 已连接业务 |
| --- | --- |
| 通信 | 串口枚举、串口开关、UDP 绑定、TCP 客户端、手动发送、接收显示/清空、通道状态与诊断 |
| 波长与标定 | 真实波长表；通道显示筛选；波长置零；A/B 配置；12 路源序号和标定系数。源序号界面从 1 开始，后端转换为从 0 开始 |
| 保存 | 静态保存当前一帧原始波长；动态保存原始波长、标定硬件数据、全部模型点位移。文件扩展名 `.csv` / `.sqlite` / `.db` 选择写入格式 |
| 二维观察页 | 开始/暂停刷新模型的原始坐标、物理位移及变形坐标表；标定配置窗口。没有用波长数据伪造光谱或二维形状图 |
| 三维模型 | STL 载入与清除；提交到解算并等待 `modelAccepted`，按模型版本接结果；点数、三角面数、关键点数及最大物理位移 |
| 色图 | X/Y/Z/总位移，归一化/物理值、符号/绝对值、自动/手动色标、刷新率及显示倍率。手动范围在物理值模式下可编辑 |
| 关键点 | 全选、清空、ID 查找/选中、逐点加入/批量删除、原始位置/位移表、标记渲染；左键选点与位置随模型更新 |
| 场景 | 坐标轴及尺寸、三平面网格及尺寸、鼠标相机锁定、六个标准视角 |
| 硬件数据 | 从 `CalculationControl::calibratedReady` 读取 IFE 输入源序号和标定应变，不依赖三维模型已载入 |

开始/暂停/停止仅控制**变形显示**。通信、解算、存储独立继续运行；复位先暂停显示，再恢复原始形状，直到再次开始。清除模型会调用 `CalculationControl::clearModel()`，清空结果 Repository，后续仅标定硬件数据，直到重新输入模型。

设备专用命令、设备采样率、定间隔保存、光谱保存、模型合并/导出/颜色、区域盒选、相机跟随、独立追踪点和账户业务尚无对应实现，其操作统一报告不可用。应变阈值、算法轴向系数没有后端配置，相关输入明确禁用。单纯编辑本地通道复选框不会提交业务，点击“选择通道”才应用到波长表显示。

## Plate IFE

算法实现、板参数和传感器横向位置已迁入 `src/calculation/PlateIFE.h/.cpp`。`CalculationConfiguration::algorithm` 选择 HYY 或 Plate，默认 HYY；仍支持原有 HYY 分支的自定义 IFEFactory。两个高频计算接口没有报错输出。

正式三维工具栏的“板模型 / Plate IFE”会提取 qrc 内嵌 STL、提交模型及默认标定，采用 12 路参考波长 `1550 + 0.5 × i` nm、A=1、B=0、波长到微应变系数 1250。发送器和运动信号生成仍在 `tools/plate-simulation`，正式程序不链接仿真工具库。工具侧兼容头仅引用 calculation 的实现。

验证步骤：

1. 用 `fosd-widgets` 配置并构建 Release，在三维页点击“板模型 / Plate IFE”。
2. 通信页选择 UDP、本地 `127.0.0.1:9000`，点击打开网络；目标端口可使用默认有效值，不影响接收外部数据。
3. 在终端运行发送器，回到三维页查看周期变形、色图及选点数值。需要保存时，在通信页开启动态保存。

```powershell
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Release/FOSD-Classic-QWidgets.exe
# 另一终端（同样设置 Qt DLL 路径）：
& ./build/fosd-widgets/bin/Release/plate-udp-sender.exe --host 127.0.0.1 --port 9000 --rate 50 --frequency 0.35 --amplitude 15
```

“切换 HYY IFE”复用当前计算线程，退出 Plate 时恢复默认波长换算系数。使用实际设备前重新置零、核对标定和载入实际模型。算法切换不自动停止外部发送器或拆分存储会话；需要分文件记录时先停止保存。Plate 是固定几何的链路验证算法，不用于替代工程求解器。

## 测试

```powershell
# 需要匹配的 Release VTK DLL 和可创建 OpenGL 上下文的桌面
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-release
ctest --preset fosd-widgets-release
```

`test_workbench_runtime` 验证真实共享 Controls、无业务操作反馈、Plate/HYY 切换、无效标定保留、模型清除及 CSV/SQLite 三类写入。`test_workbench_main` 使用 ClassicWorkbench 与真实 Designer 表单，点击工具栏、UDP 和存储控件，经 Presenter/Backend 完成 UDP → 解析 → Plate IFE → 波长表/VTK 位移 → CSV；同时核对窗口指针、场景设置及不可用操作回显。应用适配位于 `ui/FOSD-Classic-QWidgets/model`，测试源文件和注册位于 `ui/FOSD-Classic-QWidgets/tests`，完整装配与这些集成测试仅 Classic 版本启用；FOSD-Win 不构建或链接它们。

上述测试不替代真实串口/设备验收；基础 UDP/TCP 拆包与各协议仍由原有模块测试覆盖。Debug 需另配与 Debug 库匹配的运行 DLL。
