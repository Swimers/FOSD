# 迁移完成度核查与后续计划

> 历史计划：Test-VtkOpenGLNativeWidgets 已于 2026-11-09 移除；当前主工作台为 FOSD-Classic-QWidgets，构建与测试以构建预设文档为准。下文保留原核查时的状态。

核查日期：2026-10-26。依据当前重构项目和旧版 `D:/Documents/GitHub/FOSD_Project/src` 的实际源码、入口及调用关系。此文是源码核查与迁移计划，不代表本轮完成了运行测试或真实设备验收。

后续更新：本轮 MVP/Control、QVTK、保存交互及全链路测试已在 FOSD-Classic-QWidgets 分版本完成，见 [Classic 业务闭环对接](formal-workbench-integration.md)。FOSD-Win 的入口及源码保持原状，下文对该入口的缺口分析仍作为后续参考。Plate IFE 已迁入共享 calculation，发送器仍在 tools。

## 结论

通信到结构化数据、HYY 调用、模型插值、CSV/SQLite 写入，以及三维变形、位移着色、关键点、选点和场景环境已经有实现；完整组合目前主要在 Test-VtkOpenGLNativeWidgets 中使用。正式 FOSD 主界面的 Backend 仍以占位适配为主，产品级操作闭环尚未完成。

需要区分“底层尚无实现”“能力已有但正式界面未接入”“按原定范围保留为扩展”。不能以表单、接口声明或测试入口存在作为正式功能完成的依据，也不能把已明确弃用的功能列为必须恢复。

## 现状及主要缺口

| 功能 | 当前状态与证据 | 旧版依据 / 后续内容 |
| --- | --- | --- |
| 正式主程序闭环 | `apps/FOSD-Win/FOSD.cpp` 创建 ProtocolControl、CalculationControl 并连接原始帧写入。默认七类 Feature Backend 均调用 reportUnavailable；主程序没有实例化 VisualizationControl 或挂载真实 QVTK 控件 | 接入通信、标定、模型、可视化、数据保存及状态回显，独立验收正式入口 |
| 模型优化、导出、合并 | `MeshManager.h` 声明 optimize/exportModel；`.cpp` 没有定义，调用会产生链接错误。当前只有 STL 加载和清理；没有合并及对应 Control 接口 | 旧版 OptimizeMesh / ExportModel / MergeModel，需要复现并定义点 ID 和版本失效规则 |
| 基础模型颜色 | `VisualizationControl::loadModel` 硬编码 Actor RGB；无统一模型颜色配置。位移色表功能已经实现 | 旧版 setModelColor / 颜色选择器；补基础颜色。透明度、表面/线框等可统一设计，作为按需扩展 |
| 关键点编辑和模型统计 | 批量 ID 配置、单击拾取、邻域查询及标记随动已有；正式编辑窗 Backend 为空，没有旧版区域盒选、批量编辑操作和模型信息面板的数据适配 | 旧版 disp3dkeypointedit 的 SelectAll / SelectCube，以及 GetModelPointsNum / GetModelPolysNum / GetModelSize |
| 相机跟随目标 | 视图、投影、姿态、锁定和适配已有；没有与选中点/追踪点联动的跟随模式 | 旧版 CameraFellow / Flag_CameraFellow；不是重复实现当前的相机锁 |
| 任意坐标追踪点 | 当前 KeyPointDefinition 只有模型 pointId 和 label；CalculationWorker 只遍历原始模型点。Storage 的 PointDisplacement 也仅访问该模型点位移字典 | 旧版 trackingpoint 支持 id/name/x/y/z 文件导入、独立位置插值、标记、位移标签和数据表，不可直接等同当前模型关键点 |
| 二维观察、硬件数据表 | 表单、QCustomPlot 控件和字段适配存在；PlotBackend / TrackingBackend 未接业务。CalibratedData 由 Worker 直接送 Storage，Control 未公开其只读观察出口 | 接原始/位移/最终坐标观察、标定参数编辑、应变/硬件数据显示及有限长度历史曲线；不要在 Presenter 重算标定或 IFE |
| 运行状态及参数持久化 | `src/config`、`src/logging` 无实现。计算配置和模型输入接口已有，但没有公开计算暂停/恢复等完整会话状态。resetDeformation 后下一新结果仍会恢复变形 | 旧版 ConfigManager、标定/采样/存储间隔/模型路径，以及 Start/Pause/Stop/Reset 操作，需要明确各自作用对象 |
| 保存业务交互 | CSV/SQLite、三类数据、点选择与 start/stop/flush 已实现；没有旧版定间隔采样保存、手动单次快照的统一策略和正式保存界面适配 | 旧版 analyzepackets::saveAuto/saveManual；现有逐帧写入可保留，采样策略应另设一层 |
| 账户与工程收尾 | AccountBackend 尚未接登录、注册、找回和账户管理；主程序错误日志、部署及正式入口验收仍需补充 | 旧账户操作与角色流程按产品需求复现；诊断日志和部署属于工程补全 |

