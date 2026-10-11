# FOSD 构建结构说明

本文档描述 FOSD 项目当前的 CMake 构建组织方式：有哪些构建目标、它们之间如何链接、如何配置与编译，以及新增模块时应遵循的约定。

> 适用范围：仓库根目录 `FOSD-ReArc` 分支下的当前状态。文中所有路径均相对于仓库根目录。

---

## 1. 目录结构

```
FOSD-ReArc/
├── CMakeLists.txt              # 顶层构建脚本：全局设置、依赖查找、聚合子目录
├── CMakePresets.json           # 预设：一条命令完成 configure / build / test
├── cmake/
│   └── Windows.cmake           # 平台相关设置（MSVC），被顶层 include
├── src/                        # 业务模块
│   ├── CMakeLists.txt          # 模块聚合入口
│   ├── core/                   # 共享数据模型与读写锁仓库
│   │   └── Datamodel/
│   ├── protocol/               # 独立线程解析与协议分派
│   └── communication/          # 通信与拆帧模块
│       ├── CMakeLists.txt
│       ├── ConnectControl.{h,cpp}   # 对外门面：线程 + 信号槽
│       ├── ITransport.h             # 传输层抽象接口
│       ├── IPacketDecoder.h         # 拆包器接口
│       ├── ITransportTypes.h        # 公共类型（Config / Endpoint / Error）
│       ├── PacketDecoders.{h,cpp}    # UDP/TCP/串口拆包器与规则
│       ├── Tcp.{h,cpp}
│       ├── Udp.{h,cpp}
│       ├── Usart.{h,cpp}
│       └── Serialport.{h,cpp}        # 当前 ConnectControl 使用的串口实现
├── apps/                       # 可执行程序
│   ├── FOSD-Win/               # Windows 桌面端
│   │   ├── CMakeLists.txt
│   │   └── FOSD.{h,cpp}, main.cpp
│   └── FOSD-RK3588/            # 预留，暂未接入构建
├── test/
│   ├── CMakeLists.txt
│   ├── communication_tests.cpp # 通信测试
│   ├── packet_decoder_tests.cpp # 拆包测试
│   └── protocol_tests.cpp      # 协议、共享数据与全链路测试
├── cmake/ configs/ doc/ lib/ res/ ui/   # 资源与第三方库
└── build/                      # 生成目录（已 gitignore）
```

`src/` 下的 `config/`、`logging/`、`storage/`、`visualization/` 尚未接入构建。`core` 和 `protocol` 已实现并加入聚合目标。后续业务对接见 [协议与共享数据对接](protocol-integration.md)。

---

## 2. 构建目标一览

| 目标 | 类型 | 定义位置 | 说明 |
| --- | --- | --- | --- |
| `fosd_communication` | 静态库 | [src/communication/CMakeLists.txt](../src/communication/CMakeLists.txt) | 通信模块，封装 TCP / UDP / 串口 |
| `fosd_core` | 静态库 | [src/core/CMakeLists.txt](../src/core/CMakeLists.txt) | 共享数据类型和只读快照仓库 |
| `fosd_protocol` | 静态库 | [src/protocol/CMakeLists.txt](../src/protocol/CMakeLists.txt) | 协议解析、数据发布和通知 |
| `FOSD` | 可执行程序 (WIN32) | [apps/FOSD-Win/CMakeLists.txt](../apps/FOSD-Win/CMakeLists.txt) | Windows 桌面应用 |
| `test_communication` | 可执行程序 (控制台) | [test/CMakeLists.txt](../test/CMakeLists.txt) | `communication` 模块单元测试，注册到 CTest |
| `test_packet_decoders` | 可执行程序 (控制台) | [test/CMakeLists.txt](../test/CMakeLists.txt) | 拆帧规则与错误信号测试 |
| `test_protocol` | 可执行程序 (控制台) | [test/CMakeLists.txt](../test/CMakeLists.txt) | 协议解析、并发读取与真实通信全链路测试 |

---

## 3. 目标依赖关系

