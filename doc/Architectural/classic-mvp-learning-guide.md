# Classic QWidgets 三层 MVP 学习指南

本文按 2026-11-08 当前源码重写，配合 `classic-mvp-ui-backend-connection.md`（迁移审阅）阅读：那篇讲"改了什么"，这篇讲"怎么运转、怎么往上加"。

旧五层（View → Presenter → Backend 适配器 → WorkbenchServices 端口 → Runtime）已整体删除，
`UiRequest` / `UiResponse` / 请求编号 / `QueuedConnection` / `supports()` 全部不存在。
现在只有三层加一个装配根。

---

## 1. 全景

```
ClassicWorkbench（装配根）
  ├─ m_model     : WorkbenchModel        ← 业务编排，持有 3 个 Control
  ├─ m_presenter : WorkbenchPresenter    ← 创建/编排 6 个功能 Presenter
  └─ (this)      : WorkbenchView         ← 10 个面板 + FormBinding
```

```
Designer .ui（控件 objectName = 协议名）
        │ setupUi
        ▼
┌──────────────────────────────────────────────────────────────┐
│ View（widgets 目标，不链接 Model）                             │
│   MainWindowView / ConnectionView / …  : IView + FormBinding  │
│   出口唯一：ViewEvents::actionTriggered(PanelId, objectName)   │
│   入口唯一：applyFields(const QVariantMap&)                    │
└───────────────┬──────────────────────────────▲───────────────┘
                │ 直接函数调用（同线程，无队列） │ 直接函数调用
                ▼                              │
┌──────────────────────────────────────────────┴───────────────┐
│ Presenter（presenters 目标，链接 Model）                       │
│   PanelPresenter::execute() —— 四段流水线                      │
│     values() → localAction() → prepareAction() → handleAction()│
│   子类：MainWindow / Connection / Visualization / ModelPanel / │
│         HardwareData                                          │
└───────────────┬──────────────────────────────▲───────────────┘
                │ 明确的业务方法调用             │ Model 的信号
                ▼                              │ + poll()
┌──────────────────────────────────────────────┴───────────────┐
│ WorkbenchModel（model 目标，链接 src/）                        │
│   状态：WorkbenchState + revision/pending 标志                 │
│   方法：applyCalibration / openSerial / loadModel / setRecording…│
│   信号：stateChanged / dataChanged / configurationChanged /     │
│         messageChanged / frameObserved / rawDataReceived …      │
└───────────────┬──────────────────────────────────────────────┘
                ▼
   protocol::ProtocolControl / Calculation::CalculationControl /
   visualization::VisualizationControl（各自带工作线程）
```

三条硬边界：

| 边界 | 内容 |
| --- | --- |
| widgets 不链接 model | `fosd_classic_widgets` 只依赖 contracts + controls，所以 `test_classic_widgets` 不需要 VTK |
| Presenter 不碰 Control | 只调 Model 的业务方法；Model 是唯一知道 Control 存在的地方 |
| Model 不认识界面 | 不含 `PanelId`、控件名、`QVariantMap` 展示协议；只返回 typed 值和发信号 |

---

## 2. 请求方向：`PanelPresenter::execute()` 的四段流水线

所有点击都从同一个入口进来：

```cpp
// common/PanelPresenter.cpp:44
void PanelPresenter::execute(const QString &action) {
    if (action.isEmpty()) return;

    // ① 禁用态校验：程序化触发也要受 isEnabled() 约束
    if (auto *root = dynamic_cast<QWidget *>(m_view)) {
        if (!root->isEnabled()) return;
        auto *source = root->findChild<QObject *>(action);
        if (auto *w = qobject_cast<QWidget *>(source)) { if (!w->isEnabled()) return; }
        else if (auto *a = qobject_cast<QAction *>(source)) { if (!a->isEnabled()) return; }
    }

    QVariantMap fields = m_view->values();          // ② 整面板快照

    if (localAction(action, fields)) return;        // ③ 纯 UI，就地消化
    if (!m_model) { …"业务服务尚未接入"… return; }

    if (!prepareAction(action, fields)) return;     // ④ UI 前置（对话框），可取消
    QString message = tr("请求已提交");
    QVariantMap updates;
    const bool success = handleAction(action, fields, message, updates);  // ⑤ 业务
    if (!updates.isEmpty()) m_view->applyFields(updates);
    m_view->showStatus(success ? message : tr("操作失败：%1").arg(message));
    emit statusChanged(message);
    emit actionFinished(action, success, message);
}
```

四个钩子的职责划分就是"新功能该写在哪"的判据：

