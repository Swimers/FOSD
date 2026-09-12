# FOSD-Pro

FOSD-Pro（Fiber Optic Sensing Demodulator）是一套面向光纤传感解调仪的 Windows 上位机软件，负责完成设备通信、解调数据接收与解析、应变数据处理、二维数据显示、逆有限元（IFE）形变计算以及基于 VTK 的三维模型形变可视化。

当前主程序默认直接启动旧版主界面 `OldMainWindow`，并将“解调仪通信 / 2D 数据 / 3D 可视化”三个功能页面组织在同一主窗口中。仓库中同时保留了面向特定项目的 `RainbowMainWindow`、插件 DLL 入口以及独立的 `PluginHost` 测试宿主。

## 核心功能

### 1. 解调仪通信与数据采集

`src/app/conncect` 负责通信控制界面，底层通信任务位于 `src/Communication`。

目前支持：

- USART / 串口通信；
- TCP 通信；
- UDP 通信；
- 本机 IP 与网络参数配置；
- 通信状态监控与诊断信息显示；
- 数据接收、发送及调试；
- 通信任务独立线程运行，减少 UI 阻塞。

主要类：

- `connectwindow`：通信配置、数据采集及监控界面；
- `serialtask`：串口打开、关闭、发送和接收；
- `tcptask`：TCP 通信任务；
- `udptask`：UDP 通信任务。

### 2. 解调数据解析与应变计算

`src/protocol/analyzepackets.*` 负责对接收到的解调仪数据包进行解析和通道处理。

主要功能包括：

- 多通道数据解析；
- 波长数据提取；
- 初始波长保存与复位；
- 波长到应变数据的转换；
- 传感器通道及数据点管理；
- 自动/手动数据保存；
- 向后续 2D / IFE 模块提供实时应变数组。

主界面中的数据流大致为：

```text
Serial / TCP / UDP
        │
        ▼
  connectwindow
        │
        ▼
  analyzepackets
        │
        ├──► 波长 / 应变数据显示与保存
        │
        ▼
   strain_float[]
        │
        ▼
     2D / IFE
        │
        ▼
     VTK 3D
```

### 3. 二维数据与 IFE 计算

`src/app/disp2d` 为二维观测与参数设置界面，核心计算位于 `src/core/IFE`。

`disp2dwindow` 从通信模块获取实时应变数据，并将数据传递给 `IFE` 对象。`IFE` 中保存传感器数量、标记点数量以及逆有限元计算所需的矩阵参数，用于根据应变信息求解结构节点/标记点的位移或形变结果。

当前代码中默认：

- `MARKPOINTNUM = 12`；
- `SensorNum = 12`。

相关参数与输入数据需要和实际模型及数据文件保持一致。

### 4. VTK 三维模型与形变可视化

`src/app/disp3d` 与 `src/core/model` 共同负责三维显示。

主要能力包括：

- 加载、清空、合并和导出模型；
- 实时刷新模型形变；
- 关键点编辑与选择；
- Tracking Point 管理和数据查看；
- 标量映射及色彩图显示；
- 自定义标量范围与归一化显示；
- 坐标轴、网格以及 XOY / XOZ / ZOY 平面控制；
- 前后左右等视角切换；
- 相机锁定与跟随；
- 模型颜色设置；
- FPS 刷新频率控制。

VTK 渲染相关类主要位于：

```text
src/core/model/
├── disp3dmodel.*              # 三维模型管理
├── disp3dbackground.*         # 三维场景背景/辅助显示
├── disp3dinteractor.*         # VTK 交互逻辑
├── disp3dfreshmodelthread.*   # 模型刷新相关线程
├── trackingpoint.*            # 跟踪点数据
└── disp3dglobal.h
```

### 5. 数据存储

`src/utils/Storage/DataStorageThread.*` 提供独立的数据写盘线程。

该模块采用生产者-消费者模式，通过线程安全队列缓存采集数据，并批量写入文件，避免磁盘 IO 阻塞主界面和采集线程。

当前数据结构中包含：

- 12 路应变数据；
- 12 路温度数据；
- 16 路扩展数据；
- 时间戳。

默认批量写入参数：

- `BATCH_SIZE = 50`；
- `MAX_WAIT_MS = 500 ms`。

### 6. Rainbow 项目分支与测试数据注入

`src/app/mainwindow/rainbowmainwindow.*` 和 `src/core/RainbowIFE` 是当前仓库中另一套面向特定项目的主界面与 IFE 实现。

`RainbowMainWindow` 目前直接嵌入 3D 显示窗口，并通过 `TestDataInjector` 支持测试数据注入。代码中仍存在开发机上的硬编码测试数据路径，因此这一入口更适合作为开发/实验代码使用，部署前应改为配置文件或用户可选路径。

### 7. 插件模式

除独立 EXE 外，仓库还提供 `FOSD-plugin.pro` 与 `src/fosdplugin.cpp`，可将 FOSD 界面编译为 DLL 插件。

插件向宿主导出以下 C 接口：

```text
dllStartPlugin
dllInitialisePlugin
dllStopPlugin
dllShutdownPlugin
```

`PluginHost/` 是一个 Windows 测试宿主，通过 `LoadLibrary` / `GetProcAddress` 动态加载 `FOSD_ProPlugin.dll` 并调用上述接口，可用于验证插件的加载、初始化、停止和卸载流程。

## 技术栈

- C++ 11 / C；
- Qt 5.12.2；
- Qt Widgets；
- Qt Network；
- Qt SerialPort；
- Qt OpenGL；
- VTK 8.2；
- QCustomPlot；
- CMake / qmake；
- MSVC 2017；
- Windows 10+。

## 项目结构

