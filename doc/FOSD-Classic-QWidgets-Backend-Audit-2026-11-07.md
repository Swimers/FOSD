# FOSD-Classic-QWidgets 前后端功能闭环审阅

审阅日期：2026-11-07。审阅对象为当前源码，最终核对基准提交 `bafb8ab65f513d8e6569c9070c37d0d376a72541`（串口数据接入解析器），包含近期通信拆包与配置结构调整。

## 结论

**当前 Classic 的主要问题不是大量按钮完全没有后端，而是参数覆盖范围、配置恢复和操作结果反馈没有形成完整闭环。** 现有 A/B 参数、12 路传感器排序与系数、置零、通信收发、模型导入、位移着色、关键点管理和数据保存都有实际实现，不能列为“未实现”。

优先处理以下问题：

1. **串口高级参数会被默认值覆盖**：INI 支持数据位、校验位、停止位和流控，但 Classic 打开串口时仅采用界面的端口、波特率。
2. **已保存的显示刷新率会在启动时被改写为 25 Hz**：初始化强制调用 `setfreshFPS(40)`，该调用不仅更新计时器，还会写回 INI。
3. **参数恢复到后端、恢复到控件并不一致**：变形倍率未回填，UDP/TCP 地址、端口及模式恢复不完整。
4. **部分设置没有可靠的失败回执**：若底层 `void` 设置函数因 INI 写入等原因失败，界面请求仍可能返回成功。
5. **故障诊断、设备应答、完整参数编辑和高级存储选项仍只有基础能力或缺少 Classic 入口**。

下文将“确定的闭环缺陷”“部分实现或缺少前端入口”“主动裁剪及能力边界”分开说明。这里的 P1/P2 是建议修复顺序，不代表已经进行现场严重性评估。

## 1. 审阅范围与方法

审阅覆盖：

- 12 个现存 `.ui` 表单中的 95 个按钮、复选框、单选框、下拉框、滑块、数值框和 QAction；另外检查动态创建的 TCP 角色控件、解算控制按钮、10 组快捷指令，以及文本输入的提交方式。
- `FormBinding → PanelPresenter → FeatureBackend → WorkbenchRuntime → 各业务 Control` 的请求、状态回写和错误路径。
- `CoreParameters.h`、`ParameterStore`、参数编解码、参数验证，以及通信、计算、存储、可视化模块的实际使用。
- 现有 Classic 及协议测试源码，用于确认已经覆盖的行为和遗漏的场景。

静态动作名扫描中，只有 `ckbox_CH1`～`ckbox_CH16` 没有逐个出现为字符串常量；它们由 Presenter 的 `startsWith("ckbox_CH")` 和运行时循环统一处理，**不是遗漏的接口**。其他静态动作名均能找到路由引用；这只能证明入口可追踪，不能证明设置一定生效或已经通过运行验证。

本次为源码静态审阅，没有修改业务代码、重新编译或运行当前源码测试。此前发行版本的“17 项测试通过”不能代替对 11 月 7 日新增修改的验证。当前 IDE 提及的 `src/config/CoreParameters.cpp` 已被删除，不能据此判定配置后端缺失；参数结构、编解码、验证与持久化分别在头文件和配置模块其他文件中。

## 2. 参数设置：哪些已经实现