| 钩子 | 返回值含义 | 用它做什么 | 例子 |
| --- | --- | --- | --- |
| `localAction` | true = 已处理，流程结束 | 切页、切主题、开面板、关窗口 | `pbtn_JieTiaoYi`、`switch_Style` |
| `prepareAction` | false = 取消整个动作 | 弹文件/颜色对话框，往 `fields` 里补数据 | `pbtn_ModelLoad`、`pbtn_SaveDynamic` |
| `handleAction` | **bool 是"成功"**，不是"已处理" | 调 Model、校验、组织回填 | `pbtn_OpenSerial`、`cbox_FPS` |
| `refresh` / `poll` | — | 响应 Model 信号 / 周期取数 | 见第 4 节 |

---

## 3. 三个实例

### 3.1 纯本地：`switch_Style`（0 行 Model）

```cpp
// features/mainwindow/presenters/MainWindowPresenter.cpp:19
if (action == "switch_Style") {
    m_host->applyTheme(fields.value("switch_Style").toBool());
    return true;                       // 到此结束
}
```
`m_host` 是 `IWorkbenchView`，只暴露界面能力（`selectPage` / `showPanel` / `closePanel` /
`windowAction` / `applyTheme` / `setModelAvailable` / `chooseFile`）。
主题和导航不是业务，硬塞给 Model 只会多一份无意义耦合。

### 3.2 真正到后端：`pbtn_OpenSerial`

```
用户点"打开串口"
  → FormBinding::relay            action = "pbtn_OpenSerial"
  → ViewEvents::actionTriggered(PanelId::Connection, "pbtn_OpenSerial")
  → PanelPresenter::execute
      fields = m_view->values()            // 21 个控件的最新值
      localAction → false
      prepareAction → true（不是保存类动作）
  → ConnectionPresenter::handleAction
      L244: 从 fields 逐个取端口/波特率/数据位/校验/停止位/流控
            组装 communication::SerialConfig，任一非法 → success=false + message
            success → m_model->openSerial(configuration)
  → WorkbenchModel::openSerial
      m_protocol->communication().openSerial(c)        // 交给通信 Control
```

关键点：**Presenter 负责"把界面字符串翻译成 typed 配置"**，Model 只收 `SerialConfig`。
串口打开本身是异步的，`openSerial()` 立刻返回，成败由信号回来：

```
ConnectControl::stateChanged
  → WorkbenchModel::transportState()      // 记录 m_states[channel]
      emit stateChanged();
  → PanelPresenter::setModel 时连好的 stateChanged → refresh()
  → ConnectionPresenter::refresh()        // features/connection/presenters/ConnectionPresenter.cpp:126
      present(PanelId::Connection, { {"pbtn_OpenSerial", closed ? "打开" : "关闭"}, … });
  → ConnectionView::applyFields → FormBinding::applyFields → 按钮文字翻转
```

注意这条回程**没有出现在任何请求里**——它不依赖是谁点的按钮。这就是 MVP 里
"View 不参与流程控制，Presenter 不知道下一次状态从哪来"的收益。

### 3.3 需要 UI 前置 + 异步确认：`pbtn_SaveDynamic`

一个 checkable 按钮，开始/停止共用。

```cpp
// features/connection/presenters/ConnectionPresenter.cpp:57
bool ConnectionPresenter::prepareAction(const QString &action, QVariantMap &fields) {
    if (action != "pbtn_SaveDynamic") return true;
    if (!fields.value("pbtn_SaveDynamic").toBool()) return true;   // 停止，不需要路径
    const auto path = m_host->chooseFile(true, tr("选择存储文件"), "CSV (*.csv);;SQLite (*.sqlite *.db)");
    if (path.isEmpty()) {
        m_view->applyFields({{"pbtn_SaveDynamic", false}});         // 撤销用户刚才的按下
        return false;                                               // 取消动作
    }
    fields["filePath"] = path;                                      // 注入后端需要的字段
    return true;
}
```

```cpp
// 同文件 :445
success = m_model->setRecording(fields.value("pbtn_SaveDynamic").toBool(),
                                text(fields, "filePath"), &message);
const auto saving = m_model->state().recording;                     // Starting/Recording/Stopping/Stopped
updates["pbtn_SaveDynamic"] = saving != RecordingState::Stopped;
updates["pbtn_SaveDynamic.enabled"] =
    saving != RecordingState::Starting && saving != RecordingState::Stopping;
```

`setRecording()` 只做三件事：登记 `m_storagePending`、调 `storage().start()`、返回 true。
真正的结果由存储模块的信号回推：