### 直接证据位置

- [主程序组合入口](../../apps/FOSD-Win/FOSD.cpp)。
- [Workbench 默认 Backend 注册](../../ui/FOSD-Classic-QWidgets/application/WorkbenchBackend.cpp) 与 [FeatureBackend 的未实现反馈](../../ui/FOSD-Classic-QWidgets/common/FeatureBackend.cpp)。
- [模型管理声明](../../src/visualization/mesh/MeshManager.h) / [当前定义](../../src/visualization/mesh/MeshManager.cpp)。
- [可视化 Control](../../src/visualization/VisualizationControl.h) / [模型加载及运行实现](../../src/visualization/VisualizationControl.cpp)。
- [模型与计算结果结构](../../src/core/Datamodel/ModelData.h)、[计算 Worker](../../src/calculation/CalculationControl.cpp)、[对外接口](../../src/calculation/CalculationControl.h)。
- [存储配置](../../src/storage/StorageTypes.h) 与 [位移写入的数据来源](../../src/storage/StorageRecords.cpp)。

## 分阶段实施

### 阶段 1：正式主程序的最小生产闭环（P0）

在应用组合层组织生产 Control 和服务，保持模块独立。可以提取验证程序中可复用的组合逻辑；板仿真、示例球体及冒烟检查仍留在 tools/验证程序，不移入生产业务。

1. 在正式渲染宿主挂载 QVTKOpenGLNativeWidget，按现有约定设置 QApplication 前的 OpenGL 格式，传递同一 vtkRenderWindow 给 VisualizationControl。
2. 给 Connection / Model / Visualization / MainWindow Backend 注入同一套 Control；Backend 负责命令和值类型适配，Presenter 保留界面组织职责。
3. 接通通信开关和状态、有效标定配置、模型提交及 modelAccepted 版本、结果 Repository、渲染设置、保存路径/类型和 Storage 状态；补齐统一错误回显。
4. 为硬件数据显示提供只读 CalibratedData 快照出口或独立 Repository，不能依赖“已导入三维模型才会产生的 CalculationResult”。保留 Worker → Storage 的现有直接连接。

验收：从正式 FOSD 窗口完成 UDP/TCP 收包、标定、模型变形、着色/选点及 CSV/SQLite 写入；开关通信、重复加载和关闭退出均正常。先使用已知协议仿真输入，再用真实设备记录比对字段和通道顺序。

### 阶段 2：配置与会话状态（P1）

建立有版本号的配置对象，先覆盖旧版必需的模型路径、标定、通道映射、采样设置和保存设置，再按需保存新的场景/关键点设置。支持默认值、导入导出、校验失败保留有效配置及旧 JSON/INI 的一次性迁移。不得把旧版可变全局配置直接共享给工作线程。

用应用层会话状态明确“开始、暂停、停止、复位”对采集、解算、显示、存储各自的影响。暂停显示不自动停止采集或原始数据记录；复位应明确是单次恢复形状还是停留原始模型，不能把 resetDeformation 当作暂停。后台命令与 modelAccepted/configurationAccepted 等回执保持一致。

验收：重启可恢复有效配置，损坏或不兼容配置可回退；快速开始/暂停/恢复/停止、模型重载和存储启停不会出现重复线程、重复连接或旧结果覆盖新配置。

### 阶段 3：剩余模型和可视化操作（P1）

推荐先完成基础模型颜色，这是独立、范围小且可直接验证的迁移项；关闭位移着色时必须恢复用户设置的颜色。随后实现 MeshManager 的 optimize/exportModel，并新增合并、Control 转发、模型统计和正式 Model Backend 适配。

