# 数据存储模块对接

## 架构与线程

`CalculationControl` 管理解算 QThread，并通过 `storage()` 提供 `StorageControl`。
`StorageControl` 是调用线程上的门面，内部 `StorageWorker` 和 `CalculationWorker` 托管于同一解算线程；存储模块不创建额外线程。

```text
ProtocolControl::frameParsed → StorageControl::writeWavelengthFrame
                                     ↓ 队列
解算线程：StorageWorker → StorageRecords → IStorageWriter → CsvWriter / SQLiteWriter
           ↑ 直接调用
         CalculationWorker::calibratedReady / resultReady
```

写入器的创建、打开、写入、关闭和销毁均在解算线程执行，包括 SQLite 连接。写入是同步的，会占用解算线程；当前没有独立磁盘队列或背压策略。上层通过异步信号接收状态，无需直接访问 Worker 或数据库连接。

原始波长由协议的逐帧通知提供，不能用 `FrameRepository::latest()` 轮询替代，否则可能遗漏中间帧。`apps/FOSD-Win/FOSD.cpp` 已接好此入口。该协议门面通知目前经调用线程转发，调用线程需要正常运行事件循环。标定及位移数据直接在解算线程内转交存储，不经主线程。标定/解算仍采用最新快照轮询，因此不保证逐帧产生标定或位移结果。

无仓库回放模式下，`CalculationControl::submitFrame()` 会先提交原始波长存储，再提交计算；不要再为同一帧调用 `writeWavelengthFrame()`，也不要混用自动轮询和手动回放。

## 配置与调用

链接 `fosd_calculation` 自动获得 `fosd_storage` 依赖；单独使用写入器可链接 `fosd_storage`。

```cpp
#include "calculation/CalculationControl.h"
#include "protocol/ProtocolControl.h"

fosd::protocol::ProtocolControl protocol;
fosd::Calculation::CalculationControl calculation(protocol.dataModel());
QObject::connect(&protocol, &fosd::protocol::ProtocolControl::frameParsed,
                 &calculation.storage(),
                 &fosd::storage::StorageControl::writeWavelengthFrame);

fosd::storage::StorageConfiguration config;
config.format = fosd::storage::StorageFormat::SQLite; // 或 Csv
config.path = "D:/FOSD/data/experiment.sqlite";       // CSV 时指定 .csv 文件
config.dataTypes = fosd::storage::DataType::Wavelength |
                   fosd::storage::DataType::CalibratedHardware |
                   fosd::storage::DataType::PointDisplacement;
config.pointIds = {vtkIdType(42), vtkIdType(100)};
calculation.storage().start(config);
// 收到 started(sessionId) 后开始本次采集；正常配置标定参数和原始模型。
// ...
calculation.storage().flush(); // 可选；每次 write 已完成 CSV flush / SQL commit
calculation.storage().stop();
```

默认不启动存储、不创建文件；未启用的类型和非活动会话收到的数据直接忽略。支持三类数据按位组合。保存位移时，必须提供非负、无重复的 `pointIds`，或设置 `allPoints=true` 且清空 `pointIds`。选定点只影响保存范围，不影响解算遍历全部模型点。

原始波长保存不需要模型或标定配置。硬件标定数据需要有效标定配置，即使尚未导入模型也会输出；点位移需要模型和成功的解算结果。算法继续作为黑箱调用，存储层不验证 IFE 中间自由度或矩阵。

`start/stop/flush` 均为异步命令：`started(sessionId)` 表示文件已打开，`batchWritten(type,sourceRevision,rows)` 表示该批已写入，`stopped()` 表示会话已关闭。运行中修改配置会报错并保留当前会话，应先 `stop()`，再 `start(newConfig)`。同一门面提交的命令按顺序执行。每次启动产生独立 UUID 会话标识，兼容文件会追加写入。

## 数据与文件格式

`core/Datamodel/CalibratedData.h` 定义只读 `CalibratedDataPtr`：包含来源快照、配置版本、全部标定应变、实际送入 IFE 的选择/重排及增益后的应变、选中记录对应的原始下标。共享时保留 const shared_ptr，不修改来源对象。

两种后端使用 `StorageTypes.cpp::storageSchema()` 的统一长表结构，每个波长记录、标定值或模型点各占一行：

| 字段 | 含义 |
| --- | --- |
| `session_id`, `data_type`, `stage` | 会话、数据类别与处理阶段 |
| `protocol_id`, `source_revision`, `received_at_utc` | 来源协议、快照版本与 UTC 接收时间（毫秒） |
| `transport`, `peer_address`, `peer_port`, `device_sequence` | 通信来源、设备包序号 |
| `algorithm_id`, `calculation_revision`, `model_revision`, `configuration_revision` | 算法及计算所用版本；不适用的字段留空 |
| `sample_index`, `source_index`, `channel`, `sensor` | 当前数组零基下标、原始 records 零基下标、设备通道和传感器号 |
| `point_id`, `wavelength`, `strain` | 模型点 ID、原始波长、应变；仅填写适用字段 |
| `dx_mm`, `dy_mm`, `dz_mm` | 点位移，毫米 |
| `x_mm`, `y_mm`, `z_mm` | 本次变形后的模型坐标，毫米 |

`data_type/stage` 取值如下：

