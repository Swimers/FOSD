# Classic QWidgets 的 MVP 前后端连接指南

本文面向"已经重构完成、需要看懂界面与后端怎么接"的读者。所有结论都来自当前源码，路径以 `ui/FOSD-Classic-QWidgets/` 为根。

> 路径更正：主窗口表单实际在 `ui/FOSD-Classic-QWidgets/features/mainwindow/`，不在 `apps/mainwindow/`。
> 仓库根目录的 `apps/FOSD-Classic-QWidgets/main.cpp` 只有 15 行，作用仅是构造 `ClassicWorkbench` 并 `show()`；
> 真正的装配发生在 `application/ClassicWorkbench.cpp`。

---

## 1. 全景：五层，一条单行道 + 两条回程

```
                        ┌──────────────────────────────┐
                        │ Designer 表单 oldmainwindow.ui │  控件名 = 协议名
                        └──────────────┬───────────────┘
                                       │ setupUi(this)
                                       ▼
┌───────────────────────────────────────────────────────────────────┐
│ View 层（widgets）                                                 │
│   MainWindowView : QMainWindow + IView + FormBinding(PanelId)      │
│   职责：显示、采集输入、把"用户意图"变成一次 actionTriggered       │
└───────────────┬───────────────────────────────────▲───────────────┘
                │ ViewEvents::actionTriggered(panel, │ applyFields(QVariantMap)
                │                            action) │
                ▼                                    │
┌───────────────────────────────────────────────────┴───────────────┐
│ Presenter 层（presenters）                                         │
│   PanelPresenter（请求生命周期）+ MainWindowPresenter（本面板策略） │
│   职责：拼 UiRequest、本地动作、请求/回执配对、把结果写回 View      │
└───────────────┬───────────────────────────────────▲───────────────┘
                │ requestReady(UiRequest)            │ completed(UiResponse)
                │ Qt::QueuedConnection               │ presentationChanged(panel, fields)
                ▼                                    │ Qt::QueuedConnection
┌───────────────────────────────────────────────────┴───────────────┐
│ Backend 适配层（backends）                                         │
│   IBackend 契约 + FeatureBackend 基类 + MainWindowBackend 等 5 个   │
│   职责：按 FeatureId 注册、线程边界、把请求转交给服务端口           │
└───────────────┬───────────────────────────────────▲───────────────┘
                │ dispatch() → handleRequest()       │ emit presentationChanged
                ▼                                    │
┌───────────────────────────────────────────────────┴───────────────┐
│ 服务端口 / 应用层                                                  │
│   WorkbenchServices（纯虚端口）→ WorkbenchRuntime（唯一实现）        │
│   职责：真业务。connectionAction / visualizationAction /           │
│         dataAction / markerAction 四个分派 + 100 ms 推送定时器      │
└───────────────┬───────────────────────────────────────────────────┘
                ▼
┌───────────────────────────────────────────────────────────────────┐
│ 业务 Control 层（src/，与界面无关，可跨线程）                        │
│   ProtocolControl / CalculationControl / VisualizationControl      │
└───────────────────────────────────────────────────────────────────┘
```

三个关键约束，代码里都有注释兜底：

| 约束 | 证据 |
| --- | --- |
| 只有值对象跨界，Widget 与 Control 指针永不越界 | `common/IBackend.h`："Values cross threads; widgets and Controls do not." |
| 跨线程只靠 Qt 信号槽，且显式 `QueuedConnection` | `common/PanelPresenter.cpp` 的 `setBackend()` |
| 依赖方向单向：widgets → presenters → backends → contracts | `CMakeLists.txt` 各静态库的 `target_link_libraries` |

---

## 2. 契约层：四个类型、三个信号

契约全部集中在 `common/`，只有 200 行左右，值得先背下来。

### 2.1 数据类型（`common/UiTypes.h`）

```cpp
enum class PanelId { Main, Connection, ChannelCalibration, Visualization,
                     ModelPanel, ScenePanel, InfoPanel, KeyPointEdit,
                     KeyPointSelect, HardwareData };
enum class FeatureId { MainWindow, Connection, Visualization, Model, Tracking };
FeatureId featureForPanel(PanelId);          // 面板 → 功能 的路由表

struct UiRequest  { quint64 id; PanelId panel; QString action; QVariantMap fields; };
struct UiResponse { quint64 id; PanelId panel; bool success; QString message;
                    QVariantMap fields; };
Q_DECLARE_METATYPE(UiRequest);               // 能进队列，才能跨线程
Q_DECLARE_METATYPE(UiResponse);
```

- `action` 就是 Designer 里的 **objectName**，没有第二套命令枚举。
- `fields` 是**整个面板的值快照**，不是单控件增量。
- `id` 是请求/回执配对键，由 `std::atomic<quint64>` 自增生成。

### 2.2 View 侧的发射器（`common/ViewEvents.h`）

```cpp
class ViewEvents : public QObject {
    Q_OBJECT
signals:
    void actionTriggered(PanelId panel, const QString &action);
};
```

只有一个信号。`FormBinding` 是它的唯一实现，`IView::events()` 把它交出去。

### 2.3 Backend 侧的契约（`common/IBackend.h`）

```cpp
class IBackend : public QObject {
public:
    virtual bool supports(PanelId) const = 0;
public slots:
    virtual void execute(const UiRequest &) = 0;
signals:
    void completed(const UiResponse &response);                              // 有问必答
    void presentationChanged(PanelId panel, const QVariantMap &fields);      // 主动推送
};
```

两个回程信号的分工是整套架构的核心，见第 5 节。

