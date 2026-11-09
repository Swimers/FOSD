# Classic QWidgets 架构迁移后的冗余路径审阅与清理

审阅及执行日期：2026-11-09。A1–A10 已按本报告完成清理，10 个 View 的 96 个控件专用 Requested 信号与 relayControl 转发已移除；通用事件链保持为 FormBinding → ViewEvents::actionTriggered → PanelPresenter::execute。B1–B8 仍为待评估项，本轮未处理。

执行基于当前工作树，保留了原有未提交的修改。清理范围为前端声明、实现及 MemoryView 测试替身；没有修改 src 后端、Designer 表单、CMake 目标或旧 FOSD 入口。原审阅使用符号引用、分支可达性、表单连接与构建依赖交叉核对，不按普通调用次数删除 Qt 回调或第三方公开 API。

## A1–A10 清理执行记录

| 编号 | 当前源码位置 | 执行结果 | 保留功能 |
| --- | --- | --- | --- |
| A1 | [ConnectionView 事件源](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/views/connectwindow.cpp:36) | 已删除 10 组 connect、relayControl 声明/实现及 96 个专用信号。 | 保留所有 View、FormBinding 绑定及 Presenter 的通用订阅。 |
| A2 | [ConnectionPresenter::localAction](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.cpp:23) | 已删除 handleAction 中不可到达的 pbtn_ChannelCalibration 分支。 | 保留 localAction 打开标定窗口、回填和取消逻辑。 |
| A3 | [WorkbenchPresenter 当前接口](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/application/WorkbenchPresenter.h:9) | 已删除根 Presenter 的 execute(PanelId, action) 声明和实现。 | 保留 PanelPresenter::execute、panelPresenter() 及刷新协调。 |
| A4 | [ModelPanelPresenter 当前接口](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/visualization/presenters/ModelPanelPresenter.h:6) | 已删除空 poll 覆写的声明和实现。 | 继承基类默认 poll；实际数据刷新仍通过 refresh 处理。 |
| A5 | [FormBinding 当前接口](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/FormBinding.h:12) | 已删除未使用的 widget() 公开访问器。 | 保留成员 m_widget 及其控件绑定功能。 |
| A6 | [IView 当前接口](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/IView.h:9) | 已删除 IView/FormBinding 的 clearInputs、10 个 View 覆写及 MemoryView 测试替身覆写。 | 保留日志 clearTraffic、关闭面板的 clearSensitiveFields。 |
| A7 | [WorkbenchModel 选点查询](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/model/WorkbenchModel.h:172) | 已删除 selectPoint、clearSelection、nearbyPoints 三个未使用的 Model 包装。 | 保留后端 API、交互选点、selectedPoint 查询及现有后端集成测试。 |
| A8 | [renderSpectrum](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/views/ConnectionRenderers.cpp:11) | 已删除旧 x/y 单曲线回退分支及不再使用的 cmath 引用；未知字段忽略。 | 保留 series 实时曲线与 savePath 图片导出。 |
| A9 | [WindowAction 当前枚举](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/UiTypes.h:22) | 已删除 Close 枚举项及 WorkbenchView 的对应分支。 | 保留 closePanel 及当前关闭按钮处理。 |
| A10 | [PanelPresenter::localAction](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/PanelPresenter.cpp:103) | 已删除 btn_Min、btn_Close 两个旧控件名别名。 | 保留当前窗口按钮名及 rejected、pbtn_SenerCancel 等现用/未纳入本轮清理的路径。 |

## 可以合并或有条件裁剪的路径

