# Classic 功能迁移源码核查（2026-11-03）

## 结论

当前 Classic 独立程序已具备 MVP、通信→协议→标定→解算→三维显示→存储的主链路，但尚不能判定与旧版功能基本一致。优先处理 **JCS 实际协议布局不一致、TCP 服务器入口丢失、基线/通道映射兼容性**；随后补齐独立坐标观察与追踪点、模型操作、参数持久化及保存策略。界面文件存在、Backend 能收到请求或测试通过，都不能单独作为旧功能完成迁移的证据。

本次只新增审计文档和验证记录，没有修改业务源码。以下是源码可确认的缺项与行为差异；没有真实设备、同输入双版本对照和性能测量的项目，明确列为待验证，不声称已经观察到视觉或测量效果下降。

## 核查范围与证据口径

| 项目 | 实际核查基线 |
| --- | --- |
| 旧项目 | `D:/Documents/GitHub/FOSD_Project`，当前 `FOSD-ARMLinux`，HEAD `2e1c848fc2375172ef54e09e935b8ea5a6a4eca3` |
| 新项目 | `D:/Documents/GitHub/FOSD-ReArc`，当前 `FOSD-ReArc`，HEAD `2eb22ee60f38b32905d0c5261fc7eeee5592a3a7`，加当前工作区源码 |
| 本次主要入口 | `apps/FOSD-Classic-QWidgets` + `ui/FOSD-Classic-QWidgets` + 共享 `src`，对应 `fosd-widgets` preset |
| 入口区别 | 本地/远端分支列表中没有名为 `FOSD-Classic-QWidgets` 的 Git 分支；本报告按同名应用目录核查，没有切换分支。`apps/FOSD-Win` 单列说明，不混入 Classic 的完成度 |
| 已有未提交改动 | `src/visualization/scene/SceneEnvironment.h`，核查时存在，未改动 |

方法：从旧 `main.cpp` 的 `OldMainWindow("1")` 追踪到通信、二维、三维窗口和真实槽函数，再核对新 View→Presenter→FeatureBackend→WorkbenchRuntime→Control/Worker 的处理及数据回传。覆盖通信与拆包、标定和计算、模型与场景、关键点与追踪点、二维观察、存储、配置与账户，另外阅读现有测试覆盖范围。仓库内旧审计/说明作为背景材料，不视为用户本次对迁移范围的指令；结论以实际源码为准。

状态：**缺失**＝当前入口无实际业务处理；**差异**＝两边均有实现但语义不同；**待验证**＝静态分析不足以证明等价；**条件恢复**＝旧版有代码，但当前入口未启用或新代码明确标为退役，需要先确定保留范围。P0 为接入/核心输入兼容问题，P1 为主要使用功能，P2 为辅助功能与交付补全。

## 已完成的主体，避免重复重构

| 能力 | 当前证据与实际边界 |
| --- | --- |
| Classic 业务组合与渲染宿主 | [ClassicWorkbench.cpp](../../ui/FOSD-Classic-QWidgets/application/ClassicWorkbench.cpp) 创建 Runtime、注入 Services、挂载 QVTK；不同于仍是骨架的 FOSD-Win |
| 三种传输与波长链路 | `src/communication` 已有串口、UDP、TCP 客户端/服务器能力和拆包；`src/protocol` 有 MP/JCS 波长解析。Classic 已接打开、关闭、发送、清屏和状态回显，但旧 JCS 布局与 TCP 角色接入仍有 P0 差异 |
| 标定和 HYY/Plate | [CalculationControl.cpp](../../src/calculation/CalculationControl.cpp) 有标定、通道重排、模型遍历、结果版本和硬件数据出口；Classic 可置零、改 A/B、改 12 路映射/系数、选择 HYY/内置 Plate。存在输入配置兼容与数值验收缺口 |
| STL、三维变形、位移着色 | 加载/清理、倍率、四种位移量、正负/绝对值、自动/手动色域、归一化均有 Runtime 分支及 Control。三维加载/配置回执用于屏蔽部分旧结果 |
| 基础场景与网格选点 | 三平面网格、坐标轴及尺寸、六视角、相机锁；拾取、原始/显示/真实位移坐标、关键点全选/清空及部分增删已接；模型点数、面数和最大位移已回传 |
| 硬件表与二维结果表 | 无几何模型也可产出标定数据；二维观察有结果表和观察开关，但不等价于旧版独立坐标文件工作流 |
| CSV/SQLite | 有三类记录、异步写入、错误/启停回显、连续保存和单次波长保存；不等价于旧版定时采样、波长+应变联合快照 |
| 界面组织 | 页面导航、窗口关闭/最小化/全屏、主题切换及子面板打开多由 Presenter 本地处理，不能因为 Runtime 未列这些 action 就判为缺失 |

