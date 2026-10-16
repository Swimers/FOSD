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

## 模型输入与结果输出

公共类型位于 `src/core/Datamodel/ModelData.h`：

- `ModelData::rawPointMap`：`QMap<vtkIdType, QVector<double>>`，点 ID 对应 `{x,y,z}`，单位毫米。ID 可以稀疏，不按连续下标访问。
- `CalculationResult::displacementMap`：同一组点 ID 的 `{dx,dy,dz}`，单位毫米。
- `CalculationResult::deformedModel.rawPointMap`：原坐标加本次位移。每次从原模型计算，避免逐帧累计变形。
- `nodeDofs`：算法全局自由度，平移单位米、转角单位弧度。
- `strains`：校准并选择、重排后，实际送入 IFE 的应变数组。
- `sourceFrame`：本次使用的完整输入快照；`modelRevision`、`configurationRevision` 标识所用配置。

```cpp
#include "calculation/CalculationControl.h"
#include "protocol/ProtocolControl.h"

fosd::protocol::ProtocolControl protocol;
fosd::Calculation::CalculationControl calculation(protocol.dataModel());

fosd::core::ModelData model;
model.rawPointMap.insert(vtkIdType(42), QVector<double>{350.0, 10.0, 0.0});
calculation.setModel(model);
calculation.setConfiguration(configuration); // 应用提供实际标定及约束，见下节。

const auto repository = calculation.dataModel();
// 在显示刷新或业务线程中调用：
const auto result = repository->latest();
if (result) {
    const auto &points = result->deformedModel.rawPointMap;
    // 根据 points 更新显示模型；保留 result，不能仅保存局部引用。
}
```

也可将 `CalculationControl::resultReady(const CalculationResultPtr &)` 通过队列连接交给接收线程处理。需保存异步任务时复制 shared_ptr。仅使用数据模型的模块链接 `fosd_core`；使用控制器则链接 `fosd_calculation`。

仓库用读写锁保护最新指针和修订号；发布对象实际构造为 const，消费者不得保留可写别名或移除 const。旧结果及仓库句柄可存活到控制器销毁之后。一个点失败时整帧不发布，保留上次成功结果。

## 标定与 HYY 配置

`CalculationConfiguration` 通过 `setConfiguration()` 异步提交；成功发送 `configurationAccepted(revision)`。模型通过 `setModel()` 提交，成功发送 `modelAccepted(revision)`。错误使用 `errorOccurred(CalculationError)`，包含 operation、message、sourceRevision；非法更新不替换已生效的模型或配置。正确更新会重新计算当前最新输入。

标定过程：

```text
calibrated[i] = calibrationFactor * (wavelength[i] - originalWavelength[i]) / A + B
strain[j] = calibrated[channelIndices[j]] * calibrationFactors[j]
```

`originalWavelength` 与输入记录数量必须完全相同；`channelIndices` 是解析器 records 的零基位置索引，不是设备通道号。应用必须保证参考波长、映射和输入记录身份、顺序一致；若设备更改通道/传感器排列，应同步更新配置。通道映射可以选择输入的子集，输出数量等于映射数量。

`A` 必须有限且非零；B、所有波长、因子和计算结果必须有限。波长与因子要求正数。参数采用 double，初始化会恢复默认系数 0.00075、清空参考与映射，并将 A/B 复位为零。未配置状态不能计算。

HYY 固定矩阵为 20×12，因此选择/重排后必须恰好有 12 个应变。先计算 20 个值，再按配置映射到 22 项全局自由度：

```text
nodeDofs = constrainedDofs
nodeDofs[freeDofIndices[row]] = sum(matrix[row][column] * strain[column])
```

- `freeDofIndices` 必须是 20 个不重复的下标，范围 0～21，顺序对应矩阵各行。
- `constrainedDofs` 必须包含 22 个有限值；两个未映射位置保留约束值，其余位置会被计算结果覆盖。
- 现有源码没有真实的两个约束位置及行映射依据，因此模块不猜测默认约束。应用必须提供实际映射；测试中的顺序映射仅为合成用例。
- 插值沿用原有两单元公式，长度各 700 mm，模型 X 范围 0～1400 mm；超出范围报错。原来的 `/1000` 换算保留，输出位移统一乘 1000 转回毫米。

矩阵物理含义、应变单位、真实约束和参考波长仍需以算法模型与实测标定为准。软件测试不代替物理工况验算。

## 扩展、构建与生命周期

新增算法实现 `IFrameIFE`，通过 `IFEFactory(const CalculationConfiguration &)` 注入。工厂在解算线程执行；构造并配置算法后，其输入长度、点域和错误通过带 `QString *error` 的计算接口验证。算法不得操作 UI，必须返回有限结果。

core 只使用实际 VTK 的 `vtkType.h` 及其配置头文件来定义 `vtkIdType`，不链接渲染库、不创建 VTK 对象。默认取仓库内当前平台的 VTK 8.2 头文件；可用 `FOSD_VTK_TYPES_INCLUDE_DIR` 覆盖。后续可视化模块应使用同一套 VTK 类型配置，以免点 ID 宽度不一致。

主程序已按 protocol → calculation 顺序创建对象，反向析构先停止解算线程。控制器必须在创建它的调用线程析构；析构等待正在执行的任务结束，不保证排空尚在队列中的提交。不要从解算 Worker 自己析构控制器。

```powershell
cmake --build --preset windows-debug
ctest --preset windows-debug
```

`test_calculation` 覆盖标定公式及选择、非法输入、矩阵基向量与约束映射、平移/转动插值、两单元边界、全部点遍历、毫米换算、旧结果不可变、失败整帧回退、真实 UDP→protocol→calculation 链路、主线程不调度时继续解算、并发读取及线程销毁。