### 2.4 Presenter 侧的骨架（`common/PanelPresenter.h`）

```cpp
class PanelPresenter : public QObject {
public slots:
    void execute(const QString &action);
    void complete(const UiResponse &response);
signals:
    void requestReady(const UiRequest &request);
    void statusChanged(const QString &message);
protected:
    virtual bool localAction(const UiRequest &);   // 纯 UI 的意图，就地消化
    virtual bool prepareRequest(UiRequest &);      // 发后端前补字段（如文件路径）
    virtual bool isSensitiveAction(const QString &) const;
};
```

三个虚函数就是"新功能该写在哪"的分界线：

| 动作性质 | 写在哪 | 例子 |
| --- | --- | --- |
| 纯界面（导航、主题、窗口、弹窗） | `localAction()` | `pbtn_JieTiaoYi`、`switch_Style` |
| 发后端前要补 UI 信息 | `prepareRequest()` | 保存前弹文件对话框填 `filePath` |
| 真业务 | Backend → Services | 打开串口、置零、载入模型 |

---

## 3. 请求方向：一次点击的完整旅程

以 **主窗口的 `switch_Style`（深浅主题开关）** 为例，这是最短也最典型的一条链。

### 3.1 控件 → FormBinding

`SwitchControl` 是自研控件（`controls/switchcontrol.h`），既不是 `QAbstractButton` 也不是 `QCheckBox`，所以 `FormBinding` 为它写了专门的一条连接：

```cpp
// common/FormBinding.cpp:57
for (auto *toggle : widget->findChildren<SwitchControl *>())
    connect(toggle, &SwitchControl::toggled, this, &FormBinding::relay);
```

鼠标释放后 `SwitchControl::mouseReleaseEvent` 取反并 `emit toggled(m_bChecked)`。`relay()` 只做两件事：

```cpp
// common/FormBinding.cpp:79
void FormBinding::relay() {
    if (sender() && named(sender()) && owns(sender()))
        emit actionTriggered(m_panel, sender()->objectName());
}
```

- `named()`：objectName 非空且不以 `qt_` 开头 —— **控件名就是协议名**。
- `owns()`：沿 parent 链上溯，遇到别的 `fosdPanelRoot` 就返回 false —— **面板之间不串味**（见第 8.2 节）。

### 3.2 FormBinding → Presenter

`PanelPresenter` 构造时就把自己挂到 `view->events()` 上：

```cpp
// common/PanelPresenter.cpp:18
connect(view->events(), &ViewEvents::actionTriggered, this,
        &PanelPresenter::actionTriggered);
```

于是 `execute("switch_Style")` 开始跑。`execute()` 依次做五件事：

```cpp
// common/PanelPresenter.cpp:53（节选，已按源码还原为 C++11 写法）
// ① 禁用态校验：程序化触发也要受 isEnabled() 约束
if (!root->isEnabled()) return;
auto *source = root->findChild<QObject *>(action);
if (auto *widget = qobject_cast<QWidget *>(source)) {
    if (!widget->isEnabled()) return;
} else if (auto *command = qobject_cast<QAction *>(source)) {
    if (!command->isEnabled()) return;
}

// ② 组请求
UiRequest request;
request.id = ++nextRequest;            // 原子自增
request.panel = m_panel;
request.action = action;
request.fields = m_view->values();      // 全面板快照

// ③ 纯 UI 动作就地消化，不进 pending，不发后端
if (localAction(request)) return;

// ④ 登记在途请求
m_pending.insert(request.id, action);

// ⑤ 没有适配器 → 就地构造失败回执；有 → 让 Presenter 补字段后发车
if (!m_backend || !m_backend->supports(m_panel)) { ... complete(response); return; }
if (!prepareRequest(request)) { m_pending.remove(request.id); return; }
emit requestReady(request);
```

`m_host` 是 `IWorkbenchView*`，只暴露"界面能做的事"：

```cpp
// application/IWorkbenchView.h
virtual void selectPage(int index) = 0;      // 切页
virtual void showPanel(PanelId) = 0;         // 弹独立面板
virtual void closePanel(PanelId) = 0;
virtual void windowAction(PanelId, WindowAction) = 0;
virtual void applyTheme(bool dark) = 0;
virtual QString chooseFile(bool save, const QString &title, const QString &filter) = 0;
```

注意 `chooseFile()` —— 文件对话框属于"界面能力"，所以由 Presenter 通过这个口子调用，而不是让 Backend 去弹窗。

### 3.3 主窗口这一跳：`MainWindowPresenter::localAction`

```cpp
// features/mainwindow/presenters/MainWindowPresenter.cpp:12
bool MainWindowPresenter::localAction(const UiRequest &request) {
    const auto &action = request.action;
    if (m_host) {
        const QStringList navigation{"pbtn_JieTiaoYi", "pbtn_3DPlot"};
        if (navigation.contains(action)) {
            m_host->selectPage(navigation.indexOf(action));   // 0=通信页, 1=变形场页
            return true;                                      // 到此为止
        }
        if (action == "switch_Style") {
            m_host->applyTheme(request.fields.value("switch_Style").toBool());
            return true;
        }
    }
    return PanelPresenter::localAction(request);   // 最小化/全屏/关闭
}
```

`applyTheme()` 落到 `WorkbenchView`：

```cpp
// application/WorkbenchView.cpp:171
void WorkbenchView::applyTheme(bool dark) {
    QFile style(dark ? ":/fosd-ui/styles/dark.qss" : ":/fosd-ui/styles/light.qss");
    if (style.open(QIODevice::ReadOnly))
        setStyleSheet(QString::fromUtf8(style.readAll()));
    if (auto *connection = qobject_cast<ConnectionView *>(panelWidget(PanelId::Connection)))
        connection->setSpectrumTheme(dark);      // 自绘控件要自己重绘
}
```