## 核心兼容问题

### A01 · P0 · JCS 波长布局仍用推测格式，未对齐旧项目实际常量【差异】

旧 [pocketsconfig.h](../../../FOSD_Project/src/protocol/pocketsconfig.h) 第 20–22 行明确 `MAX_CHANNELS=16`、`DataAddress=24`；[analyzepackets.cpp](../../../FOSD_Project/src/protocol/analyzepackets.cpp) `processMain()` 从 `[4..19]` 读各通道数量，从 `[20]` 读总数，从 `[24]` 开始读 float32 波长。

新 [JCSWavelengthParser.cpp](../../src/protocol/JCSWavelengthParser.cpp) `parse()` 固定 **7 通道、总数 `[11]`、波长起点 `[12]`**；[PacketDecoders.cpp](../../src/communication/PacketDecoders.cpp) `JCS_Data_PacketRule()` 也固定相同布局。新解析器注释仍将原文件定义称为未确认假设，但原项目现在可直接提供这些定义。

影响：例如只有旧版 CH1 有 1 个传感器时，旧包 `[4]=1`、`[11]=0`、`[20]=1`，新拆包规则会因 `sum([4..10]) != [11]` 拒绝；某些巧合满足计数条件的包也可能从错误偏移取波长。仅修改 Parser 不够，Decoder 也会提前拦截。新 UI 虽提供 CH1–16，解析器实际上只会发布 CH1–7。

迁移：按真实 16 通道设备格式补齐拆包与解析；如确实需要保留 7 通道型号，使用明确的型号/协议布局配置和可区分规则，不以相同 magic 猜测。用旧源码可接受的包和真实抓包验证 CH1/7/8/16、空通道、总数、首末波长、长度及 TCP 粘拆包。另外旧解析接受尾部多余字节，新 Parser 虽允许 padding，UDP Decoder 却要求长度严格相等，应确认实际设备 padding 是否被前层误拒。现有 JCS 合成包通过不能证明这些布局兼容。

### A02 · P0 · Classic 的 TCP 被固定成客户端【能力已有、入口未接】

旧 [connectwindow.cpp](../../../FOSD_Project/src/app/conncect/connectwindow.cpp) `on_pbtn_OpenNet_clicked()` 将本地 IP/端口交给 `tcpOpen()`；旧 [tcptask.cpp](../../../FOSD_Project/src/Communication/tcptask.cpp) 执行 `QTcpServer::listen()`，设备作为客户端连接上位机。

新 [WorkbenchConnection.cpp](../../ui/FOSD-Classic-QWidgets/application/runtime/WorkbenchConnection.cpp) `pbtn_OpenNet` TCP 分支仅设置 `host=目标地址`、`port=目标端口`；[ITransportTypes.h](../../src/communication/ITransportTypes.h) 的 `TcpConfig::role` 默认 Client。底层 [Tcp.cpp](../../src/communication/Tcp.cpp) 已实现 Server，Classic 没有选择角色或设置 `listenAddress` 的路径。

迁移：恢复旧版默认“监听本地 IP/端口”的业务，或提供明确的客户端/服务器选择；显示监听中、设备已接入、设备断开和监听失败。验收必须从 Classic 窗口让模拟设备主动连接，并验证断开后再次接入。旧版新客户端替换旧连接、新版拒绝第二客户端的策略也应明确并比对。

### A03 · P1 · 置零从 20 帧平均变成单帧，并丢失基线文件恢复【差异/缺失】

