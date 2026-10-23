# FOSD Widgets 与 MVP 对接

本目录迁移自旧项目 `src/app` 的 20 个 Qt Designer 表单，保留控件 objectName、布局、图标、图片和界面样式。`conncect` 目录纠正为 `forms/connection`。旧 `.cpp/.h` 中的通信、文件配置、用户数据库、IFE、VTK 渲染对象及刷新线程没有迁移。

## 目录与职责

| 路径 | 内容 |
| --- | --- |
| `forms/` | 通信/诊断、二维及参数、三维及 Dock、关键点、追踪点、数据显示、两种主窗口、登录/注册/找回/账户表单 |
| `views/LegacyViews.*` | 20 个表单的独立 View 包装，只做 setupUi 和资源初始化 |
| `views/FormBinding.*` | `IPanelView` 的控件适配，收集当前表单输入、发送交互事件、应用显示数据 |
| `views/WorkbenchView.*` | 原 OldMainWindow 三页主窗口、辅助窗口和三维 Dock 的组合与显示 |
| `presenters/PanelPresenter.*` | 每个表单独立的 Presenter 实例；请求编号、响应匹配、成功/失败显示 |
| `presenters/WorkbenchPresenter.*` | 页面路由、文件选择流程、后端绑定及请求分发 |
| `contracts/` | 无业务模块依赖的 View 接口、后端接口、请求/响应类型 |
| `controls/` | 原 QCustomPlot、SwitchControl，以及预留三维挂载位置的 RenderHostWidget |
| `resources/` | 原 qrc 资源包与实际使用的深浅 QSS，修复失效路径后编入可执行文件 |

保留了 QCustomPlot 源码内的版权和许可证说明；SwitchControl 保留旧控件实现。UI 自身只依赖 Qt Core/Widgets/PrintSupport，不链接通信、解算、存储或 VTK。Presenter 库只依赖 Qt Core。

```text
Designer 控件 → FormBinding → PanelPresenter → WorkbenchPresenter
                                                ↓ queued
                                      IWorkbenchBackend::execute
                                                ↓ queued
                      completed / presentationChanged → View 更新
```

所有 View、Presenter 在 GUI 线程创建、使用和销毁。Backend 可放到其他有事件循环的线程；它的创建与销毁由应用层管理。跨线程只传递请求/响应的 Qt 值类型，不传递 QWidget、VTK 对象或旧业务对象。

## 应用入口

`apps/FOSD-Win/FOSD` 继承 `WorkbenchView`，持有 `WorkbenchPresenter`；主程序启动即显示迁移后的三页工作台。原有 ProtocolControl、CalculationControl 及协议到存储的连接保留在应用组合层。

Presenter 的本地行为包括页面切换、辅助窗口和 Dock 显示、最小化/全屏/关闭、主题切换、清空文本和全选通道。设备打开/发送、模型操作、渲染控制、保存、认证等操作交给 Backend。未绑定 Backend 时明确显示“后端适配器尚未接入”，不改变连接状态、不假装保存或登录成功。登录窗口只收集用户意图，不提供权限判断或密码持久化。

`RainbowWindowView` 作为原备选主窗口壳单独保留，当前启动入口沿用旧项目实际使用的 OldMainWindow 工作台。账户管理等辅助表单可通过 `showPanel(PanelId::Account)` 打开；哪些角色可访问、认证后如何导航由后续应用适配层确定。

## Backend 接口

应用层继承 `IWorkbenchBackend`，实现 `execute(const UiRequest &)`，然后调用：

```cpp
// 在应用组合层完成；adapter 的生命周期由应用管理。
window.setUiBackend(adapter);
// 或直接使用 WorkbenchPresenter：
presenter.setBackend(adapter);
```

每个请求包含唯一 `id`、`panel`、`action` 和 `fields`。`action` 通常为原表单按钮/工具栏动作的 objectName，例如 `pbtn_OpenNet`、`pbtn_ModelLoad`、`actionStart`。复选框、开关、下拉框、滑条的用户变化使用各自 objectName；对话框按钮盒使用 `accepted/rejected`。只收集该表单自己的输入，不递归带入其他表单，尤其不会把隐藏登录窗口的输入附到通信请求。

