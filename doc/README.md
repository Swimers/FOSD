# 开发文档入口

| 文档 | 适用场景 |
| --- | --- |
| [Classic 三层 MVP 迁移与审阅](Architectural/classic-mvp-ui-backend-connection.md) | 当前 View/Presenter/Model 分工、直接业务调用、异步状态与验证入口 |
| [Classic 通信日志与通道标定改进（2026-11-07）](Classic-Communication-Logs-Calibration-2026-11-07.md) | 本地地址刷新、曲线筛选范围、三种通信的彩色收发日志、二维页面删除及 channlCalibration 迁移 |
| [Classic 通信页面改进与验证（2026-11-07）](Classic-Connection-UI-2026-11-07.md) | Designer 布局、TCP 模式、完整串口参数、保存状态与曲线交互/主题的接入和验证 |
| [Classic 前后端功能闭环审阅（2026-11-07）](FOSD-Classic-QWidgets-Backend-Audit-2026-11-07.md) | 参数设置覆盖、配置恢复缺陷、诊断/设备应答缺口、各面板完成度和建议验收清单 |
| [Classic 迁移实施与验证](MigrationCompletion-2026-11-04/README.md) | 当前 INI、16通道JCS、HYY、功能裁剪与验证记录 |
| [核心参数与运行时 INI](Architectural/core-parameters.md) | 统一参数结构、只读默认模板、构建部署、各模块接入和校验 |
| [INI 配置读写](Architectural/config-integration.md) | 模块分区、统一读写接口、原子保存、并发访问、错误处理与测试 |
| [Classic 版本业务闭环对接](Architectural/formal-workbench-integration.md) | Classic 独立入口、MVP 业务注入、完整数据流、已接控件/不可用功能、历史界面验证（当前范围见实施记录） |
| [迁移完成度核查与后续计划](Architectural/migration-audit-and-plan.md) | 正式入口缺口、模型管理、独立追踪点、配置与会话、观察面板及分阶段验收 |
| [场景环境与相机](Architectural/scene-environment-integration.md) | 坐标轴、三平面网格、背景与纹理世界、相机视图/投影及验证 |
| [关键点、选中点与持续显示点](Architectural/keypoint-picking-integration.md) | 三类集合约束、外部固定点、Classic MVP 编辑、球/文本随动、拾取与测试 |
| [可视化迁移与标量映射](Architectural/visualization-integration.md) | 分步迁移安排、X/Y/Z/总位移着色、归一化与物理值、通用点标量入口及测试 |
| [独立构建预设](build-presets.md) | 主程序、FOSD-Classic-QWidgets 完整版本及通信助手的配置、构建、测试与运行 |
| [主工作台 Widgets 与 MVP](../ui/FOSD-Classic-QWidgets/README.md) | 旧版 19 个表单、资源、View/Presenter、后端请求及三维宿主对接 |
| [解算与模型对接](Architectural/calculation-integration.md) | 波长快照、标定、模型输入、逐点插值与只读结果输出 |
| [数据存储对接](Architectural/storage-integration.md) | 解算线程托管、CSV/SQLite、数据类型及点位选择、错误处理、扩展与测试 |
| [模块构建结构](build-structure.md) | CMake 目标、依赖、构建命令与新增模块 |
| [协议与共享数据对接](protocol-integration.md) | communication → protocol → Datamodel 数据流，storage/visualization 对接，协议扩展与测试 |
| [QML 三合一通信助手](communication-assistant-QML.md) | QML/MVP 结构、原始字节收发、独立构建、部署与测试 |
| [QWidgets 通信助手](communication-assistant-widgets.md) | 复用 Presenter 的 Widgets 前端、独立构建和界面测试 |
| [波长协议参考解析代码](QT解析代码.txt) | 当前 `0xA55A` 波长数据的字段与解码依据 |
| [参考收包代码](QT解析格式.txt) | 原始 UDP 接收和解析调用示例 |

接口行为以当前源码为准。新增协议或改变共享数据接口时，同步更新对接文档和对应测试。