旧 `analyzepackets.cpp:26,128` 构造时读 `data/calibration/初始波长.txt`；按置零后累计 20 帧求均值、写文件并发送完成信号。新 [WorkbenchRuntime.cpp](../../ui/FOSD-Classic-QWidgets/application/runtime/WorkbenchRuntime.cpp) `baseline():225` 直接复制最近一帧到 `originalWavelength`，不做平均或持久化。

影响：基线抗噪策略和重启后的工作流程不同；不能声称已经实测精度变差。迁移：恢复可配置的采样平均（兼容默认 20 帧）、采样进度/完成状态、数量与传感器身份一致性检查、基线保存/恢复。验收：输入已知波动的 20 帧，对照均值；采样中断/换设备不能混入同一基线。

### A04 · P1 · 通道筛选不再影响计算/保存，默认映射可能接错传感器【差异】

旧 `analyzepackets.cpp::processMain()` 使用 `channel_flags` 将选中通道压缩后写入 `NewStrain/strain_float`，IFE 再按 `order[i]-1` 取值。新 `WorkbenchConnection.cpp::pbtn_SelectCH` 只设置界面过滤集合，提示明确写着“原始数据继续完整解算和存储”；新 Worker 对完整 `records` 顺序标定。首次置零时默认选前 `min(12,N)` 个源，未读取旧 `ifepara.ini`。

影响：旧版取消某通道后，原先的传感器序号可能指向新的有效数据；新版同一序号仍指向原始全量帧。新 Presenter 的“全选通道”只修改勾选，旧版全选会立即提交，操作步骤也不同。旧 para 系数可直接设任意数值参与乘法，新 `StrainCalibration::checkPara()` 拒绝非正通道系数；若旧参数用负数反转方向或用零屏蔽输入，会无法导入。应核查实际 INI 并定义兼容规则，不能直接把所有非正值认作无效旧配置。源数不足 12 时新默认映射仍可提交，HYY 的 component() 对缺少分量补零；应明确是否允许这种模式，避免设备缺通道时静默得到不完整解算。

迁移：明确显示筛选和计算映射两个配置，导入旧配置时转换为稳定的 `(channel,sensor)` 身份映射；不要为追求表面一致破坏原始记录完整性。验收：同一输入、同一旧映射得到同一组 12 路应变；传感器数量变化、设备混流或记录重排时检测/阻止错误映射。

### A05 · P1 · 配置持久化和“恢复参数”语义未迁移【缺失/差异】

旧 `connectwindow.cpp:77` 的读写配置覆盖 A/B、阈值、采样显示频率和保存间隔；旧 [ConfigManager.cpp](../../../FOSD_Project/src/utils/config/ConfigManager.cpp) 有 JSON 加载/保存；旧 `IFE.cpp::loadConfig/saveConfig` 读写 12 路 order/para 和 axis；旧三维窗口保存所选模型路径。

新 Runtime 配置只存在成员变量中；没有接这些 JSON/INI/波长文件的读取、导入和保存。`pbtn_ParaRest` 直接设 A=1、B=0，旧版同名操作重新读取已保存配置。`m_modelPath` 只作内存记录，不能作为路径已持久化的证据。

迁移：建立有版本号的应用配置服务，覆盖上述旧字段及基线、映射；设置/恢复/出错反馈形成闭环，并提供旧配置迁移。验收：改值→应用→重启→恢复，损坏配置不覆盖有效会话；“恢复已保存”和“恢复默认”区分清楚。

## 尚未补齐的主要功能