到这里请求结束。**主窗口的这两个动作一次都没有碰到 Backend**，这是有意的：导航和主题是界面职责，交给业务层只会增加一份无意义的耦合。

### 3.4 真正跨到后端的路径

当 `localAction()` 返回 false 时，请求沿下面这条链继续走：

```
PanelPresenter::requestReady            （GUI 线程，emit）
        │  Qt::QueuedConnection
        ▼
IBackend::execute                       （适配器所在线程）
        │
        ├─ FeatureBackend::execute  → supports()? 否 → 直接 completed(失败)
        ▼ 是
handleRequest(request)                  （各 FeatureBackend 重写，当前都是同一句）
        │
        └─ dispatch(request)
                │
                ├─ m_services 为空 → return false → reportUnavailable()
                ▼
WorkbenchServices::handleRequest(request, &response)   ← 唯一实现是 WorkbenchRuntime
```

`dispatch()` 非常短，但它承担了"业务层不认这个动作"的兜底：

```cpp
// common/FeatureBackend.cpp:48
bool FeatureBackend::dispatch(const UiRequest &request) {
    if (!m_services) return false;
    UiResponse response;
    response.id = request.id;
    response.panel = request.panel;
    if (!m_services->handleRequest(request, &response)) return false;  // 没人认领
    response.id = request.id;
    response.panel = request.panel;
    emit completed(response);
    return true;
}
```

`WorkbenchRuntime::handleRequest()` 是四个分派器的短路或：

```cpp
// application/runtime/WorkbenchRuntime.cpp:117
const bool handled = connectionAction(request, response) ||
                     visualizationAction(request, response) ||
                     dataAction(request, response);
if (handled && !response->success && !response->fields.isEmpty())
    emit presentationChanged(request.panel, response->fields);   // 失败也要纠偏，见 5.3
return handled;
```

`markerAction()` 由 `visualizationAction()` 在末尾再转一次（关键点面板）。

---

## 4. 为什么主窗口的 Backend 看起来是空的

`features/mainwindow/backends/MainWindowBackend.cpp` 全文 14 行：

```cpp
MainWindowBackend::MainWindowBackend(QObject *parent) : FeatureBackend({PanelId::Main}, parent) {}

void MainWindowBackend::handleRequest(const UiRequest &request) {
    if (!dispatch(request))
        reportUnavailable(request);
}
```

这不是"漏写"，而是三件事共同决定的：

1. **主面板的现存动作全是界面动作**，已经在 `MainWindowPresenter::localAction` 里 `return true` 消化掉了，请求根本到不了这里。
2. **它仍然是必需的扩展点**。`WorkbenchPresenter` 为每个面板创建 Presenter，`WorkbenchPresenter::refreshBackend()` 按 `featureForPanel()` 找适配器。如果这里没有适配器，`PanelPresenter::execute()` 会走"该功能的后端适配器尚未接入"分支 —— 语义是"根本没接线"。留一个空适配器，语义变成"接了线，但这个动作还没实现"，用户看到的是 `pbtn_XXX 的业务适配尚未实现`。两种提示对排查的价值完全不同。
3. **将来加主窗口级业务（参数中心、工程会话、关于页版本信息）时，只需要在 `MainWindowBackend` 里写分派**，Presenter 侧一行都不用改。

`reportUnavailable()` 的文案也说明了这个定位：

```cpp
// common/FeatureBackend.cpp:25
response.message = tr("%1 的业务适配尚未实现").arg(request.action);
```

**注意一个连带效果**：`FormBinding` 会自动连接所有命名的 `QAbstractButton` / `QComboBox` / `QAbstractSpinBox` / `QSlider`，所以任何"自动挂了信号但没人处理"的控件，点一下就会弹这条提示。处理办法只有两种 —— 要么 Presenter 用 `localAction` 吞掉（返回 true），要么 Backend 给一个明确的软提示分支。现有代码里两种都能找到：`ckbox_CH*` 用前者，`cbox_SerialBaudChoose` 用后者（"参数将在下次打开通道时生效"）。

---

## 5. 响应方向：三条回写通道

`applyFields(QVariantMap)` 是唯一的写回入口，但它有三个来源，语义完全不同。

### 5.1 通道一：请求—回执（`completed`）

```cpp
// common/PanelPresenter.cpp:93
void PanelPresenter::receiveResponse(const UiResponse &response) {
    if (sender() == m_backend.data())     // 换过适配器，旧回执直接丢
        complete(response);
}

void PanelPresenter::complete(const UiResponse &response) {
    if (response.panel != m_panel || !m_pending.contains(response.id))
        return;                            // 不是本面板 / 不是本次在途请求 → 丢弃
    const auto action = m_pending.take(response.id);
    if (response.success)
        m_view->applyFields(response.fields);   // 只有成功才应用
    if (isSensitiveAction(action))
        m_view->clearSensitiveFields();
    const auto message = ...;
    m_view->showStatus(response.success ? message : tr("操作失败：%1").arg(message));
    emit statusChanged(message);
}
```

三个细节值得记住：

- **成功才应用字段**。失败时 `response.fields` 不会被 `complete()` 写回 —— 所以"失败回填"必须走通道三。
- **`m_pending` 是许可名单**。异步环境下迟到的、跨面板的回执会被静默丢弃，不会把别的面板的值刷到当前界面。
- **状态文本走两条路**：`m_view->showStatus()` 是就地提示，`statusChanged` 上抛给 `WorkbenchPresenter::showStatus()`，最终写到主窗口状态栏。