```cpp
// model/WorkbenchModel.cpp:346
void WorkbenchModel::storageStarted(const QString &session) {
    m_storagePending = false;  m_recording = true;
    emit stateChanged();                       // → refresh() → 按钮置为"停止保存"且可点
    notice(tr("存储已启动：%1").arg(session));
}
```

这条链演示了异步业务的通用套路：**请求只登记 + 校验，立即回执；完成由 Model 的信号驱动；
界面在 `refresh()` 里按 `WorkbenchState` 重新推导控件状态，不自己记账。**

### 3.4 失败纠偏：`cbox_CommuteChoose`

用户把下拉框拨到 TCP，但连接没关：

```cpp
// features/connection/presenters/ConnectionPresenter.cpp:224
success = false;
message = tr("请先关闭网络连接再切换方式");
updates["cbox_CommuteChoose"] =
    QVariantMap{{"index", m_networkChannel == Channel::Udp ? 0 : …}};   // 真实值
```

`updates` 在 `execute()` 里**无论成功失败都会写回**（`if (!updates.isEmpty()) applyFields`），
所以下拉框会被拨回真实状态。**界面不能比后端更乐观**，靠的就是这条。

---

## 4. 回程：三条路

| 通路 | 触发者 | 语义 | 典型内容 |
| --- | --- | --- | --- |
| **`updates` 出参** | 本次动作 | 一次性纠偏，只影响发起者 | 失败回滚、联动复选框、按钮禁用 |
| **`refresh()`（信号驱动）** | `stateChanged` / `dataChanged` / `configurationChanged` | 状态快照，谁需要谁订阅 | 按钮文案与可用性、标定回填、标量设置 |
| **`poll()`（定时拉取）** | `WorkbenchPresenter` 100 ms 定时器 | 高频数据增量 | 曲线、波长表、硬件表、收发日志 |

```cpp
// application/WorkbenchPresenter.cpp:56
m_refreshTimer.setInterval(100);
connect(&m_refreshTimer, &QTimer::timeout, this, &WorkbenchPresenter::refreshDisplays);

void WorkbenchPresenter::refreshDisplays() {
    for (auto *p : m_presenters) p->poll();      // 10 个 presenter，各自判断是否该干活
}
```

`poll()` 必须便宜，进门先做守卫：

```cpp
// features/connection/presenters/ConnectionPresenter.cpp:518
void ConnectionPresenter::poll() {
    if (!m_model || m_panel != PanelId::Connection) return;
    const auto frame = m_model->frame();
    if (m_frame && m_presentedFrame != m_frame->revision) { … }   // revision 去重
}
```

判定"该走哪条路"：

```
这次动作的结果只跟这一次有关，而且只影响当前面板？      → updates
这个状态别的面板也要看，或者异步完成后才定？            → Model 发信号 → refresh()
是周期性数据、必须按帧刷新？                            → poll()
```

---

## 5. 字段协议：`values()` / `applyFields()`

这是 View ↔ Presenter 之间唯一的约定，**全部是 `QVariantMap`**。

### 5.1 读：`values()` 产出的形状

| 控件 | 键 | 值 |
| --- | --- | --- |
| `QLineEdit` / `QPlainTextEdit` / `QTextEdit`（非只读） | objectName | `QString` |
| `QComboBox` | objectName | `{index:int, text:QString, data:QVariant}` |
| 可 check 的 `QAbstractButton` | objectName | `bool` |
| `QSpinBox` / `QDoubleSpinBox` / `QAbstractSlider` | objectName | 数值 |
| `SwitchControl` | objectName | `bool` |
| `QTableView` 选中行 | `objectName + ".selectedRows"` | `QVariantList<int>` |

两个静态工具函数统一取值（`PanelPresenter.h:41`）：

```cpp
text(fields, "cbox_FPS")    // 下拉框取 text，文本框直接取字符串
index(fields, "cbox_FPS")   // 下拉框取 index，-1 表示没有
```

### 5.2 写：`applyFields()` 支持的形状

| 目标 | 写法 |
| --- | --- |
| `QLineEdit` / `QLabel` / `QTextEdit`（按钮则改文字） | `QString` |
| 可 check 的按钮 / `QAction` / `SwitchControl` | `bool` |
| `QSpinBox` / `QDoubleSpinBox` / `QAbstractSlider` | 数值 |
| `QComboBox` | `{items:[…], data:[…], index:n}` / `{text:"…"}` / `{dataValue:…}` |
| `QTableView` / `QTableWidget` | `{headers:[…], rows:[[…],…]}` |
| 任意 `QWidget` / `QAction` | 键名加 `.enabled` 后缀 + `bool` |
| 面板状态 | `"_status"` → `showStatus()` |

实现细节（`common/FormBinding.cpp`）：

