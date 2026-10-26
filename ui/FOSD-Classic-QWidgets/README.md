# FOSD Classic QWidgets：旧版 UI 迁移与 MVP 对接

经典界面位于 `ui/FOSD-Classic-QWidgets`，命名空间为 `fosd::ui::classic`。保留 19 个旧版 Designer 表单、控件 objectName、图片和 qrc URL。主窗口只使用 `oldmainwindow.ui`；`rainbowmainwindow.ui` 及旧包装类已删除。旧设备通信、解包、IFE、数据库、VTK 业务和线程实现未搬入 View。

## 文件结构

各功能界面统一位于 `apps/`。Workbench 是跨功能组合层，相关 View、Presenter、Backend 和接口直接位于 `apps/` 根目录；每个具体功能目录仍按 `views/`、`presenters/`、`backends/` 划分。每个保留的 `.ui` 旁都有同名 `.h/.cpp`。例如 `apps/connection/views/connectwindow.ui/.h/.cpp` 定义 ConnectionView，`apps/account/views/login.ui/.h/.cpp` 定义 LoginView。头文件声明 Ui 包装、IView 接口与控件意图 signals；源文件只执行 setupUi、字段读写和信号转发。

```text
FOSD-Classic-QWidgets/
├─ apps/
│  ├─ WorkbenchView.h/.cpp       # 组合页面、Dock 和主窗口布局
│  ├─ WorkbenchPresenter.h/.cpp  # 组装各功能 Presenter
│  ├─ WorkbenchBackend.h/.cpp    # 注册各功能 Backend
│  ├─ IWorkbenchView.h           # Workbench View 接口
│  ├─ connection/
│  │  ├─ views/       # connectwindow、connectdebug 的 .ui/.h/.cpp
│  │  ├─ presenters/  # ConnectionPresenter
│  │  └─ backends/    # ConnectionBackend
│  ├─ mainwindow/
│  │  ├─ views/       # oldmainwindow
│  │  ├─ presenters/  # MainWindowPresenter
│  │  └─ backends/    # MainWindowBackend
│  └─ plot、visualization、model、tracking、account/  # 同样分层
├─ common/
├─ controls/
├─ resources/
└─ tests/
```

| 目录 | 表单 / 职责 | Presenter / Backend |
| --- | --- | --- |
| `apps/` 根目录 | WorkbenchView 组合页面和 Dock，WorkbenchPresenter 组装功能 Presenter，WorkbenchBackend 注册功能适配器 | WorkbenchPresenter / WorkbenchBackend |
| `apps/mainwindow/` | views/ 中的 oldmainwindow 及其主页面行为 | MainWindowPresenter / MainWindowBackend |
| `apps/connection/` | views/ 中的 connectwindow、connectdebug | ConnectionPresenter / ConnectionBackend |
| `apps/plot/` | views/ 中的 disp2dwindow、disp2dpara | PlotPresenter / PlotBackend |
| `apps/visualization/` | views/ 中的 disp3dwindow；views/docks/ 下的 disp3dviewdockpanel、disp3dinfodockpanel | VisualizationPresenter / VisualizationBackend |
| `apps/model/` | views/ 中的 disp3dmodeldockpanel、disp3dkeypointedit、disp3dkeypointselect、disp3dcolorpicker | ModelPresenter / ModelBackend |
| `apps/tracking/` | views/ 中的 dispdata、trackingpointmanager、trackingpointdatatable | TrackingPresenter / TrackingBackend |
| `apps/account/` | views/ 中的 login、userregister、userforget、usermanage | AccountPresenter / AccountBackend |
| `common/` | IView、ViewEvents、IBackend、请求响应类型、FormBinding、ViewFactory、请求生命周期和功能范围检查 | PanelPresenter / FeatureBackend 提供共用机制 |
| `controls/` | 原 QCustomPlot、SwitchControl、RenderHostWidget | 只提供控件能力 |
| `resources/` | 原 qrc、图片及界面样式 | qrc URL 保持不变 |
| `tests/` | 表单、资源、事件、路由、线程及后端切换验证 | Qt Test |

## 分层与事件

```text
Designer 控件 → FormBinding / ViewEvents → 当前表单的功能 Presenter
                     │                          │
                     └→ View 的控件级 signal    ├→ 本地页面、窗口和文件选择操作
                                                └→ 对应功能 IBackend::execute(UiRequest)
                                                           ↓ queued
                                          completed / presentationChanged → IView 更新
```