### 5.2 通道二：主动推送（`presentationChanged`）

这是数据"自己"流向界面的路，不依赖任何请求：

```cpp
// common/FeatureBackend.cpp:40
connect(services, &WorkbenchServices::presentationChanged, this, &FeatureBackend::publish);

void FeatureBackend::publish(PanelId panel, const QVariantMap &fields) {
    if (supports(panel))                  // 只转发自己认领的面板
        emit presentationChanged(panel, fields);
}
```

```cpp
// common/PanelPresenter.cpp:113
void PanelPresenter::present(PanelId panel, const QVariantMap &fields) {
    if (sender() == m_backend.data() && panel == m_panel)
        m_view->applyFields(fields);      // 不做 success 判定，来了就刷
}
```

典型来源：

| 场景 | 代码 |
| --- | --- |
| 启动初始化，全量回填 10 个面板 | `WorkbenchRuntime::initialize()`，一次性发十几条 |
| 100 ms 周期数据（曲线、波长表、硬件表、最大位移） | `m_timer` → `publishLatest()` |
| 异步操作的中途状态（保存中/已启动/已停止） | `storageStarted/stopped/Error` |
| 后台错误统一冒泡到主状态栏 | `notice()` → `{ "_status", msg }` |
| 3D 选点变化，同时刷新 Visualization 与 KeyPointEdit 两个面板 | `selectionChanged()` |

注意 `notice()` 去重了重复文案，也说明这条通道是**幂等推送**：同样内容重复到达不产生副作用。

### 5.3 通道三：Backend 主动"纠偏"（失败回填）

`WorkbenchRuntime::handleRequest()` 里那三行是关键：

```cpp
if (handled && !response->success && !response->fields.isEmpty())
    emit presentationChanged(request.panel, response->fields);
```

因为 `complete()` 只在成功时应用字段，失败时的回填就得借用推送通道。真实例子（`WorkbenchConnection.cpp:253`，切换通信方式时连接没关）：

```cpp
if (m_states[1] != ConnectionState::Closed || m_states[2] != ConnectionState::Closed) {
    response->success = false;
    response->message = tr("请先关闭网络连接再切换方式");
    response->fields["cbox_CommuteChoose"] =
        QVariantMap{{"index", m_networkChannel == Channel::Udp ? 0 : ...}};   // 真实值
}
```

用户把下拉框拨到 TCP，界面先按用户意愿显示了 TCP，后端拒绝后把下拉框**拨回真实状态**，同时弹"请先关闭网络连接再切换方式"。这就是"界面不能比后端更乐观"的实现方式。

> 自定义 Backend 的契约：如果你自己实现 `IBackend` 而不继承 `FeatureBackend` + `WorkbenchRuntime`，失败回填就要自己 `emit presentationChanged()`，框架不会代劳。

### 5.4 通道四（准）：Presenter 直接写 View

`localAction()` / `prepareRequest()` 里也可以直接 `m_view->applyFields(...)`，用于**纯界面状态**：

```cpp
// features/connection/presenters/ConnectionPresenter.cpp:31（全选 16 个通道）
QVariantMap fields;
for (int index = 1; index <= 16; ++index)
    fields[QStringLiteral("ckbox_CH%1").arg(index)] = true;
m_view->applyFields(fields);
return true;                              // 不发后端

// 同一文件 :48（保存对话框取消 → 把开关拨回去）
if (path.isEmpty()) {
    m_view->applyFields({{"pbtn_SaveDynamic", false}});
    return false;
}
```

这是一种刻意的例外：**输入前端的瞬时状态**不该经过业务层。

---

## 6. `applyFields` / `values()` 的字段语法

所有前端—后端通信最终都收敛到这两张"字符串键 → QVariant"的表。这是整套架构唯一的协议，必须熟记。

### 6.1 读：`values()` 产出的形状（`common/FormBinding.cpp:92`）

| 控件 | 键 | 值 |
| --- | --- | --- |
| `QLineEdit`（非只读） | objectName | `QString` |
| `QPlainTextEdit` / `QTextEdit`（非只读） | objectName | `QString` |
| `QComboBox` | objectName | `{index:int, text:QString, data:QVariant}` |
| 可 check 的 `QAbstractButton` | objectName | `bool` |
| `QSpinBox` | objectName | `int` |
| `QDoubleSpinBox` | objectName | `double` |
| `SwitchControl` | objectName | `bool` |
| `QAbstractSlider` | objectName | `int` |
| `QTableView` 选中行 | `objectName + ".selectedRows"` | `QVariantList<int>` |

所以后端统一用两个小工具取值：

```cpp
// application/runtime/WorkbenchRuntime.cpp:107
QString WorkbenchRuntime::text(const QVariantMap &fields, const QString &name) {
    const auto value = fields.value(name);
    return value.type() == QVariant::Map ? value.toMap().value("text").toString()
                                         : value.toString();   // 兼容下拉框与文本框
}
int WorkbenchRuntime::index(const QVariantMap &fields, const QString &name) {
    return fields.value(name).toMap().value("index", -1).toInt();
}
```

### 6.2 写：`applyFields()` 支持的形状（`common/FormBinding.cpp:140`）