- 查找靠 `findChildren<QObject*>(objectName)`，**objectName 就是协议名**，同一面板内必须唯一。
- 写回时 `QSignalBlocker` 屏蔽信号（`:143` 附近），否则 `setText → editingFinished → relay` 会自激。
- `owns()` + `fosdPanelRoot` 属性保证父表单不会把子面板的值一起收走。

### 5.3 View 可以局部"接管"某些键

通用绑定撑不住领域渲染时，由该功能的 View 自己接管：

```cpp
// features/connection/views/connectwindow.cpp:43
void ConnectionView::applyFields(const QVariantMap &fields) {
    auto ordinary = fields;
    if (fields.contains("cplot_Spectrum"))
        { renderSpectrum(m_ui->cplot_Spectrum, fields.value("cplot_Spectrum").toMap(), this);
          ordinary.remove("cplot_Spectrum"); }
    for (auto *log : {m_ui->ptedit_SerialRec, m_ui->ptedit_UdpRec, m_ui->ptedit_TcpRec})
        if (…contains("entries")…) { appendTrafficEntries(log, …); ordinary.remove(log->objectName()); }
    m_binding->applyFields(ordinary);      // 剩下的交给通用绑定
}
```

反方向同理——View 可以**补充**快照：

```cpp
// features/visualization/views/keypoints/disp3dkeypointselect.cpp:23
QVariantMap KeyPointSelectView::values() const {
    auto fields = m_binding->values();
    QVariantList ids;                       // 表格里选中的业务 ID（不是行号）
    for (const auto &row : table->selectionModel()->selectedRows())
        if (const auto *item = table->item(row.row(), 0)) ids.append(item->text().toLongLong());
    fields["tablew_PointPara.selectedIds"] = ids;    // 点击瞬间快照，避免坐标刷新导致误删
    return fields;
}
```

---

## 6. 新增控件的统一方法

### 6.1 先分类

```
操作只影响界面（切页/开面板/主题/窗口/取色）
    → Presenter::localAction 里 return true

需要弹对话框补数据（文件路径、颜色）
    → Presenter::prepareAction 里填 fields，返回 false 可取消

是一次性业务动作（点一下改后端）
    → Presenter::handleAction 加分支 +（必要时）Model 加方法

是"改一下就生效"的设置
    → handleAction 加分支，成功后用 updates 或 refresh 回填

只显示数据
    → 不写 Presenter；在 refresh() 或 poll() 里 present()

周期性数据
    → poll() 里加分支，务必用 revision / 已推送 id 去重
```

### 6.2 五步清单

1. **`.ui`**：起稳定 objectName（沿用 `pbtn_` / `cbox_` / `ckbox_` / `edit_` / `ptedit_` /
   `hsd_` / `spin_` / `lab_` / `action_` 前缀），面板内唯一，不以 `qt_` 开头。
2. **View：一般 0 行**。只有三种情况要动：`QLineEdit` 需要"输入即提交"（默认只有
   `le_MapRange*` 和 `edit_SerialCommand*` 接了 `editingFinished`）；写回需要界面副作用；
   新自研控件类型（要改 `FormBinding` 三处）。
3. **Presenter**：在对应 `handleAction` 里加分支。三条纪律：
   - 返回值是**成功与否**，不是"处理了没有"；
   - 失败一定要给 `message`，否则用户看到默认的"请求已提交"；
   - 需要纠偏就写 `updates`，不要背后偷偷 `applyFields`。
4. **Model**：只有满足以下之一才动 —— 需要跨面板共享状态、需要异步确认、需要持久化、
   需要 revision/pending 校验。纯展示换算不必进 Model。
5. **CMake + 测试**：View 进 `fosd_classic_widgets`；Presenter 进 `fosd_classic_presenters`；
   Model 新增文件进 `fosd_classic_model`。控件存在性测 `WorkbenchTests.cpp`；
   行为测 `workbench_model_tests.cpp`；端到端测 `workbench_main_tests.cpp`（点真实控件）。

### 6.3 模板 A：一次性设置（照抄 `hsd_GridSize`）

```cpp
// features/visualization/presenters/VisualizationPresenter.cpp:112
} else if (a == "hsd_GridSize") {
    auto settings = view.gridSettings();                 // ① 读现有整体设置
    const double scale = f.value(a).toDouble() / 20.0;   // ② 控件值 → 业务量
    settings.halfExtentMm = qMax(1.0, 3000.0 * scale);
    settings.divisions = qBound(1, qRound(100.0 * scale), 500);
    success = view.setGridSettings(settings);            // ③ 用有返回值的接口，把结果写进 success
}
```