| ID / 优先级 / 状态 | 旧版实际功能与证据 | Classic 当前缺口、迁移与验收重点 |
| --- | --- | --- |
| A06 / P1 / 缺失 | `disp2dwindow.cpp:82` 导入 `OriginalCoordiante.csv`，`IFE.cpp::modifyOriginalCoordinates` 对坐标减去 axis；`disp2dpara.cpp:90` 设置并保存 X/Y/Z 原点偏移 | `PlotPresenter` 只接参数窗打开，`dataAction()` 只接观察开关/传感器配置；`pbtn_ChangeOriginalCoordinates` 无处理。`publishCalibration()` 将 axis 三字段写为“无对应配置”并禁用。新二维表来自三维 `m_result`，最多前 256 个网格点，未加载 STL 时不能替代旧独立坐标观察。补独立观察坐标集、原点偏移、文件错误反馈和持久化；按导入行顺序/编号验证原始/位移/最终坐标，不要求先加载 STL。旧二维数组更新本身有不完整之处，需用明确预期值验收，不能把旧未初始化数组作数值标准 |
| A07 / P1 / 缺失 | `trackingpoint.cpp::LoadTrackingPointsFromFile/CreatTrackingPoint` 读取任意 id/name/x/y/z；旧 `disp3dfreshmodelthread.cpp:172` 对独立坐标调用 ShapeFunction；`trackingpointmanager.cpp` 和 `trackingpointdatatable.cpp` 实现导入、清空、显隐、自动刷新、行过滤及位移表 | `TrackingBackend` 最终走 unavailable；Runtime 无 TrackingManager/TrackingData 业务，`KeyPointDefinition` 仅绑定网格 pointId，不能替代任意空间坐标。迁移独立追踪点数据、计算、标记/标签、表格和文件操作；如接存储须区分网格点 ID 与追踪点 ID。验收至少使用不在 STL 顶点上的坐标，显隐和倍率不改变真实位移 |
| A08 / P1 / 缺失 | `disp3dmodel.cpp:286,312` 将 `rawPolyData` 导出为 STL、合并其他 STL；`disp3dwindow.cpp::MergeModel/ExportModel` 有文件交互 | `WorkbenchVisualization.cpp` 无 ModelMerge/ModelExport 分支；`MeshManager.h:14,15` 只有 optimize/exportModel 声明，无实现；`ModelPresenter::prepareRequest()` 仅允许 ModelLoad，内部 export 判定当前不可达。补对话框、MeshManager/Control 操作、回执、统计更新；合并后重建模型版本、清理选择/关键点和旧计算结果；导出默认应对齐旧版原始网格，如另加当前变形网格导出需显式区分，并检查再导入几何 |
| A09 / P1 / 缺失 | `disp3dwindow.cpp::SetModelColor` 经颜色对话框设置 Actor；旧颜色窗支持 RGB/滑条、预览及屏幕取色 | 新 `pbtn_SelectColor` 只打开表单，ColorPicker 的 accepted 不被 Runtime 处理；`VisualizationControl.cpp:113` 硬编码 RGB。补颜色模型与应用/取消、预览同步，优先做到实际修改模型；屏幕取色属旧辅助能力，后续补或明确裁剪。关位移着色后应恢复所选基础颜色 |
| A10 / P1 / 缺失 | `disp3dkeypointedit.cpp:141` 显示空间盒，随后对原始坐标包含边界判断，区域添加/移除关键点，并支持清理盒预览 | 新支持列表只有全选、清空、查找及部分增删/坐标模式，`pbtn_DispCube/SelectCube/ClearCube/pushButton` 无业务。补盒预览和批量操作，按原始坐标与边界包含规则验证，模型切换清除盒状态 |
| A11 / P1 / 差异 | 旧 `LoadModel()` 调用 `AllKeyPoint()`，候选 `keyPointSet` 与用户标记 `SelectedPointMap` 分离；Select 窗口 Add 从候选中添加观察标记，Delete 只移除标记，Edit 可以替换当前行 ID | 新 `KeyPointManager::setModel()` 清空全部定义，`publishModel()` 用同一 `keyPoints()` 填候选下拉框和观察表。加载后候选为空；Select 窗口 Add 只能取候选里已有 ID，再向同一集合去重添加，无法复现“从候选新增观察点”；Delete 修改的也是同一集合。`pbtn_EditPoint` 仅跳转编辑窗，没有替换观察行逻辑。应分离候选/计算点与观察标记集合，或者提供等价的完整选点流程；验证初载→选 ID→添加→替换→删除及两窗口同步 |
| A12 / P1 / 差异 | 旧观察标记文本显示坐标，拾取球半径随模型对角线缩放；`disp3dinteractor.cpp:83` 跟随选中点时设置相机 focal point，受相机锁约束 | 新底层有标记和拾取，但默认 label 为 ID、半径固定 mm；Scene 的 `cbox_FellowAble` 未处理。补相机跟随开关及与锁定的互斥/优先级，按旧行为至少在选点时聚焦；如需连续跟随另定语义。按小/大模型、多倍率检查标记可见性，并提供坐标/位移标签方式 |
| A13 / P1 / 差异/缺失 | `analyzepackets.cpp:319,355` 定时 `AutoSaveCycle` 保存最新波长与应变，手动保存也包含两者，旧 CSV 为 time + 各传感器宽表 | 新 `startStorage()` 连续模式固定三类数据且 allPoints=true，没有保存周期策略；单次模式只有 Wavelength，丢失应变。CSV 变为共享 schema 长表，旧下游脚本不一定可直接使用。补可选定时采样、包含相同源帧和配置版本的联合快照，以及格式兼容/转换说明；不能把分别到达的三类记录直接当成同一时刻快照。验收周期、单次不重不漏、追加、CSV/SQLite 内容、磁盘失败及退出排空 |
| A14 / P1 / 缺失 | 旧通信表列出 FBG 编码、波长、应变，并可按 StrainLow/Top 显示 outRange；频率从 CampingFre 控制表格刷新 | 新 `publishLatest()` 只发 channel/sensor/wavelength 三列，硬件窗仅显示 selectedStrains，不能替代全传感器的波长/应变联查。阈值字段被明确禁用；`cbox_ModleSelect`、CampingFre、SaveCycle 没有业务，显示定时器固定 100ms。补全量标定应变回传、模式切换、阈值显示和可配置刷新/保存周期；非算法输入范围校验不要冒充显示阈值业务 |
| A15 / P1 / 缺失/差异 | 旧 `InitSerial()` 绑定 serial/udp/fbg/start/reset/debug 六个预设命令；手动串口与 UDP 发送追加 CRLF（`connectwindow.cpp:521,533,856`） | Runtime 无 `pbtn_Cmd*` 分支；串口/UDP 手动发送直接 UTF-8，未追加终止符。设备若按行解析会无法执行命令。补预设命令及明确的行尾选项，兼容旧默认 CRLF，按字节核对六条命令和手动发送。TCP 旧版原文发送，应保持对应语义 |
| A16 / P2 / 缺失 | 旧 `disp2dwindow.cpp:108` 更换二维背景图片 | 新 View 仅发请求，Presenter/Runtime 无 `pbtn_ChangeBackGround` 文件选择、图片应用。补文件选择、缩放和失败回显，不要将背景切换判作计算模块工作 |