```
        Qt5::Widgets ─┐
                      ├──► FOSD  (apps/FOSD-Win)
   fosd_communication ─┤
        fosd_protocol ─┘

   fosd_communication ────► test_communication / test_packet_decoders
        Qt5::Test ────────┘

   fosd_protocol ────────► test_protocol
        Qt5::Test ────────┘

   fosd_protocol ──PUBLIC──► fosd_core / fosd_communication / Qt5::Core
   fosd_core ──────PUBLIC──► Qt5::Core

   fosd_communication
        │  PUBLIC 链接
        ├── Qt5::Core
        ├── Qt5::Network
        └── Qt5::SerialPort
```

要点：

- **模块以静态库形式提供**，链接名为 `fosd_communication`、`fosd_core`、`fosd_protocol`，公共头文件根目录统一为 `src/`。
- Qt 依赖声明为 `PUBLIC`，因为 [ITransportTypes.h](../src/communication/ITransportTypes.h) 在公共接口上直接使用了 `QSerialPort`、`QHostAddress`、`QString` 等 Qt 类型——使用方无需重复声明这些依赖。
- protocol 的公共接口包含通信来源及 core 数据类型，因此这两个模块依赖也声明为 `PUBLIC`。core 不链接 protocol 或 communication。
- 应用与测试都**不需要**直接写 Qt 的 `target_link_libraries` 来获取通信能力，只需链接 `fosd_communication` 即可（`FOSD` 额外需要 `Qt5::Widgets`，测试额外需要 `Qt5::Test`）。

### 头文件包含约定

通信模块的头文件被**外部**以模块名为前缀包含：

```cpp
#include "communication/ConnectControl.h"   // 测试与应用的写法
```

而模块**内部**使用同目录短包含：

```cpp
#include "ConnectControl.h"                  // 模块内部文件的写法
```

两种写法能同时成立，是因为 `fosd_communication` 通过以下语句把 `src/` 目录设成了 `PUBLIC` 包含路径：

```cmake
target_include_directories(fosd_communication
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
```

新增模块时应沿用同样的约定：公共包含根永远是 `src/`，模块内部文件用短包含。

---

## 4. 各 CMakeLists 的职责

| 文件 | 职责 |
| --- | --- |
| [CMakeLists.txt](../CMakeLists.txt) | `project()`、C++ 标准、AUTOMOC/UIC/RCC、输出目录、`find_package(Qt5)`、`include(Windows)`，最后按顺序 `add_subdirectory(src)` → `apps/FOSD-Win` → `test` |
| [cmake/Windows.cmake](../cmake/Windows.cmake) | MSVC 专属编译选项（`/utf-8`、`/MP`），用 `if(NOT MSVC) return()` 保护，非 Windows 工具链自动跳过 |
| [src/CMakeLists.txt](../src/CMakeLists.txt) | 聚合 communication、core、protocol 模块 |
| [src/communication/CMakeLists.txt](../src/communication/CMakeLists.txt) | 声明 `fosd_communication`、公共包含路径、Qt 依赖 |
| [src/core/CMakeLists.txt](../src/core/CMakeLists.txt) | 声明 `fosd_core` 和 Qt Core 依赖 |
| [src/protocol/CMakeLists.txt](../src/protocol/CMakeLists.txt) | 声明 `fosd_protocol`，PUBLIC 链接 core 与 communication |
| [apps/FOSD-Win/CMakeLists.txt](../apps/FOSD-Win/CMakeLists.txt) | 声明 `FOSD` 并链接 communication、protocol 与 Widgets |
| [test/CMakeLists.txt](../test/CMakeLists.txt) | 提供 `fosd_add_test()` 封装，声明测试目标并注册到 CTest |

---

## 5. 构建选项与变量

| 变量 | 类型 | 默认值 | 作用 |
| --- | --- | --- | --- |
| `FOSD_BUILD_TESTS` | option | `ON` | 是否查找 `Qt5::Test`、启用 CTest 并构建 `test/` |
| `FOSD_QT_ROOT` | cache PATH | `d:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64` | Qt5 kit 目录，会追加到 `CMAKE_PREFIX_PATH` |

`FOSD_QT_ROOT` 是 CACHE 变量，Qt 装在别处时在配置阶段覆盖即可：

```bash
cmake -S . -B build -DFOSD_QT_ROOT=E:/Qt/5.12.2/msvc2017_64
```

关闭测试：