### 6.4 模板 B：联动回填（照抄 `cbox_GridAble`）

```cpp
} else if (a == "cbox_GridAble") {
    const bool visible = f.value(a).toBool();
    view.showGrid(visible);
    updates = {{"cbox_XOZGrid", visible}, {"cbox_XOYGrid", visible}, {"cbox_ZOYGrid", visible}};
}
```

### 6.5 模板 C：异步确认（照抄 `pbtn_CalculationStart`）

```cpp
} else if (action == "pbtn_CalculationStart") {
    if (!m_model->setCalculationRunning(true, &message)) return false;   // 提交失败
    updates["pbtn_CalculationStart.enabled"] = !m_model->state().solving;
    updates["pbtn_CalculationStop.enabled"]  =  m_model->state().solving;
    m_model->notice(message);        // 模型级提示，走主状态栏
}
```

Model 侧对应 `applyCalibration()` 的写法：**先置 pending 并拒绝重复提交，确认信号到了才替换有效值**
（`WorkbenchModel.cpp:264`），这样"已提交"不会被误当成"已生效"。

### 6.6 新增一个面板

1. `PanelId` 加枚举值（`common/UiTypes.h`）。
2. `application/WorkbenchView.cpp` 的 `createPanelView()` 加 `case`（原 `common/ViewFactory.cpp` 已并入组装根）。
3. 新 View 照抄 30 行样板：`initializeFosdUiResources()` → `setupUi` → `new FormBinding(PanelId::Xxx, this, this)`。
4. `WorkbenchView` 决定它进 `QStackedWidget` / `QDockWidget` / 独立窗口。
5. `WorkbenchPresenter` 构造函数里 `case` 到某个 Presenter（可复用现有 Presenter 管多个面板，
   例如 `ModelPanelPresenter` 同时管 ModelPanel / KeyPointEdit / KeyPointSelect）。

---

## 7. 常见坑

| # | 坑 | 正确做法 |
| --- | --- | --- |
| 1 | `handleAction` 返回 false 被当成"失败" | 基类默认返回 false 且不改 message → 状态栏出现"操作失败：请求已提交"。新增动作必须显式 `return true` |
| 2 | 以为 `fields` 只有变化项 | 它是**全面板快照**，只读你关心的键 |
| 3 | 在 `poll()` 里做重活 | 100 ms × 10 个 presenter。先用 revision / 已推送 id 早退 |
| 4 | 用 `presentOwn` 跨面板推送 | `presentOwn` 带 `p == m_panel` 过滤，别的实例调用会被静默丢弃；跨面板用 `present(panel, fields)` |
| 5 | 觉得改 `QLineEdit` 就会发请求 | 默认不发，只在别的按钮提交时被 `values()` 带走 |
| 6 | 期待 `setCurrentIndex` 触发下拉框动作 | 连的是 `activated`（仅用户操作），且写回还有 `QSignalBlocker` |
| 7 | 在 Presenter 里直接调 Control | 会绕过 revision/pending 校验和线程边界；一律走 Model |
| 8 | 把 `QWidget*` 塞给 Model | Model 只收 typed 值 |
| 9 | 改了 objectName | 后端、测试、INI 键、`startsWith("ckbox_CH")` 前缀匹配全靠它，改名要全仓 grep |
| 10 | 用返回值 void 的 Control 接口还报成功 | `setfreshFPS` / `setDeformationScale` / `showAxes` / `setCameraLocked` / `setCameraPreset` 目前都是 void，失败也会显示"设置已应用" |

---

## 8. 建议阅读顺序

1. `common/UiTypes.h` + `common/IView.h` + `common/ViewEvents.h`（65 行）
2. `common/PanelPresenter.cpp:44`（`execute` 四段流水线，35 行）
3. `common/FormBinding.cpp` 的 `values()` 与 `applyFields()` —— 协议全在这
4. `features/data/presenters/HardwareDataPresenter.cpp`（38 行，最短的完整闭环）
5. `features/mainwindow/presenters/MainWindowPresenter.cpp`（28 行，纯本地方向）
6. `model/WorkbenchModel.h` 的公开方法区 + `applyCalibration()` / `storageStarted()`
7. `application/WorkbenchPresenter.cpp`（80 行，装配与 100 ms poll）
8. `application/ClassicWorkbench.cpp`（26 行，装配顺序与 VTK 挂载）
9. `tests/workbench_model_tests.cpp` + `tests/workbench_main_tests.cpp`（看别人怎么驱动这条链）

读完 1–8 约 600 行，就能回答"一次点击怎么变成一次业务调用、业务状态怎么回到界面"。