| 请求来源 | 示例 action | 后续适配目标 |
| --- | --- | --- |
| Connection / ConnectionDebug | `pbtn_OpenSerial`、`pbtn_OpenNet`、`pbtn_SerialSend`、`pbtn_RefreshSerial` | ConnectControl/设备指令与接收显示 |
| Plot / PlotSettings | `pbtn_ChangeOriginalCoordinates`、`pbtn_SenerSet` | 模型/标定输入及二维显示数据 |
| ModelPanel / ScenePanel / Visualization | `pbtn_ModelLoad`、`pbtn_ScalarSet`、`cbox_CareamLockAble`、`actionStart` | VisualizationControl |
| KeyPointEdit / KeyPointSelect / ColorPicker | `pbtn_SelectCube`、`pbtn_AddPoint`、`accepted` | 可视化关键点与颜色配置 |
| TrackingManager / TrackingData / HardwareData | `pbtn_LoadFile`、`pbtn_Refresh`、`checkBox` | 追踪点、标定及解算结果展示 |
| Login / Registration / Recovery / Account | `btn_Login`、`pbtn_RegisterSet`、`pbtn_ForgetSet`、`pbtn_UserSet` | 后续用户服务，当前未实现 |

旧对象名中的拼写也保留为接口标识。具体可用操作以各 `.ui` 的 objectName 为准，不从按钮中文文本推断。

输入字段编码：文本为 QString，复选框/开关为 bool，滑条/数值框为数值，下拉框为 `{index, text}`；表格选中行存于 `<tableName>.selectedRows`。Backend 需自行解析输入、检查权限和参数，并调用当前模块的公开接口。模型导入/合并/导出及追踪点导入由 Presenter 通过 View 选文件，把路径放入 `filePath`，取消选择不会发请求；这些对话框仅在 Backend 已接入时打开。

Backend 完成后发出 `completed(UiResponse)`，保留请求的 `id/panel`，设置 `success/message/fields`。只有匹配待处理请求的响应会被接受；失败响应只显示错误，不应用字段。重复、错误表单或已取消的响应被忽略。认证类提交完成后清除敏感输入。替换 Backend 前会清空待处理状态，但应用仍须自行确保旧适配器已停止处理此前收到的业务命令。

连续数据可通过 `presentationChanged(panel, fields)` 推送，不需要伪造请求编号。显示字段规则如下：

- 文本标签、编辑框：`fields[objectName] = text`。
- 下拉框：`{items: QStringList, index: int}`，或 `{text: QString}`；不提供 items 时保留现有选项。
- 复选框/滑条/数值框：bool 或数值。所有程序更新均阻断控件信号，避免反馈成新请求。
- 表格：`{headers: QStringList, rows: QVariantList<QVariantList>}`，支持 QTableView/QTableWidget，显示数据不可编辑。
- `cplot_Spectrum`：`{x: QVariantList, y: QVariantList}`，更新一条显示曲线；点数不一致的更新忽略，非有限点过滤。这只是绘图适配，不进行解包或计算。

本接口以表单+控件名为前端协议，Backend 可在适配层映射为强类型领域命令；不能让 Presenter 重新承担拆包、解算或磁盘写入。

## 三维宿主

原 UI 的 `QVTKOpenGLNativeWidget` 提升控件替换为保留 objectName `vtk_openGL` 的 `RenderHostWidget`。当前可视化业务仍在开发，裁剪 SDK 也没有配套运行 DLL，因此本次不实例化 VTK，不将空宿主当作已完成的三维显示。

后续在应用/前端适配层创建 QVTK 控件及 RenderWindow，通过 `window.renderHost()->setSurface(qvtkWidget)` 挂载，再调用可视化模块接口；Renderer 仍归 VisualizationControl 管理。宿主持有当前 surface 的 Qt 父子所有权，替换时旧 surface 被解除父对象，交调用者处理；主动删除 surface 后 QPointer 自动清空。所有操作在 GUI 线程执行。

## 构建及验证

```powershell
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-debug
ctest --preset fosd-widgets-debug
# Release 可使用 fosd-widgets-release 构建/测试预设。
$env:PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Debug/FOSDWidgets.exe
```

此预设通过 `FOSD_BUILD_WORKBENCH_APP` / `FOSD_WORKBENCH_BUILD_TESTS` 构建独立预览程序和测试，构建目录为 `build/fosd-widgets`，无需通信、解算、存储或 VTK。入口为 `apps/FOSDWidgets/main.cpp`，只组合 View 与 Presenter。主软件仍通过 `windows-msvc2017` 预设构建 `FOSD`，复用同一工作台库。完整预设说明见 [独立构建预设](../../doc/build-presets.md)。

CMake 会处理 20 个 `.ui` 的 AUTOUIC、Qt 元对象和 qrc。测试覆盖全部表单及资源、导航和 Dock、敏感字段隔离、请求响应匹配、跨线程适配器、断开后的反馈、表格/曲线及渲染宿主生命周期。测试在其工作目录生成 `workbench-migration.png` 供布局检查。