## 需限定范围或继续验证的项目

| ID / 分类 | 源码事实、结论及后续 |
| --- | --- |
| A17 / P2 / 条件恢复：频谱 | 旧 `processChannel()` 读取 X/Y，`Show_Plot()` 更新频谱，`on_pbtn_SaveSpectrum_clicked()` 导出 PNG/JPEG/BMP。新 `JCSChannelParser` 明确写“retired protocol publishes no data”，parse 返回空；默认 Decoder 不接该通道包，`cplot_Spectrum` 没有 Runtime 数据更新，SaveSpectrum 无业务。它是相对旧源码的明确功能差异，不能把空 parse 当普通漏写再盲目恢复。若本次“基本一致”包含频谱，应补完整谱数据链与图片导出，并按原 MAX_POINTS=5000/实际偏移验证；若产品已决定退役，应在界面隐藏/禁用入口并记录裁剪决定。当前用户没有在本次请求中明确裁剪该项 |
| A18 / P2 / 条件恢复：账户 | 旧登录/注册/找回/用户管理有具体代码及用户配置读写；但旧 `main.cpp` 登录启动已注释，实际直接进入权限 1 的主窗，不能说旧程序当前强制登录。新 Account 只有表单导航/清空与 unavailable，主窗 TurnOff 只显示登录面板；应决定保留当前免登录入口，还是恢复完整认证流程。若恢复，包含用户存储、登录会话、权限传递和退出登录闭环，不能只补密码比对；旧密码存储方式无需照搬 |
| A19 / P2 / 条件迁移：优化与其他算法 | 旧 OptimizeMesh 有实现，但 `LoadModel()` 中调用已经注释；新 optimize 仅声明。这是代码能力未迁移，不是旧默认加载体验确定退化。Rainbow/LoadCalibration 代码存在，旧 `main.cpp` 的 RainbowMainWindow 启动也已注释，不能自动列为 Classic 现役必须恢复；新 Plate 仿真不等价于旧 Rainbow 业务。若要覆盖这些模式需单独按对应入口验收 |
| A20 / P1 / 待验证：解算数值和单位 | 新 HYY 的形函数对应旧实际调用的 `ShapeFunction_New_ZB`，Worker 同样将 m 位移乘 1000 成 mm；不能因函数改名就判算法缺失。但 `defaultHyyPipeline()` 主要验零输入、结果有限和版本，不是旧版逐点数值基准。旧 `IFE.cpp::calculateDisp()` 循环 22 行而头文件矩阵仅声明 20 行，存在旧版越界读取，最后两自由度不能当可靠 golden。需要固定有效矩阵行和额外自由度约定，用同一已校准 12 路输入比较节点、x=0/700/1400 及边界附近、非零 y/z 的位移；独立计算器/明确公式作为标准，修复旧缺陷不算迁移回退 |
| A21 / P1 / 差异：暂停/停止/复位与统计 | 旧三维暂停会暂停解算线程和渲染刷新；新 actionPause/Stop 仅断开显示结果仓库，通信/解算/保存继续，m_result、表格与最大位移仍更新。新默认 m_running=true，旧通过 Start 启动。新最大位移取向量模最大值，旧 `disp3dfreshmodelthread.cpp:165` 取各分量绝对值最大值，例如 (3,4,0) 显示 5 而旧版 4。应明确运行状态作用范围并在界面反映；指标命名/算法统一或提供两项，防止拿新旧数值直接比较 |
| A22 / P1 / 待验证：高频与界面响应 | 新 Worker 10ms 轮询 latest，解算/标定/位移存储不保证逐个输入帧都处理；原始波长写入又走另一逐帧路径。UI 虽10Hz节流，frameParsed/calibratedReady/resultReady 仍进入 GUI 事件队列，且每帧整网格结果和全点写入可能昂贵。不能仅凭“最新快照”宣称丢帧受控或不会卡顿。测真实最大帧率/模型规模下的 GUI 延迟、CPU/内存、各类记录频率和丢帧指标；明确原始记录与派生记录采样策略 |
| A23 / P2 / 待验证：模型路径、外观、交付 | 旧 STL 路径用显式 GBK，新用 QFile::encodeName；中文/空格路径、二进制 STL、不可读/空模型需要实际检验。新视角/网格/轴虽有实现，不能从源码证明渲染外观和交互手感一致。旧 ARMLinux 分支有 RK3588/Wayland 启动、打包与数据根路径约定，新以 Windows preset 为主；若目标包括 ARM/Linux，另做平台部署验收，不把 Windows 测试视为跨平台交付证据 |
| A24 / P2 / 入口分歧 | 默认 `windows-msvc2017` preset 构建 `apps/FOSD-Win/FOSD.cpp`，只创建 Protocol/Calculation，没有 Classic Runtime/Services 与三维组合；它不能复用 Classic 完成度结论。若正式交付使用 Classic，需统一启动/发布配置和文档；若仍发布 FOSD-Win，则其业务接线本身仍是独立缺项 |

