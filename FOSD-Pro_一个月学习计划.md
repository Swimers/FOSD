# FOSD-Pro 一个月学习计划

---

## 第 1 周：C++ 补基 + Qt 环境 + 项目跑起来

### 学习目标
- 巩固 C++ 面向对象、指针/引用、STL 容器基础
- 熟悉 Qt 的信号槽、事件循环、基本控件
- 成功编译并运行 FOSD-Pro 项目

### 任务清单

| 天数 | 理论学习 | 项目实践 |
|---|---|---|
| 第 1 天 | 复习 C++ 类、继承、多态、虚函数；`std::vector` / `QList` / `QMap` 区别 | 安装 Qt 5.12.2 + MSVC 2017 64-bit Kit；打开 `FOSD-Pro.pro` |
| 第 2 天 | 学习 `argc/argv`、Qt 的 `QApplication` / 事件循环 | 运行 `main.cpp`，理解 `QApplication a(argc, argv)` 与 `a.exec()` |
| 第 3 天 | Qt 信号槽机制（`SIGNAL/SLOT` 与 Qt5 函数指针写法） | 在 `OldMainWindow` 里找一个 `connect(...)`，画一张信号槽连接图 |
| 第 4 天 | `QThread` 基础、对象树、`moveToThread` | 阅读 `connectwindow.cpp` 的 `InitSerial/InitUdp/InitTcp`，理解「工作对象移到子线程」模式 |
| 第 5 天 | Qt Designer / `.ui` 文件、常用控件 | 打开 `oldmainwindow.ui`、`connectwindow.ui`，对照代码找控件名 |
| 第 6 天 | 项目构建流程：qmake → Makefile → nmake | 执行 Release/Debug 构建，确认 `build/` 目录生成 `.exe` |
| 第 7 天 | 本周复盘：整理项目目录结构笔记 | 输出：一份「FOSD-Pro 源码目录结构说明」文档 |

### 本周产出
- 项目成功编译运行截图
- 一张手绘/文本版的「窗口-线程-模块」关系图

---

## 第 2 周：通信模块 + 主窗口框架

### 学习目标
- 理解串口、UDP、TCP 三种通信方式的实现
- 理解 `connectwindow` 如何管理三个通信线程
- 熟悉 `OldMainWindow` 的页面切换逻辑

### 任务清单

| 天数 | 理论学习 | 项目实践 |
|---|---|---|
| 第 8 天 | Qt 串口编程：`QSerialPort` / `QSerialPortInfo` | 精读 `serialtask.h/.cpp`，画出「打开 → 读取 → 发射信号」流程 |
| 第 9 天 | Qt UDP 编程：`QUdpSocket` 绑定与收发 | 精读 `udptask.h/.cpp`，理解 `udpOpen / udpRecData` |
| 第 10 天 | Qt TCP Server 编程：`QTcpServer` / `QTcpSocket` | 精读 `tcptask.h/.cpp`，理解监听、新连接、断开、错误状态机 |
| 第 11 天 | 枚举类型 `enum`、信号槽跨线程传自定义类型 | 分析 `tcptask::ConnectionStatus` 如何跨线程传到 UI |
| 第 12 天 | `QButtonGroup` + `QStackedWidget` 页面切换 | 阅读 `oldmainwindow.cpp` 的 `InitOldMainWindow()`，理解三页切换 |
| 第 13 天 | Qt 样式表 QSS、无边框窗口、鼠标拖拽 | 阅读 `switchUiStyle`、`mouseMoveEvent`、`mousePressEvent` |
| 第 14 天 | 本周复盘 | 输出：三种通信方式的「输入 → 输出 → 信号」对比表 |

### 本周产出
- 三种通信方式的状态机/流程图
- 一份 `connectwindow` 中线程与对象生命周期的说明

---

## 第 3 周：协议解析 + IFE 算法 + 2D/3D 可视化

### 学习目标
- 理解 FBG 数据包格式与波长→应变转换
- 理解 IFE 逆有限元从应变到位移的计算链路
- 理解 2D 表格显示与 3D VTK 渲染

### 任务清单

| 天数 | 理论学习 | 项目实践 |
|---|---|---|
| 第 15 天 | 字节序、union 类型、`FLOATA` / `FLOATA2` | 精读 `pocketsconfig.h` 与 `analyzepackets::analyzeData()`，理解包头识别 |
| 第 16 天 | 协议解析：`processMain()` 与 `processChannel()` | 计算主包中一个传感器数据的字节偏移，验证 `DataAddress = 24` |
| 第 17 天 | 波长-应变公式：`(0.00075*(λ-λ₀))/A+B` | 在 `connectwindow` 中找到 A/B 参数来源，理解 `set_strain_AB` |
| 第 18 天 | 逆有限元思想：应变 → 节点位移 | 阅读 `IFE::calculateDisp()`，理解 `aa_k_New[20][12]` × `sortedstrain[12]` |
| 第 19 天 | 形函数与坐标映射 | 对比 `ShapeFunction_New_HYY` 与 `ShapeFunction_New_ZB`，理解梁单元形函数 |
| 第 20 天 | VTK 基础：`vtkRenderer` / `vtkActor` / `vtkPolyDataMapper` | 阅读 `disp3dmodel.h/.cpp`，理解 STL 加载 → Actor → RenderWindow 流程 |
| 第 21 天 | QCustomPlot 曲线绘制 | 阅读 `connectwindow::InitCustomPlot()`，理解频谱图初始化 |

### 第 22~23 天本周复盘
- 输出：从「原始字节」到「3D 位移场」的完整数据流图

### 本周产出
- 一张完整的数据流 ASCII 图
- 标注每个关键数组含义：`strain_float[]` / `sortedstrain[]` / `node_u[]` / `positionDisp[][]`

---

## 第 4 周：综合实践 + 最终交付

### 任务 1：通信数据日志记录器（基础任务）

在 `connectwindow` 的串口/UDP/TCP 标签页中，为每种通信方式增加「开始记录 / 停止记录」按钮，将收发数据逐行写入以日期命名的 `.txt` 文件。

### 任务 2：全环节日志系统（拓展任务）

基于单例 + 生产者-消费者 + 独立工作线程的线程安全日志架构，构建覆盖通信层、协议层、IFE 算法层、UI 层、存储层的统一日志子系统，支持双重输出（文件持久化 + 子界面实时显示）、多级别多模块过滤，并在程序退出时保证队列数据全部落盘。

---

## 交付物清单

1. 项目编译运行的环境配置记录
2. 手绘版架构图（模块关系 + 数据流）
3. 通信日志记录器功能（按钮可用、文件可读）
4. 全环节日志系统（Logger 单例 + LogWindow 子界面 + 日志文件）
5. 日志集成点覆盖对照表（标记每个模块哪些入口加了日志）
