# 解算模块对接

## 数据流与线程

```text
通信/协议共享线程 → FrameRepository 最新只读波长快照
                           ↓ 解算线程每 10 ms 读取
标定 → 通道选择/重排 → HYY 自由度 → 遍历原模型全部点 → CalculationRepository
                                                           ↓
                                         resultReady / latest() → 存储、显示
```

`CalculationControl` 是调用线程的门面，拥有独立 QThread。定时器、标定对象、IFE 工厂、自由度计算和插值均在 `CalculationWorker` 所在线程执行。读取输入和发布输出不依赖主线程事件循环；上层通知的接收仍需要事件循环。

自动模式读取最新状态，计算耗时超过输入帧间隔时会跳过中间帧，不保证逐帧解算。需要原始帧完整存储时应使用 protocol 的逐帧通知独立持久化。`submitFrame()` 用于无输入仓库的测试和回放，不与自动模式混用。

`CalculationControl::storage()` 提供同线程托管的存储控制器，支持 CSV/SQLite、原始波长、硬件标定数据及指定点位移。标定和位移通过 Worker 间直接连接写入；原始波长在主程序连接协议逐帧通知，手动回放则由 `submitFrame()` 自动提交。有效标定配置可在尚无模型时产生硬件标定数据，导入模型后才计算位移。详见 [数据存储对接](storage-integration.md)。

## 模型输入与结果输出

公共类型位于 `src/core/Datamodel/ModelData.h`：

- `ModelData::rawPointMap`：`QMap<vtkIdType, QVector<double>>`，点 ID 对应 `{x,y,z}`，单位毫米。ID 可以稀疏，不按连续下标访问。
- `CalculationResult::displacementMap`：同一组点 ID 的 `{dx,dy,dz}`，单位毫米。
- `CalculationResult::deformedModel.rawPointMap`：原坐标加本次位移。每次从原模型计算，避免逐帧累计变形。
- `nodeDofs`：算法内部数据，仅供诊断；控制层不解释其维度、排列或内容，不以它作为模型输出的验收依据。
- `strains`：校准并选择、重排后，实际送入 IFE 的应变数组。
- `sourceFrame`：本次使用的完整输入快照；`modelRevision`、`configurationRevision` 标识所用配置。

`ModelData` 默认构造时 `rawPointMap` 是空表，不会从 VTK、文件或计算参数自动获取点。当前主程序只创建控制器，还未接入实际模型导入器；测试通过构造坐标表提供模型。

导入或编辑模块生成 `core::PointMap` 后，调用 `CalculationControl::setRawPointMap()`。该公开槽可直接调用，也可以从导入线程通过 `Qt::QueuedConnection` 连接。已有 `setModel(const ModelData &)` 继续可用。

```cpp
#include "calculation/CalculationControl.h"
#include "protocol/ProtocolControl.h"

fosd::protocol::ProtocolControl protocol;
fosd::Calculation::CalculationControl calculation(protocol.dataModel());

fosd::core::PointMap rawPoints;
// 后续由文件导入器或 VTK 模型遍历填充；这里仅示范一个点。
rawPoints.insert(vtkIdType(42), QVector<double>{350.0, 10.0, 0.0});
calculation.setRawPointMap(rawPoints);
calculation.setConfiguration(configuration); // 仅提供波长标定及通道选择配置，见下节。

// 修改原始模型后，重新提交完整坐标表。
rawPoints[42] = QVector<double>{360.0, 10.0, 0.0};
calculation.setRawPointMap(rawPoints);

const auto repository = calculation.dataModel();
// 在显示刷新或业务线程中调用：
const auto result = repository->latest();
if (result) {
    const auto &points = result->deformedModel.rawPointMap;
    // 根据 points 更新显示模型；保留 result，不能仅保存局部引用。
}
```

该输入接口每次整体替换原始点表，不合并旧点；适用于导入新模型，或编辑后重新提交。数据经队列传到解算线程，校验非空、非负 ID、三维有限坐标后保存为只读模型快照。提交后修改调用方的坐标表不会改变已提交的模型。成功发送 `modelAccepted(revision)`，拒绝时发送 `errorOccurred`，旧模型仍有效。

替换成功后模型版本递增，自动模式会用当前最新波长快照重新计算；回放模式需再调用 `submitFrame()`。显示或存储模块使用结果的 `modelRevision` 确认所用模型。`dataModel()->latest()` 读取的是计算结果，不能把其变形后的坐标当成原始模型编辑源；编辑模块应保留自己的原始坐标表。

也可将 `CalculationControl::resultReady(const CalculationResultPtr &)` 通过队列连接交给接收线程处理。需保存异步任务时复制 shared_ptr。仅使用数据模型的模块链接 `fosd_core`；使用控制器则链接 `fosd_calculation`。

仓库用读写锁保护最新指针和修订号；发布对象实际构造为 const，消费者不得保留可写别名或移除 const。旧结果及仓库句柄可存活到控制器销毁之后。一个点失败时整帧不发布，保留上次成功结果。

## 标定输入与算法调用

`CalculationConfiguration` 通过 `setConfiguration()` 异步提交；成功发送 `configurationAccepted(revision)`。模型通过 `setModel()` 提交，成功发送 `modelAccepted(revision)`。错误使用 `errorOccurred(CalculationError)`，包含 operation、message、sourceRevision；非法更新不替换已生效的模型或配置。正确更新会重新计算当前最新输入。

标定过程：

```text
calibrated[i] = calibrationFactor * (wavelength[i] - originalWavelength[i]) / A + B
strain[j] = calibrated[channelIndices[j]] * calibrationFactors[j] + calibrationOffsets[j]
```