补充边界：旧 `disp3dwindow::on_actionBackData_triggered()` 主体已注释，旧温度/应变窗虽有数据读取代码，不能认定当前旧主入口完整可用；新已接的硬件表是有效进展。旧标量初始化包含占位场，不能把缺少真实温度/节点应变提供者当成已完成旧业务的回退。主界面导航/主题、全选通道的本地处理、参数窗取消都已考虑，没有仅以 Runtime 缺分支判缺失。

## 本轮运行证据及其限制

2026-11-03 在已有 `build/fosd-widgets` 构建目录执行：

```powershell
ctest --test-dir build/fosd-widgets -C Release --output-on-failure --timeout 25
```

结果 **14/15 测试目标通过**。通过项：classic_widgets、workbench_runtime、workbench_main、plate_simulation、communication、packet_decoders、protocol、jcs_wavelength_parser、jcs_channel_parser、calculation、storage、end_to_end、visualization、scene_environment。`test_keypoint_interaction` 失败；CTest 没有捕获具体文本后，另用 QtTest `-o` 输出，见 [keypoint-interaction-existing-release.txt](keypoint-interaction-existing-release.txt)。

两处失败是 `managerLifecycle()` 在 `setModel({},nullptr)` 后渲染器仍有 4 个 prop，以及 `controlDeformationAndReset()` 在 unload 后 keyPoints 非空。**该 exe 时间为 2026-10-27 10:33:41，早于当前审计；当前 `KeyPointManager::clear()` 源码已包含摘除 actor 和清空 definitions 的处理。不能将旧二进制失败直接归因当前源码。** 环境有 ctest，但所查 CMake bin 中没有 cmake.exe，未完成当前源码重新构建。所有本轮通过结果也只能表述为已有构建产物测试，不能充当本次最新源码编译通过证明。