| 编号 | 位置与证据 | 冗余及处理建议 | 条件或风险 |
| --- | --- | --- | --- |
| B1 | [configurationChanged 订阅](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.cpp:74)；[publishCalibration](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.cpp:800) | Connection 与 ChannelCalibration 各有一个 Presenter，二者都订阅 configurationChanged，且各自又同时回填两个面板。同一次通知会重复回填。可改为各自只回填自己的面板，或让一处负责广播。 | 应保留标定窗口每次打开时恢复有效配置的行为；删除重复订阅前须保留首次初始化和配置异步确认后的回填。 |
| B2 | [解算启停的重复更新](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/connection/presenters/ConnectionPresenter.cpp:208) | setCalculationRunning 已发出 stateChanged，refresh 已写按钮状态；handleAction 又回填同样状态。m_model->notice(message) 与基类 statusChanged 也会重复通知主状态。可删除这组成功路径的重复回填/通知。 | 不能把所有 updates 都删除：保存按钮失败时需要回滚 checked 状态，通信方式切换失败也需要恢复真实选项；这些是有效纠偏。 |
| B3 | [publishMarkers / publishDisplayedPoints](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/features/visualization/presenters/ModelPanelPresenter.cpp:262) | KeyPointEdit 和 KeyPointSelect 两个 Presenter 都计算面板数据和显示点行，最后由 presentOwn 丢弃不属于自己的结果。可先按面板判断，再计算相应字段，避免无用表格构造。 | 不能删除真实的显示点刷新、位移/原坐标切换或选择 ID 保持逻辑。 |
| B4 | [FormBinding 通用字段协议](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/FormBinding.cpp:133) | _status、append、dataValue、.selectedRows 当前无相应生产者或消费者；QDialogButtonBox 的 accepted/rejected 绑定也无现存表单实例。可按“只支持现有界面”的目标成组裁剪。 | 属于通用能力收缩，不是运行错误。保留 showStatus、日志 entries 渲染和用于安全删除显示点的 .selectedIds。若希望继续提供通用绑定库，也可保留并记录协议。 |
| B5 | [closePanel 的清理钩子](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/application/WorkbenchView.cpp:141)；[clearSensitiveFields](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/FormBinding.cpp:242) | 调用链真实存在，但当前表单没有密码型 QLineEdit，所以通常无实际清理对象。可移除整个能力，或改成 IView 默认空实现以减少 View 转发。 | 不应列为“没有调用者”。若后续仍希望关闭时清理敏感输入，建议保留该能力。 |
| B6 | [FOSD-Win 旧应用入口](D:/Documents/GitHub/FOSD-ReArc/apps/FOSD-Win/FOSD.cpp:6) | 旧入口构造 ProtocolControl/CalculationControl，却给 WorkbenchPresenter 传空 Model，业务控件仍走“业务服务尚未接入”。存在旧预览入口与完整 Classic 两条启动路径。 | 不可直接删除：顶层 FOSD_BUILD_MAIN_APP 默认 ON，windows-msvc2017 预设仍构建它。需先决定保留预览、迁移到 ClassicWorkbench，还是正式下线该目标，并同步预设和启动配置。 |
| B7 | [ViewFactory 的 Main 分支](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/common/ViewFactory.cpp:35) | 当前唯一调用方在 Main 时直接使用 this，因此工厂 Main 分支未被生产路径使用。可以裁剪。 | 若保留 createPanelView 的完整枚举工厂契约，该分支也合理；不是优先清理项，不能删除实际 MainWindowView。 |
| B8 | [ClassicWorkbench 析构](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/application/ClassicWorkbench.cpp:24)；[WorkbenchModel 析构](D:/Documents/GitHub/FOSD-ReArc/ui/FOSD-Classic-QWidgets/model/WorkbenchModel.cpp:90) | 应用、Model、VisualizationControl 析构均有 detach，存在重复清理。可统一主要解绑职责。 | 当前 detach 有无宿主检查。删除前必须确认 RenderWindow、QVTK 控件和 Model 的销毁次序；建议本轮先保留，不与普通死代码一起删除。 |

## Requested 转发的完整范围

下表是 A1 清理前的展开清单，所列信号现已移除；不包含仍在使用的 Model::sendRequested 和通信 Control 请求信号。

| View 表单 | 控件专用信号数 |
| --- | ---: |
| connectwindow | 36 |
| channlCalibration | 2 |
| oldmainwindow | 7 |
| dispdata | 1 |
| disp3dwindow | 8 |
| disp3dinfodockpanel | 5 |
| disp3dmodeldockpanel | 9 |
| disp3dviewdockpanel | 10 |
| disp3dkeypointedit | 8 |
| disp3dkeypointselect | 10 |
| 合计 | 96 |

InfoPanel 中 radioButton 等名字现在对应 QLabel，并非可点击按钮；其旧 Requested 路径也无法由当前表单产生。不应因此删除这些仍显示节点数等标题的 QLabel。