| 目标控件 | 值的写法 |
| --- | --- |
| `QLineEdit` / `QLabel` / `QTextEdit` | `QString`（按钮则是文字） |
| 可 check 的按钮 / `QAction` | `bool` |
| `SwitchControl` | `bool` |
| `QSpinBox` / `QDoubleSpinBox` / `QAbstractSlider` | 数值 |
| `QComboBox` | `{items:[...], data:[...], index:n}` / `{text:"..."}` / `{dataValue:...}` |
| `QTableView` / `QTableWidget` | `{headers:[...], rows:[[...], ...]}` |
| `QCustomPlot` | `{series:[{name, samples:[[x,y]...]}]}` / `{x:[], y:[]}` / `{savePath:"..."}` |
| 日志 `QPlainTextEdit` | `{entries:[{text, outgoing}], clear:bool}` |
| 任意 `QWidget` / `QAction` | 键名加 `.enabled` 后缀 + `bool` |
| 面板状态 | `"_status"`（内部会转成 `showStatus()`） |

两个设计细节：

- **`QSignalBlocker` 阻断回环**（`FormBinding.cpp:165`）。写回控件时屏蔽信号，否则 `setText`→`editingFinished`→`relay` 就会自激。
- **增量结构由发送方维护游标**。日志用 `entries + clear` 而不是 `append`，就是为了让 `applyFields` 可重复执行；`publishLatest()` 用 `m_publishedTrafficId[i]` 记录已推送位置。**新增推送字段时，务必先问"重复送达会不会出问题"。**

---

## 7. 三个真实控件案例

### 案例 A：`pbtn_WaveLengthReset`（连接页，一次完整往返）

```
用户点"波长复位"
  → FormBinding::relay                action = "pbtn_WaveLengthReset"
  → PanelPresenter::execute           fields = 整个连接页快照
  → ConnectionPresenter::localAction  不认 → 返回基类 false
  → emit requestReady                 [排队]
  → ConnectionBackend::execute        supports(Connection) ✔
  → FeatureBackend::dispatch
  → WorkbenchRuntime::connectionAction
        L420: action == "pbtn_WaveLengthReset" → baseline(&message)
  → completed(UiResponse{success, message})
  → PanelPresenter::complete          → view->showStatus(...)
```

`baseline()` 做的是"登记一次采样"而不是"立即置零"：

```cpp
// application/runtime/WorkbenchRuntime.cpp:374
m_baselineTarget = count;  m_baselineTimeout = timeout;   // 来自 INI：20 帧 / 10 s
m_baselineCount = 0;  m_baselineSum.clear();  m_baselineClock.start();
*error = tr("开始采样置零：等待%1个有效帧").arg(m_baselineTarget);
return true;
```

真正的推进发生在**周期回调**里：`frameReceived()` 每来一帧累加一次并 `notice("置零采样 3/20")`；凑够 20 帧后算出基线，再 `configure()` 提交给计算模块。这体现了异步业务的通用套路：

1. 请求只做"状态登记 + 参数校验"，立即回执（避免界面卡住）；
2. 后续进度用 `presentationChanged` / `notice()` 推送；
3. 真正的完成由业务层自己的信号驱动（`configurationAccepted`），不是由界面轮询。

### 案例 B：`pbtn_SaveDynamic`（连接页，Presenter 补字段 + 异步完成）

这个控件最能说明三层分工。它是一个 **checkable 按钮**（开始/停止保存共用一个控件）。

**① View 只负责报告"被点了"** —— `values()["pbtn_SaveDynamic"]` 是当前的 checked 状态。

**② Presenter 负责"要不要弹文件框"**：

```cpp
// features/connection/presenters/ConnectionPresenter.cpp:41
bool ConnectionPresenter::prepareRequest(UiRequest &request) {
    if (request.action != "pbtn_SaveDynamic") return true;
    if (!request.fields.value("pbtn_SaveDynamic").toBool()) return true;   // 停止保存，无需路径
    const auto path = m_host->chooseFile(true, tr("选择存储文件"),
                                         tr("CSV (*.csv);;SQLite (*.sqlite *.db)"));
    if (path.isEmpty()) {
        m_view->applyFields({{"pbtn_SaveDynamic", false}});   // 撤销用户刚才的点击
        return false;                                          // 取消请求
    }
    request.fields["filePath"] = path;      // 注入后端需要的字段
    return true;
}
```

**③ Backend 负责"能不能开始"**：`startStorage()` 校验扩展名、防重入、置 `m_storagePending`，并立刻推送"按钮置灰、显示为按下"：

```cpp
// application/runtime/WorkbenchConnection.cpp:210
emit presentationChanged(PanelId::Connection,
    {{"pbtn_SaveDynamic", true}, {"pbtn_SaveDynamic.enabled", false}});
m_calculation->storage().start(configuration);      // 异步
```

**④ 完成由存储模块的信号回推**（`WorkbenchRuntime.cpp:439`）：

```cpp
void WorkbenchRuntime::storageStarted(const QString &session) {
    m_storagePending = false;  m_recording = true;
    emit presentationChanged(PanelId::Connection,
        {{"pbtn_SaveDynamic", true}, {"pbtn_SaveDynamic.enabled", true}});
    notice(tr("存储已启动：%1").arg(session));
}
```

失败路径同理（`storageError`）会把按钮拨回未按下并恢复可点击。

### 案例 C：`cplot_Spectrum` + `tablev_Wavelength`（连接页，周期推送）

频率最高的数据通路，全部在 `publishLatest()` 里（100 ms 定时器驱动，`WorkbenchData.cpp:81`）：

```cpp
if (m_frame && m_presentedFrame != m_frame->revision) {         // 用 revision 去重
    // 曲线：每通道保留 300 个显示采样
    emit presentationChanged(PanelId::Connection,
        {{"cplot_Spectrum", QVariantMap{{"series", series}}}});
    // 表格：最多 2048 行
    emit presentationChanged(PanelId::Connection,
        {{"tablev_Wavelength", table({tr("通道"), tr("传感器"), tr("波长 (nm)")}, rows)}});
    m_presentedFrame = m_frame->revision;
}
```