```text
FOSD_Project/
├── CMakeLists.txt                 # CMake 主构建脚本
├── CMakePresets.json              # CMake 默认构建预设
├── FOSD-Pro.pro                   # qmake：独立 EXE
├── FOSD-plugin.pro                # qmake：插件 DLL
├── PluginHost/                    # DLL 插件测试宿主
├── data/                          # 运行时数据、QSS 等外部资源
├── doc/                           # 项目文档
├── installer/                     # 安装/部署相关文件
├── lib/
│   ├── CustomUi/                  # 自定义 Qt 控件
│   └── VTK/                       # VTK Debug / Release 库
├── res/                           # Qt Resource 资源
└── src/
    ├── main.cpp                   # 独立程序入口
    ├── fosdplugin.cpp             # 插件 DLL 入口
    ├── app/
    │   ├── conncect/              # 通信控制与数据采集界面
    │   ├── disp2d/                # 二维数据显示与参数设置
    │   ├── disp3d/                # VTK 三维显示及 Dock 面板
    │   ├── mainwindow/            # Old / Rainbow 主界面
    │   └── user/                  # 登录界面
    ├── Communication/             # Serial / TCP / UDP 通信任务
    ├── core/
    │   ├── IFE/                   # 原 IFE 计算模块
    │   ├── RainbowIFE/            # Rainbow IFE 与标定相关模块
    │   └── model/                 # VTK 模型、交互、Tracking Point
    ├── protocol/                  # 解调仪协议解析
    ├── test/                      # 数据注入与存储等测试代码
    └── utils/
        ├── QCustomPlot/           # 2D 绘图库
        ├── Storage/               # 异步数据存储
        ├── Timers/                # 定时器辅助模块
        ├── config/                # 配置管理
        ├── database/              # 数据库相关代码
        └── global/                # 全局数据/工具
```

> 注意：源码目录当前实际名称为 `src/app/conncect` 和 `src/Communication`，其中 `conncect` 为历史拼写，修改目录名时需要同步更新 qmake、CMake 以及 `#include` 路径。

## 程序结构与数据关系

当前默认 EXE 的主要对象关系如下：

```text
main.cpp
  │
  └── OldMainWindow
       │
       ├── connectwindow
       │     ├── serialtask (QThread)
       │     ├── tcptask    (QThread)
       │     ├── udptask    (QThread)
       │     └── analyzepackets
       │
       ├── disp2dwindow
       │     └── IFE
       │
       └── disp3dwindow
             ├── disp3dmodel
             ├── disp3dbackground
             ├── disp3dinteractor
             ├── disp3dfreshmodelthread
             ├── trackingpoint
             └── Dock Panels
```

其中 `OldMainWindow` 将通信模块的应变数组传给 `disp2dwindow`，同时将 `disp2dwindow` 中的 `IFE` 对象传给 `disp3dwindow`，因此三个页面并不是互相独立，而是构成一条连续的数据处理与可视化链路。

## 构建环境

推荐使用与现有工程一致的环境：

- Windows 10 / 11；
- Visual Studio 2017 / MSVC 2017 x64；
- Qt 5.12.2 MSVC 2017 64-bit；
- VTK 8.2；
- CMake 3.16+。

CMake 当前会优先检测：

```text
D:/Qt/5.12.2/msvc2017_64
```

VTK 默认从仓库内以下目录查找：

```text
lib/VTK/VTK-release
lib/VTK/VTK-debug
```

如果本机 Qt 安装位置不同，请通过 `CMAKE_PREFIX_PATH` 指定实际 Qt 路径。

## CMake 构建

仓库提供了 `CMakePresets.json`，默认构建目录为：

```text
build/CMake
```

可执行：

```bash
cmake --preset default
cmake --build --preset default --config Release
```

构建产物按配置输出至：

```text
build/CMake/bin/debug
build/CMake/bin/release
```

构建结束后，CMake 会自动将 `data/` 目录复制到可执行文件目录。

也可以继续使用原有的 qmake 工程：

```text
FOSD-Pro.pro
```

## 当前程序入口

`src/main.cpp` 当前默认执行：

```cpp
OldMainWindow *m_MainWindow = new OldMainWindow("1");
m_MainWindow->show();
```

因此启动后直接以管理员权限进入旧版 FOSD 主界面。

登录窗口以及 `RainbowMainWindow` 的入口代码目前仍保留，但已被注释。如需切换主界面，应同步确认对应数据源、IFE 实现和测试代码是否满足实际部署环境。

## 开发注意事项

1. IFE 中存在和具体结构模型绑定的矩阵、传感器数量及标记点数量，不能简单作为通用算法参数使用；更换结构或传感器布置时需要重新标定。
2. `RainbowMainWindow` 中存在本地硬编码测试数据路径，正式部署前需要移除。
3. 仓库当前包含部分历史构建产物和 `PluginHost` 的 Debug / Release 文件，后续可进一步清理并完善 `.gitignore`。
4. 当前工程同时维护 CMake、`FOSD-Pro.pro` 和 `FOSD-plugin.pro`，新增或删除源码时需要检查三套构建配置是否同步。
5. VTK 版本与 Qt / MSVC ABI 需要保持一致，Debug 与 Release 库不可混用。

## 贡献规范

建议提交信息使用：

```text
feat: 新功能
fix: Bug 修复
docs: 文档修改
refactor: 代码重构
style: 格式调整
test: 测试修改
build: 构建系统修改
```

## 致谢

感谢 VTK、Qt、QCustomPlot 等开源项目提供的技术支持，以及项目开发过程中各位老师和同学的指导与帮助。

## 作者

- Author: SuningJan
- Repository: `Swimers/FOSD_Project`
