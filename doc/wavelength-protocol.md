# 波长数据包兼容与验证

## 新格式

依据提供的 `QT解析代码.txt` 实现，整数及 float 均为小端序。

| 偏移 | 长度 | 字段 |
| --- | --- | --- |
| 0 | 2 | magic = 0xA55A，线上字节为 `5A A5` |
| 2 | 1 | version |
| 3 | 1 | dataType |
| 4 | 2 | recordCount，uint16 |
| 6 | 4 | sequence，uint32 |
| 10 + 6 × i | 1 | channel |
| 11 + 6 × i | 1 | sensor |
| 12 + 6 × i | 4 | wavelength，IEEE754 float32 |

包长必须等于 `10 + recordCount × 6`，最多 1024 条记录。空帧会清空当前输出。
附件未规定编号起点，当前按原界面 `CH1_1` 约定采用 **从 1 开始** 的编号：
channel 为 1～16，sensor 为 1～255。拒绝重复编号、非有限波长值及错误包长。
附件未限定 version/dataType 的有效值，解析器保留这些字段，不额外限制取值。

## 接入原路径

新格式按 `(channel, sensor)` 排序，旧 `FA FB FA FB` 波长包按原通道顺序解码，
二者共用 `processWavelengthFrame()`：通道计数、通道筛选、`FBG_count`、
`CH_datagram`、波长字符串、应变计算、`strain_float`、20 帧平均复位及 CSV 保存。
传感器编号直接用于显示，例如 `(16, 255)` 显示为 `CH16_255`。
原有初始波长文本仍按通道/传感器排序后的数组位置对应，需保证校准时与采集时的传感器布局一致；
布局改变后应重新复位。复位期间布局变化会重新开始 20 帧统计，避免混合不同传感器。

每个新包作为完整帧更新数据。乱序记录不会改变输出顺序，异常包不会覆盖上一帧。
通道被取消或帧变短时清除旧输出；最后一个物理通道未选中时，仍正确生成保存字符串。
旧光谱包继续使用原解包路径，旧波长包允许原有尾部数据，并校验通道数量之和。

UDP 入口支持两种帧头。TCP 对新格式根据包长缓存分片、拆分粘包；旧 TCP 接收行为保留。

## 测试

测试源：`tests/protocol_tests.cpp`，使用真实解析器、数据处理类和网络接收类。

- 独立十六进制样本验证波长 1550.25、小端字段及 sequence。
- 截断、尾随字节、错误帧头/数量、非法编号、重复记录、NaN/Inf；失败时保留解析输出。
- 空帧、1024 条边界与超限拒绝。
- 新旧包输出一致，乱序排序、通道筛选、应变数值与保存内容。
- 新旧包交替进行 20 帧平均复位，核对初始波长文件及手动/自动 CSV。
- 稀疏编号、复位中布局变化、重复复位。
- 旧光谱包及截断保护。
- 本机 UDP 实际收发新旧包，核对最终波长。
- 本机 TCP 逐字节分片、连续新包粘包、旧包与重连后缓存清理。

可独立构建测试，不依赖 VTK：

```powershell
cmake -S tests -B build/protocol-tests -DCMAKE_PREFIX_PATH=<Qt安装路径>
cmake --build build/protocol-tests --config Release
cd build/protocol-tests
# 将 Qt 的 bin 目录加入 PATH。
ctest -C Release --output-on-failure
```

主项目通过 `BUILD_TESTING` 控制是否构建测试。验证使用生成数据与本机网络，未连接实际解调仪。

2026-09-30 验证结果：Qt 5.12.2 下 10 个测试场景全部通过（含初始化/清理共 12 项），
完整主项目 Release 编译与链接通过（MSVC v141）。

## 回环适配器发送脚本

`scripts/send_wavelength_packets.py` 只依赖 Python 标准库。
本机 Microsoft KM-TEST 环回适配器“以太网 2”已配置 `192.168.31.89`、
`192.168.31.88` 和 `192.168.31.1`。脚本默认绑定 `.89`，向 `.88:5060` 发送 UDP。
这两个地址属于同一回环适配器，无需修改网卡配置。

在项目的连接界面选择本地 IP `192.168.31.88`、本地端口 `5060`，启用 UDP 接收并选中通道 1、2。
然后在仓库根目录运行：

```powershell
python scripts/send_wavelength_packets.py
```

默认每 100 ms 发送一帧，包含 CH1_1～CH1_3、CH2_1～CH2_3，波长基值为
1550、1550.25、1550.5、1555、1555.25、1555.5，叠加周期 5 秒、幅值 0.05 的正弦变化。
按 Ctrl+C 停止。`--amplitude 0` 可发送恒定波长，便于复位与数值核对。

```powershell
# 新旧包交替发送 100 帧，打乱新包记录顺序以检查编号映射。
python scripts/send_wavelength_packets.py --protocol alternate --count 100 --shuffle

# 自定义固定波长；--record 可重复。
python scripts/send_wavelength_packets.py --record 1:1:1550.25 --record 2:1:1555.5 --amplitude 0

# 旧协议；传感器编号须在每个通道内从 1 连续递增。
python scripts/send_wavelength_packets.py --protocol legacy --count 20

# TCP 发送，需先在项目中启用同地址、同端口的 TCP 监听。
python scripts/send_wavelength_packets.py --transport tcp --count 20

# 使用临时空闲端口验证实际回环适配器 UDP/TCP 收发，不占用项目监听端口。
python scripts/send_wavelength_packets.py --self-test
```

独立验证脚本收发时，可在两个终端分别运行下列命令；接收脚本支持 UDP 解码。
端口 `5061` 用于独立接收，避免与项目的 `5060` 冲突。

```powershell
# 终端 1：接收并显示解包后的波长。
python scripts/send_wavelength_packets.py --mode receive --port 5061 --count 20
# 终端 2：发送。
python scripts/send_wavelength_packets.py --port 5061 --protocol alternate --count 20
```

通过 `--bind`、`--host`、`--port` 可改用其他已配置地址和端口，例如常规本机回环：
`--bind 127.0.0.1 --host 127.0.0.1`。IP 必须与项目实际监听地址匹配。
UDP 的发送成功仅表示已交给系统；收到并显示波长后才能确认接收成功。