三个可复用的做法：

1. **`revision` 去重**：数据没变就不推，避免无谓的重绘。
2. **整帧快照下发**：不做增量 diff，界面拿到就能直接重建，`applyFields` 天然幂等。
3. **显示策略留在界面侧**：通道筛选（`m_channels`）在推送前过滤；但波长表、解算、存储仍用全部通道 —— 业务不受显示影响。

---

## 8. 线程、生命周期与边界

### 8.1 线程

当前所有对象都在 GUI 线程，但**代码按跨线程写**：

```cpp
connect(this, &PanelPresenter::requestReady, backend, &IBackend::execute, Qt::QueuedConnection);
connect(backend, &IBackend::completed, this, &PanelPresenter::receiveResponse, Qt::QueuedConnection);
connect(backend, &IBackend::presentationChanged, this, &PanelPresenter::present, Qt::QueuedConnection);
```

带来的行为差异（写代码时必须知道）：

- **一切请求都是异步的**。`execute()` 返回时后端还没跑，界面不能在下一行读结果。
- `values()` 在 GUI 线程取值、`applyFields()` 在 GUI 线程写值，中间只有 `QVariantMap` 副本。
- 想把某个 Backend 挪到工作线程，只需 `backend->moveToThread(...)`，其余代码零改动 —— 这就是"值跨界、指针不跨界"的收益。
- 测试里因此要 `QTest::qWait()` / `QTRY_COMPARE()`，不能用"点完立刻断言"。

### 8.2 面板嵌套

`WorkbenchView` 把 10 个面板塞进一个窗口（有的进 `QStackedWidget`，有的进 `QDockWidget`，有的是独立 `QDialog`）。为防止父表单把子表单的字段一并收走：

```cpp
// common/FormBinding.cpp:38
widget->setProperty("fosdPanelRoot", true);      // 构造时给根控件打标

bool FormBinding::owns(const QObject *object) const {
    for (const QObject *parent = object; parent; parent = parent->parent()) {
        if (parent == m_widget.data()) return true;
        if (parent->property("fosdPanelRoot").toBool()) return false;   // 撞到别的面板，不是我的
    }
    return false;
}
```

顺序也很关键（`WorkbenchView.cpp:29` 有注释）：**先把父表单绑定好，再往里面塞子表单**。否则父表单的构造期 `connect` 会把子面板的按钮也连上，一次点击触发两次。

### 8.3 生命周期

三处 `QPointer` + `destroyed` 处理保证"后端没了界面还能活"：

| 位置 | 行为 |
| --- | --- |
| `PanelPresenter::setBackend()` | 换适配器时断开旧连接、`cancelPending()`、清敏感字段 |
| `PanelPresenter::backendDestroyed()` | 清在途请求，主状态栏提示"后端适配器已断开" |
| `WorkbenchPresenter::registryDestroyed()` | 反向遍历所有 Presenter 置空 backend |

`receiveResponse()` / `present()` 都用 `sender() == m_backend.data()` 过滤 —— 防止**被替换掉的旧适配器**的排队信号污染新状态。

---

## 9. 新增控件的统一方法

### 9.1 先做一次决策：这控件属于哪一类

```
新控件被操作后，谁应该知道？
│
├─ 只有界面自己（切页、开面板、主题、窗口按钮、颜色选择框）
│     → Presenter::localAction() 里 return true
│     → 不动 Backend、不动 Runtime
│
├─ 是"提交时才生效"的输入（一组参数 + 一个确认按钮）
│     → 输入控件：Presenter::localAction() 里 return true（吞掉、暂存界面）
│     → 确认按钮：发后端
│     → 参照 ckbox_CH1..16 + pbtn_SelectCH、edit_SensorXxxOrder + pbtn_SenerSet
│
├─ 是"改一下就生效"的设置（下拉、滑块、开关、数值框）
│     → 直接发后端，由 Runtime 校验并回填
│     → 失败要能纠偏（response.fields 或 presentationChanged）
│     → 参照 cbox_FPS、hsd_GridSize、cbox_AxisAble
│
├─ 只显示数据（标签、表格、曲线、状态栏）
│     → 不需要任何 Presenter 代码
│     → 在 Runtime 的 publishXxx() 或完成回调里 emit presentationChanged
│
└─ 控件类型 FormBinding 不认识（新自研控件）
      → 必须扩 FormBinding：连接 + values() + applyFields() 三处
```

### 9.2 七步清单（按改动顺序）

**Step 1 · `.ui`：起一个稳定的 objectName**

这是协议字符串，一旦被后端、测试、INI 引用就不能再改。

- 前缀沿用现有约定：`pbtn_`/`cbox_`/`ckbox_`/`radioButton`/`edit_`/`textEdit_`/`ptedit_`/`hsd_`/`spin_`/`action_`/`tablev_`/`tableWidget_`/`cplot_`/`lab_`/`statusBar_`。
- 同一面板内必须唯一（`applyFields` 用 `findChildren<QObject*>(name)` 取第一个）。
- 不要以 `qt_` 开头（`named()` 会跳过）。

**Step 2 · View：通常 0 行**

因为 `FormBinding` 构造时已经按控件类型批量连好了。只有三种情况需要动 View：