优化/合并可能重建拓扑和点 ID：统一产生新 modelRevision，重新提交模型给 CalculationControl，清除旧选择、关键点及过期标量结果。导出须明确原始模型或当前显示模型；STL 再导入不保证保留点 ID，按几何/三角形验证，不按旧 ID 验收。

在现有关键点和 Picker 基础上补编辑窗的增删改、全选和区域盒选。明确区域判定依据原始坐标还是显示坐标，选择结果绑定模型版本。最后补跟随选中点/追踪点的相机模式；锁定、目标删除、模型切换时处理跟随状态。

验收：颜色与位移映射开关切换一致；优化/合并后无旧 ID 残留；导出可重新加载且几何符合选定导出模式；区域选择和模型统计正确，跟随不改变解算数据。

### 阶段 4：独立追踪点闭环（P1）

在 core/Datamodel 定义追踪点 id、名称、原始坐标及只读结果，区分“模型网格点 ID”和“独立追踪点 ID”。提供文件导入、增删改及显隐配置。

CalculationWorker 使用已有 IFrameIFE 接口在独立坐标上插值，不修改 IFE 算法、不把追踪点硬塞入原始网格。可视化复用 KeyPointCreator，增加适配独立坐标的管理层，显示位置与倍率同步；位移数值保持真实单位。通过单独数据类型或显式目标类别接入 Storage，避免与模型点 ID 冲突。

验收：不位于网格顶点的追踪坐标可以得到插值输出；同号网格点和追踪点不会混淆；表格、标记、位移文本、CSV/SQLite 结果对应同一配置版本。

### 阶段 5：观察面板与采样保存交互（P2）

接入二维坐标观察、硬件数据和追踪点表格。UI 以可控频率读取只读快照；历史曲线使用有限长度缓存，不逐帧向 GUI 堆积事件。标定配置只由计算模块生效，UI 不重复计算。

在现有 StorageControl 上层补手动单次快照和可配置间隔的采样策略，明确逐帧保存和定时采样的区别；写入仍托管在解算线程。正式保存界面提供格式、数据类型、点选择、文件路径和写入状态。

验收：无三维模型时仍可观察有效波长/标定硬件数据；二维表中的原始/位移/最终坐标正确；单次保存无重复，定间隔保存符合策略，未破坏现有逐帧模式。

### 阶段 6：账户、诊断与交付（P2，账户按产品范围决定）

账户功能确需保留时，以独立服务接入 AccountBackend，复现登录、注册、账户管理和角色流程，不直接搬入旧版明文密码配置方式。统一汇总通信、协议、计算、存储及可视化错误，补充诊断日志和会话关联信息。

使用正式主程序完成连续运行、断开重连、模型重载、配置切换、存储故障和退出排空测试。制作 Qt/VTK/SQLite 运行依赖的部署清单，在没有开发环境 PATH 的机器验收。Debug/Release 使用各自匹配的 VTK 运行库。

## 保持现有约束与迁移边界

- ProtocolControl 继续托管通信与协议的同一工作线程；CalculationControl 继续托管计算和存储。先测实际负载，不为功能迁移重新拆分线程。
- IFE 保持黑箱，仅检查输入输出、调用顺序和单位，不新增矩阵/自由度约束配置，也不承担算法正确性验证。
- 弃用 JCS 通道包 parse 返回空、未发布下游数据是既定设计，不列为缺失功能；原始波长链路继续覆盖现有 MP/JCS 协议。
- 通用点标量输入已经能接扩展场，但真实节点应变/温度尚需提供者；旧版部分标量初始化为占位值，不能据此声称已有完整物理场可直接迁移。
- 数据回放工具、更多算法和更多设备协议属于可选扩展，除非确认是本次交付范围，否则不计入旧版必须复现清单。
- 当前 `src/visualization/README.md` 仍描述“骨架阶段”，已与实现不符，需在文档收尾时同步更新，不能以该旧说明判断模块功能未实现。

建议验收顺序为：正式入口最小闭环 → 配置/会话稳定 → 模型外观与管理 → 独立追踪点 → 观察面板与保存策略 → 账户/工程交付。每阶段分别安排底层单元测试、Backend/Presenter 适配测试和正式入口场景测试。