`originalWavelength` 与输入记录数量必须完全相同；`channelIndices` 是解析器 records 的零基位置索引，不是设备通道号。应用必须保证参考波长、映射和输入记录身份、顺序一致；若设备更改通道/传感器排列，应同步更新配置。通道映射可以选择输入的子集，输出数量等于映射数量。

`CalibrationChannelData::calibrationOffsets` 是每个输出通道的加性偏置，与映射、比例因子按输出位置 `j` 对齐，单位与标定后的应变一致；允许正值、负值和零，与全局波长转换中的 `B` 分开应用。偏置默认空数组，计算时等价于全部为零，兼容未提供偏置的调用方。显式提供偏置时长度必须与 `channelIndices` 一致，且所有值必须有限；长度不匹配、NaN/Inf 均拒绝配置，乘加结果溢出则拒绝本帧。

```cpp
fosd::Calculation::CalculationConfiguration configuration;
// 同时设置有效的参考波长、calibrationFactor、A/B。
configuration.strainCalibration.originalWavelength = {1000, 2000, 3000};
configuration.strainCalibration.calibrationFactor = 2;
configuration.strainCalibration.A = 4;
configuration.strainCalibration.B = 3;
configuration.channelCalibration.channelIndices = {2, 0};
configuration.channelCalibration.calibrationFactors = {2, 3};
configuration.channelCalibration.calibrationOffsets = {1.5, -0.25};
calculation.setConfiguration(configuration);
// 输入 {1002, 2004, 3006} -> 全量应变 {4, 5, 6} -> 通道应变 {13.5, 11.75}。
```

单独使用标定器时通过 `setCalibrationChannelData()` 设置上述结构，再调用 `checkPara()`；setter 沿用整体赋值行为，校验在显式检查和计算入口执行。使用 `CalculationControl::setConfiguration()` 时，解算线程先校验、持久化完整配置，再激活；非法更新不改变已生效的配置或 INI。`calibratedStrains` 保留偏置前的全量波长标定结果，`selectedStrains`、IFE 输入、结果 `strains` 和存储的 selected 阶段均包含通道偏置。清空 `calibrationOffsets` 可取消所有通道偏置。

INI 使用 `[calculation]` 下的 `channelCalibration.calibrationOffsets` 字符串列表。源码默认模板为空列表 `@Invalid()`；构建部署后只写运行时 `parameters.ini`。旧文件缺少该键时，`ParameterStore::initialize()` 补入零偏置默认值，保留已有基线、映射及比例；`read()` 只读取，不负责补齐。本次仅增加后端参数，前端没有新增输入控件。

`A` 必须有限且非零；B、所有波长、因子、偏置和标定结果必须有限。波长与因子要求正数，偏置没有正数限制。参数采用 double，初始化会恢复默认系数 0.00075、清空参考、映射和偏置，并将 A/B 复位为零。未配置状态不能计算。

`CalculationConfiguration` 只包含 `strainCalibration` 和 `channelCalibration`。IFE 作为黑箱调用，不再提供自由度映射、约束值或矩阵维度等业务配置；算法内部数据直接从计算接口传递到插值接口。控制层不会试算、检查或拒绝算法的中间数组，也不验收算法或插值的物理正确性。

保留的检查只针对模块边界：波长快照及其记录数量、标定参数、输入坐标格式和有限性，以及最终位移和更新坐标的有限性。模型点的算法适用域不作为输入模型的通用准入条件。

## 扩展、构建与生命周期

新增算法只需实现 `IFrameIFE` 的调用接口，通过无参 `IFEFactory()` 注入。工厂在解算线程创建算法，无需调用者了解矩阵、自由度数目或初始化过程：

```cpp
virtual QString IFEId() const = 0;
virtual NodeDofs calculateDofs(const Strain &strain) const = 0;
virtual Displacement calculateDisplacement(const Point3D &point,
                                           const NodeDofs &dofs) const = 0;
```

`initMatrix()` 等初始化方法属于算法类的私有实现，基类不暴露。两个计算函数不接收错误字符串，不格式化错误、不记录日志、不发送信号。控制层每帧调用一次计算接口，随后对全部模型点调用插值接口；发现最终模型输出不是有限数值时，统一上报错误并保留上一份完整结果。

HYY 保留原有数值实现，去除内部维度、缺项及约束配置的验收。容器默认取值属于实现细节，不要求上层提供约束参数；不能将默认取值解释为已经验证的物理约束。后续替换算法仅需适配上述接口。

core 只使用实际 VTK 的 `vtkType.h` 及其配置头文件来定义 `vtkIdType`，不链接渲染库、不创建 VTK 对象。默认取仓库内当前平台的 VTK 8.2 头文件；可用 `FOSD_VTK_TYPES_INCLUDE_DIR` 覆盖。后续可视化模块应使用同一套 VTK 类型配置，以免点 ID 宽度不一致。

主程序已按 protocol → calculation 顺序创建对象，反向析构先停止解算线程。控制器必须在创建它的调用线程析构；析构等待已提交到工作线程的任务执行，停止轮询，关闭同线程存储，最后退出线程。外部采集应先停止提交新任务；尚未转发到解算/存储的通知不在排空范围。不要从解算 Worker 自己析构控制器。

```powershell
cmake --build --preset windows-debug
ctest --preset windows-debug
```

`test_calculation` 验证标定输入、输入输出边界、任意算法内部数组可透传、配置时不试算、全部点遍历、毫米换算、旧结果不可变、最终输出异常时整帧回退、真实 UDP→protocol→calculation 链路、主线程不调度时继续解算、并发读取及线程销毁。不再验证矩阵维度、自由度映射及算法物理正确性。
