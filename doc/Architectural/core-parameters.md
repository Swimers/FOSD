# 核心参数与运行时 INI

核心参数结构集中在 `src/config/CoreParameters.h`。通信、解算、存储、可视化原头文件通过 include 继续提供原类型名和命名空间；原结构默认值未修改。选中点样式及显示刷新参数也集中定义，保持原有 7 mm / 青色选中样式、1 倍变形和 33 ms 刷新间隔。

## 默认模板和运行文件

| 文件 | 用途 |
| --- | --- |
| `src/config/core-defaults.ini` | 源码中的默认模板，与结构默认值对应 |
| `<可执行文件目录>/config/core-defaults.ini` | CMake 部署的默认模板副本，只作参考和部署，不用于保存用户修改 |
| `<可执行文件目录>/config/parameters.ini` | 实际运行配置，所有参数更新写入这里 |

构建 `fosd_parameters` 的依赖目标时，CMake 复制默认模板；不会复制或覆盖 parameters.ini。默认模板同时编入资源，首次构造业务 Control 时可从只读资源初始化完整运行配置；部署副本意外缺失也不会丢失默认值。已有参数只保留，新增字段补默认值。补齐动作持有文件锁，并原子保存。

`ParameterStore` 拒绝把名为 core-defaults.ini 的文件及指向它的别名作为写入目标。`FOSD_CONFIG_FILE` 可显式指定另一份运行文件，供测试或多配置实例使用，未指定时始终以 executable 目录定位，不依赖当前工作目录。

## 参数分区和接入位置

| INI 分区 | 结构 | 初始化与更新入口 |
| --- | --- | --- |
| communication.serial / udp / tcp | SerialConfig、UdpConfig、TcpConfig | ConnectControl 构造时补齐；openSerial/openUdp/openTcp 校验后保存，再提交设备请求 |
| calculation | CalculationConfiguration，包含应变标定和通道排列 | CalculationWorker 初始化时补齐；configure 通过标定和算法工厂校验后保存，再替换当前算法配置 |
| storage | StorageConfiguration | StorageControl 构造时补齐；StorageWorker::start 验证并成功打开写入器后保存，随后启动会话 |
| visualization.scalar | ScalarColorSettings | VisualizationControl 构造读取和应用；setScalarColorSettings 更新 |
| visualization.axes / grid / background / world / camera | 对应 Settings | 构造读取和应用；对应 set*Settings 更新，显隐和背景快捷入口同样转入这些设置方法 |
| visualization.keyPoint / selection / picking | KeyPointStyle、SelectionStyleSettings、PickingSettings | 构造读取和应用；对应样式/拾取设置入口更新 |
| visualization.runtime | VisualizationRuntimeSettings | 构造读取和应用；setDeformationScale、setfreshFPS 更新 |

通信、解算、存储在构造时只注册并校验已存参数，不自动连接设备、激活标定或开启存储会话。应用层仍显式调用原有业务入口；需要恢复已有参数时，可通过下面的统一读取接口取得。模型点坐标、计算结果、逐帧波长、当前选中点和鼠标拖动时的相机位姿属于运行数据，不写入这份参数配置。协议解析器工厂/拆包回调也不序列化。

## 统一接口

通用 INI 能力仍由 `fosd_config` / IConfigStore 提供，仅依赖 Qt Core。类型化参数由 `fosd_parameters` / ParameterStore 提供，依赖传输类型所需的 Qt Network / SerialPort，但不反向依赖各业务库。

```cpp
#include "config/ParameterStore.h"

fosd::config::ParameterStore store;
fosd::communication::TcpConfig tcp;
QString error;
if (store.initialize(&tcp, &error)) {
    // 已存值优先；缺失字段补默认值。此处不建立 TCP 连接。
}
if (store.read(&tcp, true, &error)) {
    // true 要求达到可运行状态，例如 TCP 端口不能为 0。
}
tcp.host = "127.0.0.1";
tcp.port = 9000;
if (!store.write(tcp, true, &error)) {
    // 校验失败不写入，I/O 失败保留原文件。
}
```

initialize 按结构初值补齐一个分区，read 只读取，write 以完整结构替换其所属分区。所有调用返回 bool，可选 error 获取原因。业务 Control 已在相应入口调用这些方法，业务调用方无需额外再写一遍 INI。

## 校验及失败处理

严格检查字段存在性、未知字段、字符串/布尔/数值转换、整数溢出、非有限数值及定长数组元素数。进一步校验传输枚举、端口和串口参数，标定系数、基线/通道映射/倍率的对应关系，存储格式/数据类型/点位选择，以及显示颜色、范围、尺寸、字体、网格、相机和拾取参数。

默认值不一定可立即运行业务。例如默认串口名和存储路径为空、TCP 端口为 0、标定基线为空且 A=0。这些作为待配置初值允许保存和读取；ready=true 时拒绝用它们启动对应业务。IFE 矩阵及内部自由度维度仍不属于配置层校验范围。

设备连接失败并不意味着连接参数无效，因此通信入口保存的是通过校验的请求参数。解算只有成功创建算法后才持久化配置；存储只有成功打开对应写入器后才持久化；可视化先由组件验证和应用，保存失败时回滚组件设置，再报告错误。构造时遇到配置错误通过现有 errorOccurred 延迟上报；失败分区保持结构默认状态，不静默覆盖损坏文件。

## 测试和默认值维护

`test_parameters` 校验模板和结构默认值一致、运行文件初始化、重建对象保留用户参数、坏字段/溢出/枚举拒写、默认模板保护，以及通信/解算/存储的实际配置落盘。回归测试为每个用例使用独立的临时 INI，避免影响开发机器的运行配置。可视化原有测试继续验证设置、标量和交互功能。

```powershell
cmake --build --preset windows-debug --target test_parameters test_config
ctest --preset windows-debug -R '^test_(parameters|config)$'
```

开发时如需更新默认模板，显式构建并运行生成器，再运行 test_parameters；生成器不会被普通构建或程序启动自动执行：

```powershell
cmake --build --preset windows-debug --target generate_parameter_defaults
./build/bin/Debug/generate_parameter_defaults.exe src/config/core-defaults.ini
```
