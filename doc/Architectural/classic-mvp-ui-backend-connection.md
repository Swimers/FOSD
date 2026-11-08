# Classic QWidgets 三层 MVP 架构与迁移审阅

更新日期：2026-11-08。当前调用链为 View → Presenter → WorkbenchModel → src 业务 Control；旧五层连接链已删除。

## 当前职责

| 层 | 入口 | 职责 |
| --- | --- | --- |
| 装配 | application/ClassicWorkbench | 创建 Model、View、Presenter，连接 Qt/VTK 宿主，管理销毁次序 |
| View | features/*/views、common/FormBinding | Designer 表单、输入采集、主题与控件回填；ConnectionRenderers 负责曲线、图片导出和彩色两行日志 |
| Presenter | application/WorkbenchPresenter、features/*/presenters | 解释控件操作、转换参数、处理文件/颜色选择、推导控件状态和展示数据，直接调用 Model |
| Model | model/WorkbenchModel、model/WorkbenchMarkers | 业务编排、配置/模型版本校验、置零、连接、显示和存储状态；调用 src 中的 Control |

WorkbenchPresenter 是 Presenter 的装配与刷新协调器，不是新的业务转发层。各功能 Presenter 直接持有同一个 WorkbenchModel，通过明确的业务方法调用，不再发送通用请求信封。

## 已完成的迁移

1. 通道标定：ConnectionPresenter 将源序号、倍率和偏置转换成 CalculationConfiguration，直接调用 applyCalibration；Model 等工作线程确认后才替换有效配置并发出 configurationApplied。
2. 通信和保存：串口参数、TCP Server/Client、UDP、快捷指令及保存操作改为 Model 业务方法。串口 CRLF、配置恢复、错误反馈继续保留。
3. 三维和状态：ModelPanelPresenter 负责模型与关键点界面，VisualizationPresenter 负责场景、选点状态栏和工具栏。模型是否可用直接取自 Model 状态，不再从 lab_NumPoints 等展示字段反推。
4. 清理：删除各 features/backends、WorkbenchBackend、IBackend、FeatureBackend、WorkbenchServices、UiRequest/UiResponse、FeatureId、旧 application/runtime 及相关请求注册逻辑。测试改为注入 Model；不保留旧 Backend 兼容桥接。

Model 不包含控件名称、PanelId、QVariantMap 展示协议、颜色/坐标排版或 QWidget 操作。控件名和 QVariantMap 仅用于 View/Presenter 之间的本地绑定。

## 主要业务入口

| 操作 | Model 方法 | 确认方式 |
| --- | --- | --- |
| 标定与置零 | applyCalibration、loadCalibration、baseline | configurationPending、configurationChanged、configurationApplied |
| 通信 | openSerial/openUdp/openTcp、closeChannel、send、sendSerialCommand | connectionState 与 stateChanged；发送日志表示请求已提交，不代表下位机应答 |
| 显示 | loadModel、clearModel、setDisplayState、resetDeformation | hasModel、modelPending、DisplayState |
| 保存 | setRecording | RecordingState 的 Starting/Recording/Stopping/Stopped，实际 started/stopped/error 回调驱动状态 |
| 点集合 | add/removeKeyPoint、updateKeyPointsInBox、upsert/removeDisplayedPoints 等 | typed 参数调用、业务校验及 dataChanged |

重复配置提交由 Model 拒绝，Presenter 在配置未确认时禁用应用按钮。错误配置不覆盖有效配置；模型请求保留 generation/revision 检查，旧模型结果不能覆盖新模型。

## 刷新与线程

- 通信/协议、解算、存储的 Control/Worker 线程机制保持不变；View、Presenter、Model 协调及 VTK 操作仍在 GUI 线程。
- WorkbenchPresenter 每 100 ms 刷新展示快照，ConnectionPresenter 只刷新新帧和增量日志。
- 曲线每条保留 300 个点；日志每种通道最多 200 帧、载荷预览最多 64 字节/64 字符，每 60 秒清理；有效 UDP 包继续合并摘要。
- 坐标两位小数、SelectedPoint 固定宽度、深浅主题、曲线缩放和取点均保留。
- 无模型时的禁用规则由 Presenter 根据 hasModel 推导；底层环境接口仍可独立调用。

## 构建与审阅

构建目标为 contracts、controls、widgets（View）、model、presenters、application。widgets 不链接 Model/Presenter，presenters 显式依赖 Model；完整 application 才装配各层及 Qt/VTK。apps/FOSD-Classic-QWidgets 仍只包含启动入口与 EXE 部署配置。

测试分为控件、Model/Presenter、点集合和完整工作台。test_workbench_runtime 已更名为 test_workbench_model；主界面 QtTest 将结果写入构建目录 ui/FOSD-Classic-QWidgets/tests/workbench-main-test.txt，便于 Windows GUI 测试失败时定位。

审阅重点：Model 不依赖界面字段；Presenter 不直接调用 Control；保存/标定不能把“已提交”误判为“已生效”；销毁时先解除 VTK 连接，再停止业务线程。FOSD-Win 保留其原有未装配完整业务的入口行为，仍可使用导航、主题和基础界面，不再构造空 Backend 注册表。

历史审阅与迁移记录保留当时的架构描述，当前实现以本页与源码为准。

## 本次验证结果（2026-11-08）

| 检查 | 结果 |
| --- | --- |
| Classic QWidgets Release | 编译通过，输出 build/fosd-widgets-package/bin/Release/FOSD-Classic-QWidgets.exe |
| FOSD-Win 兼容入口 Release | 编译通过，输出 build/classic-layout-compat/bin/Release/FOSD.exe |
| 完整 CTest 回归 | 18/18 通过，记录见 build/fosd-widgets-package/mvp-ctest.log |
| 最终审阅修复后的前端回归 | Model、点集合和完整工作台 3/3 通过，记录见 build/fosd-widgets-package/mvp-review-ctest.log |
| 静态依赖与改动边界 | 生产代码已无旧转发链引用；Model 无界面字段依赖；Presenter 不直接调用 Control；src 无改动；git diff --check 通过 |

回归覆盖 UDP/MP 解包回环、TCP Client/Server 分包与粘包、串口配置校验及 CRLF 指令、12 路倍率/偏置读写、配置异步确认与重复提交拒绝、均值置零、解算启停、CSV/SQLite 保存、曲线筛选、日志模式与清理、关键点及显示点、无模型禁用、工具栏互斥、颜色/渐变和 SelectedPoint 对齐。完整工作台 QtTest 为 12 项（含初始化和清理），均通过。

最终审阅修复了关键点数量在三维信息面板中未随集合变化更新的遗漏，并增加数量变化及清空回归断言；另检查了没有业务 Model 的预览入口，避免打开标定等基础界面时访问空对象。截图复核了标定面板和三维选点界面的深色主题、偏置输入及坐标显示。

串口验证使用配置与发送链路测试，尚未连接真实串口设备进行硬件联调；网络测试使用真实本机回环套接字。自动化通过不能替代目标设备上的最终联调。