```bash
cmake -S . -B build -DFOSD_BUILD_TESTS=OFF
```

---

## 6. 环境要求

| 组件 | 当前使用版本 | 说明 |
| --- | --- | --- |
| CMake | 按所用命令选择 | 顶层最低要求 **3.16**；当前预设文件声明最低 **3.20**，旧版 VS 自带 CMake 3.12 不满足顶层要求 |
| Visual Studio | 2017 (v141, x64) | 生成器 `Visual Studio 15 2017` |
| Qt | 5.12.2 (`msvc2017_64`) | 需要 Core / Widgets / Network / SerialPort，测试另需 Test |

---

## 7. 构建与测试

### 方式一：直接调用

```bash
cmake -S . -B build -G "Visual Studio 15 2017" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

> VS 生成器是**多配置**的，Debug/Release 共用同一份 configure 结果，因此不必为不同配置准备不同构建目录。

### 方式二：使用预设（当前文件要求 CMake ≥ 3.20）

```bash
cmake --preset windows-msvc2017     # 配置
cmake --build --preset windows-debug    # 构建（或 windows-release）
ctest --preset windows-debug            # 测试
```

预设定义见 [CMakePresets.json](../CMakePresets.json)：

- configure：`windows-msvc2017` —— VS 2017 x64，`binaryDir` 为 `${sourceDir}/build`
- build：`windows-debug`、`windows-release`
- test：`windows-debug`

---

## 8. 产物布局

顶层把三类产物统一收敛到固定目录，避免散落在各子目录：

```
build/
├── bin/<Config>/                 # 可执行程序
│   ├── FOSD.exe
│   ├── test_communication.exe
│   ├── test_packet_decoders.exe
│   └── test_protocol.exe
└── lib/<Config>/                 # 库
    ├── fosd_communication.lib
    ├── fosd_core.lib
    └── fosd_protocol.lib
```

由以下全局变量控制：

```cmake
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
```

---

## 9. 扩展指南

### 新增一个业务模块

1. 在 `src/<module>/` 下放源码，并新建 `CMakeLists.txt`：

    ```cmake
    add_library(fosd_<module> STATIC
        Foo.cpp
        Foo.h
    )

    target_include_directories(fosd_<module>
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

    target_link_libraries(fosd_<module>
        PUBLIC Qt5::Core
        PRIVATE fosd_communication)   # 按需声明模块间依赖
    ```

2. 在 [src/CMakeLists.txt](../src/CMakeLists.txt) 中加一行 `add_subdirectory(<module>)`。

> 注意：**只链接真实存在的目标**。模块间依赖用 `PRIVATE`（实现细节）还是 `PUBLIC`（公共头文件里出现）取决于该依赖是否暴露在接口上。

### 新增一个测试

在 [test/CMakeLists.txt](../test/CMakeLists.txt) 末尾追加：

```cmake
fosd_add_test(test_<name> <name>_tests.cpp)
target_link_libraries(test_<name> PRIVATE fosd_<module>)
```

`fosd_add_test()` 已自动完成三件事：链接 `Qt5::Test`、强制控制台子系统（否则 QtTest 输出不可见）、注册到 CTest。

### 新增一个平台

参照 [cmake/Windows.cmake](../cmake/Windows.cmake) 的做法新建 `cmake/<Platform>.cmake`，全部内容用条件判断保护，再从顶层 include 进来。（`apps/FOSD-RK3588/` 预留了该方向的空目录。）

---

## 10. 已知限制

- 命令行环境需提供满足要求的 CMake；若 PATH 指向 VS 2017 自带的旧版本，应改用已安装的新版本可执行文件。Qt DLL 也需在测试运行时可找到。
- `src/` 下的 `config/`、`logging/`、`storage/`、`visualization/` 尚未纳入构建；不要提前链接不存在的模块目标。
- `apps/FOSD-RK3588/` 为预留目录，未接入构建。
- 通信模块为纯 C++/Qt 实现，未依赖任何尚未实现的模块，因此 `fosd_communication` 目前没有对外的模块间依赖。
- protocol 当前只解析参考文本中明确的 `0xA55A` 波长协议；共享仓库仅保存最新快照。完整接口及后续扩展约束见 [协议与共享数据对接](protocol-integration.md)。