| 前端功能 | 请求及后端路径 | 实际行为 | 判定 |
| --- | --- | --- | --- |
| 通信页参数 1/2，A/B 确认 | `pbtn_ParaSet` → `connectionAction()` → `configure()` → `CalculationControl::setConfiguration()` | 解析两个数值，校验完整标定配置，异步提交，收到 `configurationAccepted` 后确认配置并回填 | 已实现；依赖有效基线和通道映射，不是任意时刻都能设置成功 |
| A/B 重置 | `pbtn_ParaRest` | 将 A/B 改为 `1/0`，仍经过完整配置校验 | 已实现；只重置 A/B，不是恢复全部出厂参数 |
| 12 路序号与校准系数 | `pbtn_SenerSet` → `dataAction()` | 将 1 起始的界面序号转换为 0 起始索引，构造 12 项通道映射和倍率，再提交计算配置 | 已实现；固定为当前 HYY 界面的 12 路形式 |
| 波长复位/均值置零 | `pbtn_WaveLengthReset` → `baseline()`、`frameReceived()` | 采集请求之后的新帧，检查来源与布局、正有限波长，平均后更新基线；超时或连接关闭会取消 | 已实现；采样数和超时目前通过 INI 配置 |
| 读取 INI 标定配置 | `pbtn_ConfigRead` | 读取 `CalculationConfiguration`，提交并等待计算配置回执 | 已实现；读取对象仅为计算配置，不是全部模块热重载 |
| 变形倍率 | `spin_DeformationScale` → `setDeformationScale()` | 改变显示变形倍率并持久化 | 后端已实现；存在启动回填和失败反馈缺口，见 F03、F05 |
| 标量映射设置 | 映射/清除、X/Y/Z/总位移、归一化、符号、自动/手动范围 | 调用 `setScalarColorSettings()`，检查返回值，并通过信号刷新控件 | 已实现；自定义节点标量没有 Classic 数据输入入口 |
| 坐标轴、网格、视角、锁定 | `ScenePanel` 的动作 → `VisualizationControl` | 设置显示、尺寸、网格平面、六个预设视角和相机锁定 | 已实现基础子集；不是完整场景参数编辑器 |

