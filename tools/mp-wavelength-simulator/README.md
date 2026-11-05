# MP 波长 UDP 回环模拟器

`MPWavelengthSimulator.exe` 提供持续发包和自动自检两种模式，仅发送至 IPv4 回环地址 `127.0.0.1`。不需要真实设备，也不会向局域网发包。

## 与主程序联调

在 FOSD-Classic-QWidgets 的通信页选择 UDP，本地地址填 `127.0.0.1`、本地端口填 `9000`。目标地址填 `127.0.0.1`、目标端口填 `9001`（当前界面要求目标端口有效，接收不依赖这个端口），点击打开网络。

在模拟器目录打开终端执行：

```powershell
.\MPWavelengthSimulator.exe --port 9000 --count 100 --interval 20
```

默认每帧 3 通道 × 4 传感器，共 12 条记录、82 字节，序号从 1 开始。波长为 `1550 + 记录序号 * 0.5 + 0.02 * sin(帧序号 * 0.1)` nm，记录序号从 0 开始。可在主程序波长页查看解析结果，在 UDP 接收区查看 Hex。

连续发包直到 Ctrl+C：

```powershell
.\MPWavelengthSimulator.exe --port 9000 --count 0 --interval 20 --channels 3 --sensors 4
```

`--sequence` 可设置初始 uint32 序号，溢出后回到 0。`--channels` 和 `--sensors` 范围为 1–255，乘积不超过 10916。`--interval` 单位为毫秒，默认 20；`--count` 默认 100，0 表示持续发送。

UDP 发送成功只说明数据提交给本机 socket；验证接收与解析请使用下面的自检模式，或在主程序检查波长表。这个模拟器只验证原始波长接收，不自动配置标定、模型和解算。

## 自动验证生产解析链路

```powershell
.\MPWavelengthSimulator.exe --self-test
```

自检创建独立 UDP 接收端和临时配置文件，发送端和接收端均绑定回环地址，使用空闲端口。接收端运行生产 `ProtocolControl` 默认工厂，经过真实 socket → `UdpPacketDecoder` → `MPWavelengthParser` → 只读数据仓库；不调用 `parseFrame()` 绕过网络。无需打开主程序。

验证内容：独立 Hex 固定样本、小端序、帧序号、版本和类型、通道和传感器编号、每个浮点数的精确位模式、零记录、字段边界、特殊浮点值、65506 字节 UDP 包、连续 100 帧，以及错误魔数、短头、截断、尾随字节、错误记录数、合并两帧和跨数据报分片的拒绝。无效包不得覆盖已有有效快照。

成功输出 `RESULT PASS: received=113 parsed=105 rejected=8 parserErrors=0`，退出码 0；失败输出原因并返回非 0。8 个坏包应在 UDP 拆帧层拒绝，因此解析层错误数为 0。UDP 一个数据报对应一帧，不缓存跨数据报半包，也不接受一个数据报拼接多帧。65535 条记录的协议极限超出 IPv4 UDP 单包容量，仍由现有 `test_protocol` 直接解析测试覆盖。

## 编译与依赖

模拟器随启用 `fosd_protocol` 的主工程配置构建；启用测试时自动注册为 `test_mp_wavelength_loopback`。

```powershell
cmake --build build/fosd-widgets-package --config Release --target MPWavelengthSimulator
ctest --test-dir build/fosd-widgets-package -C Release -R "test_mp_wavelength_loopback|test_protocol|test_packet_decoders" --output-on-failure
```

运行需要匹配的 Qt 5 Core、Network、SerialPort 和 MSVC x64 运行库。随附便携 ZIP 已包含这些 DLL，无需安装 Qt 或 Visual Studio。`验证MP解包.cmd` 可双击执行自检，`连续发送到9000.cmd` 可双击开始向主程序发包。