| 类别 | 阶段 | 内容 |
| --- | --- | --- |
| `wavelength` | `raw` | 解析后的原始波长记录，保留原通道和传感器 ID；不包含原始字节包 |
| `calibrated_hardware` | `calibrated` | 全部硬件记录的波长标定应变 |
| `calibrated_hardware` | `selected` | 通道选择、重排及增益后的应变；`source_index` 保留来源位置 |
| `point_displacement` | `calculated` | 指定点或全部点的位移及变形坐标 |

波长和应变单位沿用协议/标定定义，不另行换算。CSV 用 UTF-8、逗号分隔、LF 换行；字段包含逗号、双引号或换行时加引号，双引号转义为两个双引号。double 使用可往返精度。SQLite 表名为 `fosd_records`，另有自增 `record_id` 主键和来源索引。`quint64` 版本号使用十进制 TEXT 避免精度损失；点 ID 使用 INTEGER 保留 vtkIdType 的整数精度。原始波长中的非有限值保存为文本 `NaN/Inf/-Inf`，避免 SQLite 将 NaN 转为 NULL；标定和位移必须为有限数值。

同一文件仅供一个写入会话使用。现有 CSV 表头及 SQLite 列名、类型、主键必须兼容，否则拒绝打开，保留已有内容。CSV 尾部缺少换行时也拒绝追加，避免与残缺记录拼接。没有自动覆盖、轮转或旧格式迁移。

同一帧不同类别分批提交：一次原始帧、一次完整标定数据、一次选定点位移。SQLite 每批使用一个事务，任一行失败整批回滚；CSV 先组装整批再写入，普通文件写入失败时尝试截断回该批起始位置。不同批之间不承诺原子性，例如已保存原始波长后解算失败时，原始波长仍保留。模型或配置修改会触发对最新快照重算，同一 `source_revision` 可出现多次处理记录，应结合会话及各版本字段分析，不能将来源版本视为唯一键。

## 错误与生命周期

`errorOccurred(StorageError)` 包含 `operation`、`message`、`path`、`sourceRevision`。配置/打开失败不会启动会话；活动会话的序列化或写入失败会报错并关闭存储，防止失败后继续静默积压。已提交的其他批保留，解算模块继续运行。选中点缺失会拒绝整批点数据，不部分保存；需要恢复时修正配置后显式重新启动。

在创建 `CalculationControl` 的调用线程销毁控制器。析构先等待已经提交到解算线程的任务执行，再关闭写入器，最后退出 QThread；采集端应先停止产生新数据。尚未由协议门面转发到存储的通知不属于已提交任务。`flush/commit` 不等于断电安全保证。

## 扩展与测试

新增格式实现 `IStorageWriter` 的 `format/open/write/flush/close/isOpen`，统一结构由 `columns()` 暴露；通过 `createStorageWriter` 或构造 `CalculationControl` 的第四个 `WriterFactory` 参数注入。工厂必须在工作线程创建对象，不共享 UI 线程创建的数据库连接。

新增数据类型在 `DataType`、配置支持集合及 `StorageRecords` 添加序列化规则和输入入口，再与对应业务阶段连接。后端只处理统一行数据。变更列结构时应另行设计格式迁移，当前版本会拒绝旧结构。

```powershell
cmake --build build --config Debug --target test_storage
$env:PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
$env:QT_PLUGIN_PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/plugins'
ctest --test-dir build -C Debug -R test_storage --output-on-failure
```

`test_storage` 覆盖两种格式、三种数据类别、CSV 转义和精度、重复启动追加、SQLite 事务回滚、线程归属、无模型标定、协议逐帧原始存储、指定/全部点、异常隔离及退出排空。运行和部署需要 Qt5Sql DLL 与 Qt 的 `sqldrivers/qsqlite[d].dll` 插件，Debug/Release 必须匹配。

`test_end_to_end` 从真实本机网络收包开始，覆盖以下四种组合：UDP → CSV、UDP → SQLite、TCP 分片 → CSV、TCP 分片 → SQLite。测试仅用 socket 模拟外部设备，内部使用生产拆包器、MP 波长解析器、标定、HYY 和存储写入器，不调用 `parseFrame()` 或 `submitFrame()` 绕过前序模块。

每组发送 3 帧不同序号和波长的 12 记录数据，等待该帧计算及保存完成后再发送下一帧，符合解算模块“轮询最新快照、可能跳帧”的接口约定。原模型含 3 个稀疏 ID 点，跨两个插值单元，保存其中 2 点（含大整数 ID）。每帧核对 12 行波长、12 行标定值、12 行重排/增益值和 2 行点位移，关闭写入器后重新读取文件/数据库，共检查 114 行数据无重复、无遗漏，并核对各阶段来源快照、时间、端口、设备序号及配置版本的一致性。

TCP 每帧先只发送前 5 字节，确认通信层已收到而完整包、解析、计算、存储均未提前输出，再发送剩余部分。HYY 仅通过公开接口独立生成预期位移，用于检查模块间传参及毫米换算，不验收算法内部或物理正确性。串口仅传输指令，不在波长解算存储链路测试范围内。

```powershell
cmake --build build --config Debug --target test_end_to_end
ctest --test-dir build -C Debug -R '^test_end_to_end$' --output-on-failure
```

运行前沿用上面的 Qt DLL 和 SQL 插件环境配置。测试使用系统动态分配的端口和自动清理的临时目录，不依赖硬件设备或固定端口。
