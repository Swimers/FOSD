# 板模型 / IFE / UDP 仿真

该工具集只由 `vtk-native-widgets` 验证项目构建。包含可直接导入的 STL、与其几何一致的示例逆向应变重构算法、固定频率 UDP 波长发送器和测试。HYY 生产算法不改动。

## 一键运行

```powershell
cmake --preset vtk-native-widgets
cmake --build --preset vtk-native-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe
```

打开窗口后，选择端口（默认 9000），点击顶部“一键启动板仿真”。程序自动选择 Plate IFE、导入板、提交默认标定和模型、监听本地 UDP，并在三者就绪后启动发送器。默认采样 50 Hz、运动主频 0.35 Hz、主模态幅值 15 mm；界面可修改采样率、频率和幅值。

板将周期弯曲、扭转，同时显示总位移颜色。可用现有颜色控件切换 X/Y/Z/总位移，或修改显示倍率放大运动；右手坐标单位 mm，真实位移不会随显示倍率改变。当前侧视方向是验证场景的等轴观察角度。

“停止板仿真”停止发送和 UDP 接收，保留最后结果。“返回 HYY”停止仿真并还原 HYY 解算实例，清除板模型，之后需重新导入实际模型和设置标定。切换算法会关闭原解算线程及其存储会话；需要录制时，先启动板仿真，再用现有 CSV / SQLite 按钮开始存储。

## 独立发送器

界面点击“加载板 / 等待外部发送器”，然后在另一终端执行：

```powershell
$env:PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/vtk-native-widgets/bin/Release/plate-udp-sender.exe `
  --host 127.0.0.1 --port 9000 --rate 50 --frequency 0.35 --amplitude 15
# --duration 10 可在 10 秒后退出；默认持续发送，Ctrl+C 终止。
```

独立发送器无需 VTK DLL，只需要匹配的 Qt 运行库。`--rate` 为 1～200 Hz，`--frequency` 为 0～10 Hz，`--amplitude` 为 0～100 mm，端口 1～65535。独立发送和界面内置发送二选一，避免同端口叠加两个时间序列。内置模式与默认外部模式监听 127.0.0.1。

发送采用单调时钟和精确定时器，按绝对截止时刻调度，运动相位按实际经过时间计算。桌面系统不是硬实时环境；调度滞后会跳过已错过的时间槽，不突发补发，也不累计毫秒取整误差。

## 文件与构建归属

| 文件 | 内容 |
| --- | --- |
| `models/plate.stl` | 二进制封闭板，1000×400×4 mm；40×16 表面网格；VTK 合并后 1394 点、2784 三角面 |
| `models/sensors.csv` | 12 个纵向表面传感器的编号、坐标、方向和参考波长 |
| `generate_plate.py` | Python 3 标准库生成器；重新生成同一几何及传感器表 |
| `PlateIFE.h/.cpp` | 实现 IFrameIFE：应变拟合三个模态及逐点插值 |
| `PlateSimulation.h/.cpp` | 几何、标定默认值、运动与采样数据、MP 波长编码 |
| `PlateUdpSender.h/.cpp` | 窗口内置与独立程序共用的定频发送实现 |
| `sender_main.cpp` | 独立发送器命令行入口 |
| `tests.cpp` | 算法、标定、封包、STL、UDP 到计算/存储及独立进程发送测试 |

模型随验证程序构建复制到 `bin/<配置>/tools/plate-simulation/models/plate.stl`。程序优先查找该部署位置，开发时可回退到源码 tools 路径；复制操作随构建执行，重新生成 STL 后不必修改 C++ 以触发复制。

重新生成（需要 Python 3，正常运行不需要 Python）：

```powershell
python tools/plate-simulation/generate_plate.py
```

## 数值模型与单位

这是一套用于数据链路验证的**三模态薄板逆向应变重构示例**，并非任意板、任意传感器布置的通用工程 iFEM 求解器。几何和传感器布局固定；改动几何时需同步修改 PlateSpec、传感器与模态定义。

板范围 x∈[0,1000]、y∈[-200,200]、z∈[-2,2] mm，x=0 边固支。令 ξ=x/L、η=2y/W：

```text
w(x,y) = ξ² (a0 + a1 η + a2 η²)           [mm]
u = -z ∂w/∂x, v = -z ∂w/∂y              [mm]
εxx = -z ∂²w/∂x²                         [无量纲]
传感器表面 z=t/2；其输出为 εxx × 10⁶      [microstrain]
```

12 个传感器的 η 从 -0.9 均匀分布到 0.9，测量方向均为 x。该模态假设的纵向曲率与 x 无关，纵向传感器位置仅用于说明布局。算法通过固定观测矩阵的最小二乘逆，得到 a0/a1/a2，再对全部模型点计算 u/v/w；x=0 的位移和转角均为零。矩阵只在算法实例创建时求逆，高频接口不输出报错。

模态运动为：

```text
a0 = amplitude × sin(2π f t)
a1 = 0.35 × amplitude × sin(1.3 × 2π f t)
a2 = 0.20 × amplitude × sin(0.6 × 2π f t)
```

参考波长 λ0[i]=1550+0.5i nm。默认标定 `factor=1250 microstrain/nm, A=1, B=0`，12 路按 0～11 顺序、通道倍率为 1。发送端按 `λ = λ0 + εmicro/1250` 生成 float32；现有 StrainCalibration 按 `εmicro = factor*(λ-λ0)/A+B` 还原。float32 波长有量化误差，测试按相应容差比较重构位移，不要求解析后逐位等于理想连续信号。

PlateIFE 的 NodeDofs 为三个模态系数，内部单位 mm；calculateDisplacement 按现有 IFrameIFE 约定返回 m，CalculationWorker 再乘 1000 得到供显示、存储使用的 mm。

## 数据链路与生命周期

```text
PlateUdpSender → 真实本地 UDP → ConnectControl 拆包 → MPWavelengthParser
  → ProtocolControl::frameParsed → CalculationControl::submitFrame
  → 默认波长标定 → PlateIFE → 逐点插值 → CalculationRepository
  → VisualizationControl / 前端同一个 vtkRenderWindow
```

板模式使用新到达的完整 UDP 波长帧，等模型、配置、端口就绪后才提交计算，避免把旧设备 Repository 快照套用到仿真标定。原始波长由 submitFrame 写入存储，标定和位移仍由现有解算线程直接交给 StorageControl。返回 HYY 后恢复原有 Repository 轮询方式。模型、标定和模式切换经 Presenter → 验证 Backend 组织，不改动通用计算模块的算法默认值。

## 自动验证

```powershell
ctest --preset vtk-native-widgets-release
# 真实 OpenGL：自动启动、发送、计算和显示板，停止后逐点比较网格，再退出。
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe `
  --plate-smoke-test --screenshot ./build/vtk-native-widgets/plate-simulation.png | Out-Host
if ($LASTEXITCODE -ne 0) { throw '板仿真失败' }
```

`test_plate_simulation` 不依赖 OpenGL/VTK DLL：检查解析解、固支边界、波长量化误差、STL 尺寸/面数、UDP 到标定/IFE/CSV、实际发送周期、停止行为和独立发送器进程。OpenGL 冒烟检查实际 1394 个显示点与解算结果一致、颜色数组存在、固支边不动、真实 RenderWindow 指针一致及返回 HYY 时的清理。

Debug 也可编译并运行无窗口单元测试；原生窗口运行须配备对应 Debug VTK DLL。现有 `--smoke-test` 继续验证 HYY 原链路，与 `--plate-smoke-test` 分别运行。