| 情况 | 做法 | 现有例子 |
| --- | --- | --- |
| `QLineEdit` 需要"输入即提交" | 在 View 构造里手工 `connect(edit, &QLineEdit::editingFinished, ...)` 并 `emit m_binding->actionTriggered(...)` | `connectwindow.cpp:24` 的 `edit_SerialCommand*` |
| 写回时需要附带界面副作用 | 在 `applyFields()` 里加钩子 | `ConnectionView::applyFields` 处理 `_spectrumReset`、曲线选中恢复 |
| 新自研控件 | 扩 `FormBinding`（见 9.4） | `SwitchControl`、`QCustomPlot` |

**Step 3 · Presenter：决定"就地消化 / 补字段 / 直通"**

在对应 `XxxPresenter` 里选一个虚函数重写：

```cpp
// 纯 UI
bool XxxPresenter::localAction(const UiRequest &request) {
    if (request.action == "pbtn_XXX") { m_host->showPanel(PanelId::YYY); return true; }
    return PanelPresenter::localAction(request);
}

// 需要 UI 前置信息
bool XxxPresenter::prepareRequest(UiRequest &request) {
    if (request.action != "pbtn_XXX") return true;
    request.fields["filePath"] = m_host->chooseFile(false, tr("选择文件"), "...");
    return !request.fields["filePath"].toString().isEmpty();
}
```

> 小技巧：`ConnectionPresenter::localAction` 对 `pbtn_ChannelCalibration` 先 `showPanel()` 再 **`return false`**，让请求继续走后端刷新编辑框 —— "先弹窗，再请后端回填"。

**Step 4 · Backend：一般 0 行**

`FeatureBackend::dispatch()` 已经兜底。只有当这个动作要落到**别的服务/别的线程**时才需要改 Backend。

**Step 5 · Runtime：写业务分支**

在 `connectionAction` / `visualizationAction` / `dataAction` / `markerAction` 里加分支，并遵守四条纪律：

```cpp
} else if (action == "pbtn_MyNewThing") {
    // ① 取值：用 text() / index() / toBool()，不要假设类型
    const QString value = text(fields, "edit_MyInput");
    // ② 校验：非法就给出可执行的原因
    if (value.isEmpty()) { response->success = false;
                           response->message = tr("请输入有效值"); return true; }
    // ③ 调 Control 并检查返回值（不要调 void 接口就当成功）
    response->success = m_visualization->setSomething(value);
    if (!response->success) response->message = tr("参数超出范围");
    // ④ 成功后推最终状态（推荐推送全量快照而不是只改一个键）
    if (response->success) publishSomething();
}
```

**Step 6 · CMake：把新文件放进对应静态库**

| 新文件 | 目标 |
| --- | --- |
| `features/*/views/*.{h,cpp,ui}` | `fosd_classic_widgets`（+ `common/ViewFactory.cpp` 若为新面板） |
| `features/*/presenters/*` | `fosd_classic_presenters` |
| `features/*/backends/*` | `fosd_classic_backends` |
| `application/runtime/*` | `fosd_classic_runtime` |

**Step 7 · 测试**

| 测什么 | 挂在哪 | 例子 |
| --- | --- | --- |
| 控件存在、初始值、默认主题 | `tests/WorkbenchTests.cpp`（只链 widgets，无 Runtime） | 断言 `switch_Style` 初值为 true |
| 端到端（点击 → 业务层状态变化） | `tests/workbench_main_tests.cpp`（链 application） | `pbtn_ModelLoad()->click()` 后 `QTRY_COMPARE(models.count(),1)` |
| 纯运行时行为 | `tests/workbench_runtime_tests.cpp` | 直接喂帧、断言曲线/存储 |

### 9.3 模板 1：新增"一次提交"的参数项（推荐新手第一个照抄）

需求：在连接页加"置零采样数"和"应用"按钮。

```cpp
// ① oldmainwindow/connectwindow.ui
//    <widget class="QSpinBox" name="spin_BaselineCount">  range 1..10000
//    <widget class="QPushButton" name="pbtn_BaselineApply">

// ② View：0 行改动。（QSpinBox 已由 FormBinding 连 editingFinished；
//    values() 会收集成 int。）

// ③ Presenter：把 spin 的即时触发吞掉，只在点按钮时提交
bool ConnectionPresenter::localAction(const UiRequest &request) {
    if (request.action == "spin_BaselineCount")
        return true;                       // 暂存界面，等 pbtn_BaselineApply
    ...
}

// ④ Runtime
} else if (action == "pbtn_BaselineApply") {
    bool ok = false;
    const int count = fields.value("spin_BaselineCount").toInt(&ok);
    if (!ok || count < 1 || count > 10000) {
        response->success = false;
        response->message = tr("置零采样数应为 1～10000");
        return true;
    }
    config::IniConfigStore store(config::ParameterStore::runtimePath());
    response->success = store.writeValue("classic.baseline", "sampleCount", count,
                                         &response->message);
    if (response->success) response->message = tr("置零采样数已保存");
}
```

要点：**输入控件必须在 Presenter 里被吞掉**，否则每次改数值都会弹一次"尚未实现"。

### 9.4 模板 2：为 FormBinding 增加一种控件类型

新增自研控件 `GaugeWidget`（有 `valueChanged(double)`、`value()`、`setValue(double)`）时，改三处 + 一处顺序：

```cpp
// 1) common/FormBinding.cpp 构造函数：接信号
for (auto *gauge : widget->findChildren<GaugeWidget *>())
    connect(gauge, &GaugeWidget::valueChanged, this, &FormBinding::relay);

// 2) values()：采集
for (auto *gauge : widget->findChildren<GaugeWidget *>())
    if (owns(gauge) && named(gauge))
        fields[gauge->objectName()] = gauge->value();

// 3) applyFields()：写回（务必放在 QSignalBlocker 之后）
else if (auto *gauge = qobject_cast<GaugeWidget *>(object))
    gauge->setValue(value.toDouble());
```