源码证据：[A/B 与配置读取](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L176)、[传感器映射及参数回填](../apps/FOSD-Classic-QWidgets/backend/WorkbenchData.cpp#L18)、[配置提交与置零](../apps/FOSD-Classic-QWidgets/backend/WorkbenchRuntime.cpp#L254)、[计算配置校验及持久化](../src/calculation/CalculationControl.cpp#L65)、[标量及场景路由](../apps/FOSD-Classic-QWidgets/backend/WorkbenchVisualization.cpp#L7)。

**参数的作用域需要说明清楚：A/B 和传感器系数用于本机波长→应变→HYY 计算，不会自动转换成下位机配置命令。** 计算标定公式为 `calibrationFactor × (当前波长 − 基线波长) / A + B`；A 必须非零，基线和通道映射不能是空数组。首次使用尚未置零时设置失败，属于已有校验约束，不是按钮没有实现。[标定公式与校验](../src/calculation/StrainCalibration.cpp#L33)

## 3. 确定的配置及交互闭环缺陷

### F01 · P1：串口高级参数没有前端入口，打开串口还会覆盖已保存值

`SerialConfig` 已包含 `dataBits`、`parity`、`stopBits`、`flowControl`。Classic 打开串口时却创建一个新的默认 `SerialConfig`，只填 `portName` 和 `baudRate`。`ConnectControl::openSerial()` 随后把整个结构写入 INI。

因此，用户即使手动将 INI 设置为非默认校验位、停止位等，下一次从 Classic 打开串口也会按默认值重建并保存，可能导致设备无法通信。问题同时涉及“缺少入口”和“丢失未展示参数”，不能仅补几个控件名称。

**建议**：先读出完整配置，再由界面覆盖已展示字段；增加数据位、校验、停止位和流控设置，并在打开前展示有效参数。验收应覆盖“7 数据位、偶校验、2 停止位”等非默认配置的保存、重启及打开请求，不能只验证默认 8N1。

证据：[串口配置结构](../src/config/CoreParameters.h#L30)、[Classic 重建串口配置](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L219)、[打开前写入完整配置](../src/communication/ConnectControl.cpp#L245)。

### F02 · P1：刷新率的恢复结果被启动代码覆盖

可视化构造函数通过 `ParameterStore` 恢复 `VisualizationRuntimeSettings.refreshIntervalMs`；但 Classic 的 `initialize()` 随后无条件执行 `setfreshFPS(40)`，并把下拉框设置为索引 2，即 25 Hz。`setfreshFPS()` 会将传入的周期写回 INI。

**可推导结果**：保存的 60 Hz、144 Hz 或其他刷新周期在下次启动时会被改为 40 ms。后端持久化接口存在，恢复流程却没有闭合。

**建议**：初始化仅根据实际恢复值回填 UI，不调用具有写入副作用的设置方法。下拉框无法精确表示自定义周期时，应显示自定义项或保留原值。验收应检查启动前后 INI 内容不变。

证据：[恢复运行时参数](../src/visualization/VisualizationControl.cpp#L78)、[启动时强制设置](../apps/FOSD-Classic-QWidgets/backend/WorkbenchRuntime.cpp#L143)、[刷新率设置会写入 INI](../src/visualization/VisualizationControl.cpp#L277)。

### F03 · P2：变形倍率只恢复到后端，没有恢复到输入框

`VisualizationControl` 恢复并应用保存的 `deformationScale`，而 `spin_DeformationScale` 在 `.ui` 中默认是 1。Classic 初始化与现有发布函数没有将恢复值写回该控件。

**影响**：INI 设为 50 时，模型可能按 50 倍显示，界面仍显示 1；用户无法从当前控件判断真实状态。

**建议**：提供可查询的运行时设置或完整状态快照，初始化时回填倍率与刷新率。验收直接比较“INI、后端有效值、控件值”三者。

证据：[后端恢复倍率](../src/visualization/VisualizationControl.cpp#L78)、[控件默认值](../ui/FOSD-Classic-QWidgets/apps/model/views/disp3dmodeldockpanel.ui#L290)、[Classic 初始化](../apps/FOSD-Classic-QWidgets/backend/WorkbenchRuntime.cpp#L109)。

### F04 · P2：网络连接参数恢复不完整，且 UDP 与 TCP 共用字段缺少切换回填

初始化只恢复串口名称/波特率、部分 UDP 端口/目标，以及 TCP 角色。UDP `bindAddress` 没有回填，本地地址列表最终默认选索引 0；TCP `host`、`listenAddress`、`port` 没有按角色完整恢复。UDP 仅在保存的 `localPort` 非零时恢复部分字段，合法的随机端口配置也会跳过这段回填。

`cbox_CommuteChoose` 切换仅改变 `m_networkChannel`，没有按新通道加载相应参数；界面默认仍是 UDP。TCP 打开时会读取界面上可能来自 UDP 或 Designer 默认值的地址和端口，再保存为 TCP 配置。

**建议**：以通道和 TCP 角色分别维护编辑状态；切换时回填对应配置，并明确区分未保存草稿和有效配置。验收应使用不同的 UDP/TCP 地址与端口，覆盖随机本地端口和 TCP 服务端模式。

证据：[启动回填逻辑](../apps/FOSD-Classic-QWidgets/backend/WorkbenchRuntime.cpp#L109)、[网络切换与打开](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L204)、[通用下拉框回填方式](../ui/FOSD-Classic-QWidgets/common/FormBinding.cpp#L163)。

### F05 · P2：部分设置在底层失败后仍会返回“设置已应用”

`visualizationAction()` 默认将响应标记为成功。刷新率、变形倍率、坐标轴总开关、网格总开关、相机锁定等分支调用的是 `void` 接口。部分接口内部可能因为参数文件读写失败而提前返回或回滚，但 Runtime 没有将此结果反映到 `UiResponse.success`。

底层 `errorOccurred` 已接入 `notice()`，因此并非完全没有错误提示；真正缺少的是**同一次操作的失败结果、控件回滚和成功响应的一致性**。标量设置等已有 `bool` 返回值并被检查的路径，不应一并判为未实现。

**建议**：同步设置返回 `bool/error` 或有效值，异步设置使用请求编号与完成回执。只有实际生效后才报告成功；失败时回填旧值。验收应覆盖不可写 INI、非法配置和持久化失败。

证据：[默认成功及相关分支](../apps/FOSD-Classic-QWidgets/backend/WorkbenchVisualization.cpp#L20)、[倍率设置的失败出口](../src/visualization/VisualizationControl.cpp#L247)、[刷新率设置的失败出口](../src/visualization/VisualizationControl.cpp#L277)、[相机锁定](../src/visualization/VisualizationControl.cpp#L496)。

### F06 · P2：系数设置窗口“取消”只有关闭，没有恢复草稿

`pbtn_SenerCancel` 由通用 Presenter 关闭面板。关闭仅清理敏感输入，不恢复 12 路排序和系数；再次打开只是显示窗口，也不主动发布已生效参数。

**影响**：未确认的修改不会提交给后端，但可能留在控件里，下次打开仍显示草稿。它不是空按钮，而是“取消编辑”的语义不完整。

**建议**：打开时加载有效配置，取消时丢弃草稿；或者明确标注草稿状态。验收应覆盖“修改→取消→重新打开”，确认没有改变后端且控件恢复有效值。

证据：[取消动作](../ui/FOSD-Classic-QWidgets/common/PanelPresenter.cpp#L125)、[面板显示与关闭](../ui/FOSD-Classic-QWidgets/apps/WorkbenchView.cpp#L131)、[仅清理敏感字段](../ui/FOSD-Classic-QWidgets/common/FormBinding.cpp#L272)。

## 4. 部分实现或缺少 Classic 入口的能力

### F07 · 故障诊断目前只是连接状态与帧计数快照

点击“故障诊断”能打开面板；点击“启动”只填写 `Serial=%1 UDP=%2 TCP=%3; frame=%4`，连接变化时也会更新状态字段。没有看到独立诊断任务、设备探测、帧率/丢包统计、命令超时分析或诊断报告导出。

因此应称为“连接状态查看”，不能按完整故障诊断验收。若要建设诊断功能，需先明确设备支持的探测指令及错误码，再增加诊断状态机和明确的完成/失败反馈。

证据：[打开诊断窗口](../ui/FOSD-Classic-QWidgets/apps/connection/presenters/ConnectionPresenter.cpp#L20)、[诊断启动实际处理](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L383)。

### F08 · 配置后端已有较完整参数结构，Classic 只暴露其中一部分

目前没有一个统一的参数编辑、校验、导入导出和恢复默认值界面。缺少入口不等于参数模块没有实现：许多参数能通过 INI 加载，或者已有对应的 Control 方法。

| 参数组/能力 | Classic 当前入口 | 尚未暴露或未闭合部分 | 后端情况 |
| --- | --- | --- | --- |
| `communication.serial` | 端口、固定列表波特率 | 数据位、校验位、停止位、流控；完整回填与保留 | 结构、验证、传输已有；存在 F01 |
| `communication.udp/tcp` | 本地/目标地址端口、UDP/TCP、TCP 角色 | 按通道恢复有效配置 | 传输与持久化已有；存在 F04 |
| `calculation` | A/B、置零、12 路索引与倍率 | `strainCalibration.calibrationFactor`、基线数组的查看/导入导出 | 标定计算、校验与持久化已有 |
| `classic.baseline` | 置零按钮 | `sampleCount`、`timeoutMs` 的参数控件 | INI 读取与范围校验已有，默认 20 帧/10 秒 |
| `visualization.runtime` | 倍率、FPS | 两者完整恢复与状态回填 | 后端已实现；存在 F02、F03 |
| `visualization.axes` | 开关、整体尺寸 | 原点、字号、标签可见性 | 参数加载及设置接口已有 |
| `visualization.grid` | 总开关、三个平面、尺寸滑块 | 独立 divisions、颜色、透明度、线宽、原点 | 参数加载及设置接口已有；当前滑块同时改变范围和 divisions |
| `visualization.background/world` | 没有相应编辑控件 | 渐变、地面/天空、纹理、颜色等 | INI 与设置接口已有；旧入口属于此前裁剪项 |
| `visualization.camera` | 六个预设视角、锁定 | 投影、视角角度、平行缩放、相机姿态编辑/复位入口 | 对应 Control 能力已有 |
| `visualization.keyPoint/selection` | 点的选择、增删、显示 | 半径、颜色、字体、标签偏移、球面分辨率 | 样式参数及设置接口已有 |
| `visualization.picking` | 鼠标拾取、按 ID 查询 | 拾取开关、容差、仅拾取关键点、相机交互时清除选择 | 后端参数及交互能力已有 |
| `visualization.scalar` | 位移四种字段及基本色图模式 | 色相端点、自定义节点标量数据输入 | 色图和通用节点标量入口已有；硬件应变到模型节点的空间映射仍需业务定义 |
| 日志显示策略 | 手动清除、部分显示模式 | 60 秒、200 条、10 Hz、64 字节预览的可配置入口 | 当前为 Runtime/UI 中的固定策略，不属于现有参数结构 |

建议先增加“计算参数/串口参数/显示参数”三个小范围编辑入口，复用 `ParameterCodec` 和既有验证，不要再建立一套与 INI 相互独立的参数副本。场景高级功能是否恢复到 Classic，应由产品范围决定。

证据：[参数定义](../src/config/CoreParameters.h)、[通用参数存储](../src/config/ParameterStore.h)、[可视化启动恢复](../src/visualization/VisualizationControl.cpp#L49)、[可视化能力接口](../src/visualization/VisualizationControl.h#L44)、[Classic 已暴露设置](../apps/FOSD-Classic-QWidgets/backend/WorkbenchVisualization.cpp#L7)。

### F09 · 高级存储配置有后端，但 Classic 保存时使用固定策略

连续保存可以正常走 CSV/SQLite 写入；格式根据用户选择的文件扩展名决定，并非缺少格式实现。但 `startStorage()` 新建 `StorageConfiguration`，连续保存固定包含波长、标定数据和位移，并设置 `allPoints=true`。没有为用户提供 `dataTypes`、`pointIds` 和 `allPoints` 的选择控件，也没有把保存过的完整存储配置恢复成编辑状态。

**缺口**：无法在 Classic 中配置“只存位移”“只存指定关键点”等；手动编辑 INI 的这类选项也不会自动成为本次保存请求的配置。单次保存当前明确是波长快照，不应误称为全部结果快照。

**建议**：增加数据类型与点集选择，启动前显示有效配置；以 CSV/SQLite 读回内容作为验收依据。

证据：[固定保存策略](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L135)、[存储配置结构](../src/config/CoreParameters.h)、[存储启动与配置保存](../src/storage/StorageControl.cpp#L45)。

### F10 · 串口快捷发送已实现，但设备配置/应答闭环尚未接入

当前串口快捷指令能持久化并发送，普通串口发送与快捷发送均自动附加实际 CRLF。该功能是字符串发送，不是下位机参数管理协议。

当前工作区新增了 `IPacketDecoder → ConnectWorker → ConnectControl::commandReceived`，但 Classic Runtime 没有订阅这个信号来处理设备应答。串口原始文本仍通过 `rawDataReceived` 显示，所以不是完全收不到回复；缺少的是命令与回复对应、ACK/NACK、超时、读取设备参数并回填、设置成功后再确认等业务适配。

**建议**：先确定设备命令/应答格式和编码，再增加命令服务及完成事件；不能仅根据 `requestSend` 已发出就宣布设备参数已设置成功。

证据：[串口发送与快捷指令](../apps/FOSD-Classic-QWidgets/backend/WorkbenchConnection.cpp#L118)、[新增命令信号](../src/communication/IPacketDecoder.h#L43)、[命令转发](../src/communication/ConnectControl.cpp#L34)、[Classic 当前订阅](../apps/FOSD-Classic-QWidgets/backend/WorkbenchRuntime.cpp#L19)。

### F11 · 模型与关键点能操作，但没有项目/会话恢复功能

导入 STL、清除模型、查询点、关键点增删与原始/位移坐标显示均有后端；但没有看到保存并恢复模型路径、关键点集合、当前选择和相关会话状态的入口或完整实现。`setKeyPoints()` 更新当前模型下的关键点，不写入 `ParameterStore`；加载模型会重新设置关键点管理器的模型。

如果“关闭软件后继续上次工程”是目标，这部分尚未闭环。建议作为独立会话文件设计，记录模型身份/路径与点 ID 校验，不要只把关键点 ID 无条件套用到其他模型。

证据：[模型加载会重置相关状态](../src/visualization/VisualizationControl.cpp#L133)、[关键点更新不持久化](../src/visualization/VisualizationControl.cpp#L400)、[Classic 模型操作](../apps/FOSD-Classic-QWidgets/backend/WorkbenchVisualization.cpp#L40)。

## 5. 当前串口调整的额外协议风险

### F12 · 需核实：按分隔符拆串口二进制包的协议约束

这是近期串口通信调整涉及的接口边界，不是已经复现的真机故障。

当前 `SerialPacketDecoder` 先按默认 `\r` 找分隔符，再判断分隔符之前是否为 MP/JCS 二进制帧。头文件要求发送方转义二进制载荷中的分隔符，但当前拆包代码没有对应的去转义实现。普通 IEEE 754 浮点载荷可以包含 `0D` 字节，所以不能据“部分串口样本能识别”认定任意原始二进制波长包均可使用该通道。

**建议**：明确设备串口究竟承载文本命令还是二进制波长。若两者混用，需规定可靠的长度/类型分帧或完整转义/去转义协议，并对齐发送端；前端应展示实际工作模式。验收应包含载荷内 `0D`、`0A`、半包、多包、文本/二进制混合、超长丢弃及恢复。

证据：[串口协议约束注释](../src/communication/PacketDecoders.h#L53)、[先查找分隔符的实际算法](../src/communication/PacketDecoders.cpp#L232)、[默认工厂](../src/communication/PacketDecoders.cpp#L274)。

## 6. 按面板汇总当前完成度

| 面板 | 当前已接入能力 | 主要缺口/限制 |
| --- | --- | --- |
| Main | 页面导航、主题、窗口控制 | 没有统一参数中心；导航由前端处理，不需要业务 Runtime 分支 |
| Connection | UDP/TCP/串口、双角色、收发、日志、快捷指令、置零、A/B、筛选、保存、解算启停 | F01、F04、F08、F09、F10；不是一组空接口 |
| ConnectionDebug | 状态查看、关闭 | F07：尚无完整诊断任务 |
| Plot | 开始/暂停观察结果表、打开系数窗口 | 当前是结果表，最多展示前 256 个点；没有分页/全量查看。不能把此页当作位移时间曲线功能 |
| PlotSettings | 12 路顺序和系数提交、取消关闭 | F06；固定 12 路并非可动态扩展的传感器映射编辑器 |
| Visualization | 模型/视图/信息 dock、显示启停/复位、硬件数据入口 | Start/Pause/Stop 控制显示订阅，不等同于通信和解算启停；Stop 当前也不关闭连接或清除数据 |
| ModelPanel | STL 导入清除、倍率、刷新率、位移色图、关键点入口 | F02、F03、F05、F11；没有自定义节点场数据导入 |
| ScenePanel | 坐标轴、网格、六个视角、锁定 | 参数只暴露子集；部分 void 设置存在 F05 |
| InfoPanel | 点数、关键点数、面数、最大位移 | 为只读显示面板，无需人为增加“确认”接口 |
| KeyPointEdit | 全选/清空关键点、按 ID 查询、坐标显示 | F11：未形成持久化会话 |
| KeyPointSelect | 点增删、关键点显示、原始/位移坐标切换 | 同上；表格只读是当前设计，不是编辑提交丢失 |
| HardwareData | 选中应变与来源索引、自动刷新开关 | 没有在此面板直接修改标定配置；标定入口在 PlotSettings |

波长表最多展示 2048 条记录，曲线每条保留 300 个显示采样点，观察表最多 256 个点。这些是当前显示策略，不等于解析器或存储只接受相同数量；建议在界面明确提示截断/采样，后续若要求全量浏览再补分页与查询。

证据：[面板创建](../ui/FOSD-Classic-QWidgets/common/ViewFactory.cpp#L19)、[动态控件及界面组合](../ui/FOSD-Classic-QWidgets/apps/WorkbenchView.cpp#L24)、[数据显示及数量限制](../apps/FOSD-Classic-QWidgets/backend/WorkbenchData.cpp#L119)、[显示控制语义](../apps/FOSD-Classic-QWidgets/backend/WorkbenchVisualization.cpp#L23)。

## 7. 不应混入“现存按钮未实现”的项目

| 项目 | 当前情况 | 如何归类 |
| --- | --- | --- |
| 账户/登录、独立追踪点、Plate 算法与演示、模型合并/导出、旧颜色窗口等 | 此前实施记录已主动裁剪，当前 Classic 没有对应入口 | 产品范围变更；不是本次新发现的死按钮 |
| 网格优化 | `MeshManager::optimize()` 明确返回不支持；Classic 无对应操作控件 | 后端明确未实现，但不影响当前现存按钮闭环 |
| JCS 单通道曲线协议 | `JCSChannelParser` 标为退役，生产工厂未注册；当前生产解析器为 MP 与 JCS 波长 | 独立协议能力边界；如要恢复，应同时实现数据模型、拆包、注册和显示 |
| 通用节点标量、场景高级参数 | 对应后端入口已有，但 Classic 没有完整编辑/输入路径 | 前端能力覆盖不足或未来扩展，不应写成整个后端不存在 |
| 泛型 `reportUnavailable()` | 是未知动作和服务缺失时的兜底 | 不能仅凭该函数存在就断言全部业务适配未实现 |
| 系数文本框没有逐字提交 | 由“确定”按钮统一收集 `values()` 并提交 | 正常的批量编辑方式，不是信号漏接 |
| TCP 角色是运行时动态控件 | 打开网络时从面板值读取 | 不要求每次改变下拉框立即打开/重配通道 |

证据：[迁移范围记录](MigrationCompletion-2026-11-04/README.md)、[明确不支持的优化](../src/visualization/mesh/MeshManager.h#L14)、[退役曲线解析器](../src/protocol/JCSChannelParser.h#L7)、[生产工厂](../src/protocol/IFrameParser.cpp#L8)、[兜底分派](../ui/FOSD-Classic-QWidgets/common/FeatureBackend.cpp#L27)。

## 8. 建议实施顺序与验收清单

| 阶段 | 建议工作 | 可验证的完成标准 |
| --- | --- | --- |
| 第一阶段：配置不丢失 | F01、F02、F03、F04 | 串口非默认参数重开不变；启动不改写 FPS；倍率控件与后端一致；切换 UDP/TCP 正确显示各自配置 |
| 第二阶段：操作结果可信 | F05、F06；明确 A/B 的本地标定作用 | INI 不可写时不报告生效；失败回滚控件；取消丢弃草稿；配置异步回执与对应操作一致 |
| 第三阶段：补足参数编辑 | F08、F09 | 标定常量、置零采样参数、存储类型与点集可编辑、校验、持久化并回填 |
| 第四阶段：设备与会话能力 | F07、F10、F11，先确认 F12 | 设备请求有 ACK/NACK/超时结果；诊断有明确输出；工程重开恢复模型及合法关键点；串口分帧契约通过边界测试 |

现有测试源码已经涉及 TCP 两种角色、均值置零、解算启停、采集到显示/文件保存、串口快捷指令、显示编码、UDP 摘要和清理。**未在现有 Classic 测试中找到针对 F01～F06 的完整回归场景**，尤其是非默认参数重启、启动不改写配置和持久化失败回滚。单独的参数模块测试只能证明编码/验证/保存接口工作，不能证明 Classic 传入了正确值。

测试入口：[运行时测试](../apps/FOSD-Classic-QWidgets/tests/workbench_runtime_tests.cpp)、[主程序界面集成测试](../apps/FOSD-Classic-QWidgets/tests/workbench_main_tests.cpp)、[UI 存在性检查](../ui/FOSD-Classic-QWidgets/tests/WorkbenchTests.cpp)、[参数模块测试](../test/parameters_tests.cpp)。

本轮交付为审阅报告；上述修复、补充控件与设备协议实现均尚未在本轮执行。