现有 `workbench_runtime_tests::unavailableUsesFeatureBackend()` 还明确期待预设串口命令、模型合并、相机跟随、颜色确认、追踪文件和登录请求失败；因此测试通过本身包含“正确报告未实现”，不等于这些功能已经迁移。`workbench_main_tests::completeUiPipeline()` 覆盖的 Plate/UDP 合成数据主链也不能替代旧 16 通道协议、TCP Server、真实 HYY 输入和上述完整功能验收。

## 实施顺序与验收门槛

1. **先恢复接入与输入兼容（A01–A05、A15）**：实际 16 通道布局→Classic TCP Server→命令行尾→20 帧置零/配置导入→稳定传感器映射。以旧设备抓包和旧参数文件建立可重复输入，逐项核对身份、波长和应变。这一步未过，后续变形演示没有等价性意义。
2. **补齐主要工作流（A06–A14）**：独立坐标观察/追踪点→关键点候选与标记分离→模型合并/导出/颜色/区域选择/跟随→全量应变与采样保存。通过 Service/Control 接口完成业务，不回到 View/Presenter 中执行 IFE 或直接读写共享全局数组。
3. **固定语义与交付（A16–A24）**：确定频谱/账户/可选模式是否保留；统一统计、暂停/复位、CSV兼容和发布入口；当前源码重建后重跑测试并检查关键点清理；最后做真机、性能、跨平台（如需）验收。

建议最低验收记录表：

| 验收场景 | 必须留存的证据 | 通过标准 |
| --- | --- | --- |
| 原设备 JCS/串口/UDP/TCP | 抓包、发送/接收字节、布局配置、解析记录 | CH1–16及边界包正确，原 TCP 设备可主动接入，命令字节/行尾一致 |
| 基线与旧配置恢复 | 20帧原始输入、均值、JSON/INI、重启前后配置 | 基线平均与恢复正确，同身份传感器映射和标定一致 |
| HYY 数值等价 | 固定12路应变、节点与空间点坐标、可信参考输出 | mm/m、方向、边界与分量在事先约定容差内；排除旧越界未定义行为 |
| 模型/关键点/追踪点 | 操作步骤、导出STL、坐标与ID/版本、截图 | 重载/合并/卸载无旧标记和旧结果残留，任意坐标追踪正确 |
| 保存 | 同源帧三类数据、定时/单次文件与统计 | 不混配置/模型版本，周期与单次策略明确，旧消费方式有兼容方案 |
| 会话与稳定性 | 暂停/停止/复位记录、断线重连、异常文件与持续负载日志 | 状态语义一致且可见，无事件无限积压/未排空退出；测量结论有实际数据 |

达到“基本一致”的条件应是：P0 全部关闭，主要工作流 P1 逐项验收，对保留/裁剪项有明确记录，并用当前源码生成的程序完成真实输入对比。本文是待办与验收基线，不代表已完成这些迁移。