**顺序陷阱**：`applyFields` 是一串 `if/else if`，`QWidget` 派生类的 `qobject_cast` 必须放在更具体的分支之后，且不能被前面的分支抢先命中。

### 9.5 新增一个面板（PanelId）

比新增控件多四步：

1. `common/UiTypes.h` 加枚举值；`common/UiTypes.cpp` 的 `featureForPanel()` 加映射。
2. `common/ViewFactory.cpp` 加 `case → new XxxView`。
3. 新 View 照抄 30 行样板（见 `disp3dmodeldockpanel.cpp:8`）：`initializeFosdUiResources()` → `setupUi` → `new FormBinding(PanelId::Xxx, this, this)`。
4. `application/WorkbenchView.cpp` 决定它进 `QStackedWidget`、`QDockWidget` 还是独立窗口。
5. 若属于新功能，`FeatureId` 也要加，并在 `WorkbenchPresenter`/`WorkbenchBackend` 的 `switch`/`setBackend` 里注册。

注意 `MainWindowPresenter` 的导航用 `navigation.indexOf(action)` 当页索引，页序与 `QStackedWidget::addWidget()` 的顺序强耦合 —— 插页时要同步改。

---

## 10. 常见坑（全部来自现有代码的真实约束）

| # | 坑 | 说明 / 正确做法 |
| --- | --- | --- |
| 1 | 以为改 `QLineEdit` 就会发请求 | 只有 `le_MapRange*` 和 `edit_SerialCommand*` 接了 `editingFinished`。其余文本框只在别的按钮提交时被 `values()` 一起带走 |
| 2 | 以为 `setCurrentIndex()` 会触发下拉框动作 | 连的是 `QComboBox::activated`（仅用户操作），且 `applyFields` 还有 `QSignalBlocker`。这是防回环设计，别改成 `currentIndexChanged` |
| 3 | 失败时用 `response.fields` 回填却没生效 | `complete()` 只在 `success == true` 时应用。要么走 `WorkbenchRuntime` 的自动纠偏（它转发成 `presentationChanged`），要么自己 emit |
| 4 | 控件自动挂了信号但后端不认 | 用户会看到"XXX 的业务适配尚未实现"。要么 `localAction` 吞掉，要么给软提示分支 |
| 5 | 把 `QWidget*` 塞进 `UiRequest` | 契约只允许值类型。界面的东西留在界面 |
| 6 | 在 View/Presenter 里 `new` 一个 `src/` 下的 Control | 绕过服务层就等于绕过线程边界，也让 `test_workbench_runtime` 覆盖不到 |
| 7 | 假定 `fields` 只有变化项 | 它是**全面板快照**。后端只读自己关心的键，别做"键数量"推断 |
| 8 | 推送结构不可重复执行 | 日志用 `entries + clear` 而不是 `append`；周期数据用 `revision` 去重。新字段先问"重复送达会怎样" |
| 9 | 把子表单塞进父表单后父表单多收一遍值 | 子表单的 `FormBinding` 会给根控件打 `fosdPanelRoot`；父表单绑定要在塞子表单**之前**构造 |
| 10 | 点完立刻断言 | 三处连接都是 `QueuedConnection`，请求是异步的。测试用 `QTRY_*` / `qWait` |
| 11 | 改了 objectName 就断链 | 后端、测试、INI 键、`startsWith("ckbox_CH")` 这类前缀匹配全靠它。改名要全仓 `grep` |
| 12 | 用 `void` 设置函数还报成功 | 审阅文档 F05 记录的正是这个：`setfreshFPS`、`setCameraLocked` 等无返回值，失败也会显示"设置已应用"。新代码应返回 `bool`/错误 |

---

## 11. 一句话总结每一层

| 层 | 一句话 | 代码量级 |
| --- | --- | --- |
| View | "我只认识 objectName 和 QVariantMap，别的一概不知" | 每个 30 行样板 + 一次 `relayControl` 转发 |
| Presenter | "把点击翻译成请求，把回执翻译成界面；纯 UI 的事我自己办" | 每个 20～60 行 |
| Backend | "我是注册表里的插头，负责把请求送到服务端口，并按面板过滤回推" | 每个 15 行 |
| Services/Runtime | "我拥有全部业务状态和定时器，只吐快照" | 最大的一块，按功能拆成 4 个 `.cpp` |
| Controls | "我不知道有界面这回事" | `src/`，独立可测 |

---

## 12. 建议的阅读顺序

1. `common/UiTypes.h`（50 行）→ `common/IView.h` / `common/IBackend.h` / `common/ViewEvents.h`（各 20 行）
2. `common/FormBinding.cpp` 的 `values()`（46 行）与 `applyFields()`（180 行）—— 协议全在这
3. `common/PanelPresenter.cpp`（163 行）—— 生命周期全在这
4. `features/mainwindow/` 三个文件（140 行）—— 最短的一条完整链
5. `application/WorkbenchPresenter.cpp` + `WorkbenchBackend.cpp`（135 行）—— 装配与路由
6. `application/ClassicWorkbench.cpp`（34 行）—— 装配顺序
7. `application/runtime/WorkbenchConnection.cpp` 的 `connectionAction()`（250 行）—— 真实业务的写法样板
8. `tests/workbench_main_tests.cpp` —— 看别人怎么驱动这条链

读完 1～6 大约 500 行，就能完整回答"一个点击怎么变成一次业务调用、一个结果怎么回到界面"。