所有 View 都直接继承统一 `IView`，公开 `values()`、`applyFields()`、`clearSensitiveFields()`、`clearInputs()`、`showStatus()` 和 `events()`。FormBinding 是 View 内部共用的 Qt 控件适配器，负责信号接线、字段隔离和阻断程序更新产生的反馈，不承载业务。

各 View 为 Designer 中的按钮、动作、开关、下拉框、数值和滑条公开 `<objectName>Requested(const QVariantMap &fields)` 信号。例如 ConnectionView 的 `pbtn_OpenSerialRequested`。同时通过 `IView::events()` 提供统一的 `actionTriggered(panel, objectName)` 信号，功能 Presenter 使用该通道收集当前表单参数并转发请求。单个表单也可脱离 WorkbenchView 使用同一个 IView 接口。

每个表单拥有独立 Presenter 实例和待处理请求集合；同一功能的多个表单复用对应 Presenter 类。主 WorkbenchPresenter 只装配和绑定；导航、模型文件选择、账户输入清理等 UI 行为分别位于所属功能 Presenter。Presenter 不包含生成的 ui_*.h、QWidget 或业务 Control。

Backend 也按上述七个功能划分。FeatureBackend 检查请求 panel 是否属于其功能；WorkbenchBackend 只管理功能到 Backend 的注册关系。当前各功能 Backend 均为扩展入口，收到业务请求会明确返回“业务适配尚未实现”，不执行旧版业务或伪造成功状态。

## 应用层接入

独立入口 `apps/FOSD-Classic-QWidgets/main.cpp` 组合 View、功能 Backend 注册表和 Presenter；主程序 `apps/FOSD-Win` 复用同一界面。未来在应用适配层继承对应功能 Backend 并重写 `handleRequest()`，或实现 IBackend，再注入指定功能：

```cpp
using namespace fosd::ui::classic;
WorkbenchView view;
WorkbenchBackend backends;
WorkbenchPresenter presenter(&view);
presenter.setBackends(&backends);
backends.setBackend(FeatureId::Connection, connectionAdapter);
// 主程序也提供 window.setUiBackend(FeatureId::Connection, connectionAdapter)。
```

注册表拥有其默认创建的占位 Backend；外部注入适配器的生命周期由应用层管理。注册表、View、Presenter 在 GUI 线程使用；适配器可运行在其他具有事件循环的线程，通过 queued 连接收发 UiRequest / UiResponse。适配器的 supports(panel) 必须覆盖其功能的表单。

请求包含唯一 `id`、`panel`、原 Designer `action` 和当前表单 `fields`；响应保留请求 id/panel，并填写 success/message/fields。Presenter 只应用匹配待处理请求且来自当前适配器的成功响应。替换或销毁 Backend 会取消未完成请求；旧响应被忽略。业务层仍需自行停止旧适配器正在执行的任务。

密码字段仅属于其当前表单，不会混入通信或主窗口请求。账户提交完成、窗口关闭和 Backend 替换时清除敏感输入。此机制不实现账户认证或权限判断。

显示字段沿用 objectName：文本为 QString，开关为 bool，数值为数值，下拉框为 `{items, index}` 或 `{text}`，表格为 `{headers, rows}`；曲线 `cplot_Spectrum` 接收等长 `{x, y}`。连续数据显示可由 Backend 发出 presentationChanged(panel, fields)，无需伪造请求响应。

## 三维宿主

经典界面的三维控件仍使用 RenderHostWidget 保留原 `vtk_openGL` 对象名。后续应用层可通过 `view.renderHost()->setSurface(qvtkWidget)` 挂载实际 QVTK 控件。宿主不创建 VisualizationControl，也不自行持有后台结果。真正 QVTK 到全部后端的集成示例见 [VTK 验证前端](../Test-VtkOpenGLNativeWidgets/README.md)。

## 构建和验证

```powershell
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-debug
ctest --preset fosd-widgets-debug
# Release 对应 fosd-widgets-release。
$env:PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Debug/FOSD-Classic-QWidgets.exe
```

沿用现有 `fosd-widgets` 预设名和工作台构建选项；应用目标更新为 `FOSD-Classic-QWidgets`，库目标使用 `fosd_classic_*` 前缀。测试目标为 `test_classic_widgets`。Qt Core 合约、功能 Backend、功能 Presenter、Widgets 与控件分别构建，预览程序不依赖业务模块。

测试覆盖全部 19 个 IView、资源引用、控件级 signals、独立表单使用、页面和 Dock、表单输入隔离、敏感信息清理、成功/失败及重复响应、功能 Backend 范围、跨线程请求、替换后旧响应、显示字段和渲染宿主生命周期。