## 应保留的基础设施

| 对象 | 保留原因 |
| --- | --- |
| ViewEvents、IView、FormBinding | 事件契约与 QWidget 绑定实现分离；MemoryView 测试直接提供事件源。精简控件专用 Requested 不等于合并或删除通用 actionTriggered。 |
| QAbstractButton::clicked 等控件绑定 | 它们是输入进入 Presenter 的实际路径。SwitchControl 继承 QWidget，不会与 QAbstractButton 绑定重复；曲线自动范围的 View 交互也有实际展示作用。 |
| FormBinding::owns、QSignalBlocker | 分别防止嵌套面板串事件，以及回填状态再次触发业务操作。 |
| poll 定时器与 baseline 定时器 | 一个更新展示，一个负责置零采样超时，不是同一业务的重复计时器。 |
| panelPresenter、actionFinished、rawSent 等测试入口 | 当前测试使用 panelPresenter 查找 Presenter、观察 actionFinished，并通过 QMetaObject 调用 rawSent；不能只按生产调用数删除。 |
| Model 的 protocol/calculation/visualization 访问器 | 测试和装配仍实际使用；可考虑以后缩小暴露面，但必须先迁移调用者。 |
| 模型/配置 revision 与 generation 检查、pending 状态 | 保护异步确认和旧结果过滤；前端按钮禁用不能代替这些业务检查。 |
| Qt 控件事件覆写、Q_INIT_RESOURCE | paintEvent、resizeEvent 等由 Qt 调用，资源初始化用于静态库资源注册；没有普通函数调用不代表无用。 |
| 旧控件名的否定断言 | WorkbenchTests 中的 Diagnosis、SaveStatic、2DPlot 等名称用于验证已删除控件没有重新出现，并非旧功能实现。 |

旧 FeatureBackend、WorkbenchBackend、WorkbenchServices、UiRequest/UiResponse、WorkbenchRuntime、诊断模块及二维窗口实现没有重新出现在当前生产目标中。不要根据历史文档或上述否定断言再次执行源码删除。

## 执行验证与后续清理

A1–A10 已完成；B1–B4 后续按行为边界评估，B6 单独作入口迁移决策，B8 单独核对生命周期。不要按名称后缀批量删除所有 Requested 信号，也不要按引用次数机械删除 Qt 回调。

本次已针对清理后的源码完成下列验证：

| 检查 | 结果与记录 |
| --- | --- |
| 重新配置 fosd-widgets | 通过；build/fosd-widgets/a-cleanup-configure.log |
| Classic Release 完整构建 | 通过；build/fosd-widgets/a-cleanup-build.log |
| FOSD-Win 兼容入口 Release 构建 | 通过；build/classic-layout-compat/a-cleanup-build.log |
| Release 完整 CTest | 18/18 通过，用时 55.82 秒；build/fosd-widgets/a-cleanup-ctest.log |
| 完整工作台 QtTest | 12 项通过（含初始化和清理），0 失败；build/fosd-widgets/ui/FOSD-Classic-QWidgets/tests/workbench-main-test.txt |
| 清理边界 | A 类旧入口已无源码引用；通用事件订阅、日志发送信号及 16 项保留行为检查通过；src、Designer 表单与 CMake 目标未修改 |
| 改动差异 | 对清理前快照核对 34 个文件，净删除 675 行，未引入新的空白错误；原有用户修改保持在工作树中 |

在现有 GUI 回归中增加了有实际行为意义的断言：刷新本地地址的按钮操作只完成一次且返回成功；未知曲线字段保持多曲线数据与手动视口；点击保存曲线可生成非空且可解码的 PNG，导出后曲线数据保持不变。

完整回归还覆盖标定窗口打开与取消、倍率/偏置设置、异步保存成功与失败回滚、通信模式、串口 CRLF 指令、UDP/TCP 回环、日志清理、显示点选择与删除、深浅主题、曲线拖拽缩放及选点、无模型禁用、三维工具栏和销毁。真实串口设备尚未做硬件联调。

后续处理 B 类项目时，应重新构建和运行对应回归；本次通过的测试不能替代后续修改后的验证。
