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
│   └── communication/          # 通信模块（目前唯一已实现）
│       ├── CMakeLists.txt
│       ├── ConnectControl.{h,cpp}   # 对外门面：线程 + 信号槽
│       ├── ITransport.h             # 传输层抽象接口
│       ├── IPacketDecoder.h         # 拆包器接口
│       ├── TransportTypes.h         # 公共类型（Config / Endpoint / Error）
│       ├── Tcp.{h,cpp}
│       ├── Udp.{h,cpp}
│       └── Usart.{h,cpp}
├── apps/                       # 可执行程序
│   ├── FOSD-Win/               # Windows 桌面端
│   │   ├── CMakeLists.txt
│   │   └── FOSD.{h,cpp}, main.cpp
│   └── FOSD-RK3588/            # 预留，暂未接入构建
├── test/
│   ├── CMakeLists.txt
│   └── communication_tests.cpp # QtTest 单元测试
├── cmake/ configs/ doc/ lib/ res/ ui/   # 资源与第三方库
└── build/                      # 生成目录（已 gitignore）
```

`src/` 下另有 `config/`、`core/`、`logging/`、`protocol/`、`storage/`、`visualization/` 六个**空占位目录**，目前尚未接入构建（没有源文件与 CMakeLists）。

---

## 2. 构建目标一览

| 目标 | 类型 | 定义位置 | 说明 |
| --- | --- | --- | --- |
| `fosd_communication` | 静态库 | [src/communication/CMakeLists.txt](../src/communication/CMakeLists.txt) | 通信模块，封装 TCP / UDP / 串口 |
| `FOSD` | 可执行程序 (WIN32) | [apps/FOSD-Win/CMakeLists.txt](../apps/FOSD-Win/CMakeLists.txt) | Windows 桌面应用 |
| `test_communication` | 可执行程序 (控制台) | [test/CMakeLists.txt](../test/CMakeLists.txt) | `communication` 模块单元测试，注册到 CTest |

---

## 3. 目标依赖关系

```
        Qt5::Widgets ─┐
                      ├──► FOSD  (apps/FOSD-Win)
   fosd_communication ─┘

   fosd_communication ────► test_communication
        Qt5::Test ────────┘

   fosd_communication
        │  PUBLIC 链接
        ├── Qt5::Core
        ├── Qt5::Network
        └── Qt5::SerialPort
```

要点：

- **模块以静态库形式提供**，对外只暴露两个东西：链接名 `fosd_communication`，以及公共头文件根目录 `src/`。
- Qt 依赖声明为 `PUBLIC`，因为 [TransportTypes.h](../src/communication/TransportTypes.h) 在公共接口上直接使用了 `QSerialPort`、`QHostAddress`、`QString` 等 Qt 类型——使用方无需重复声明这些依赖。
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
| [src/CMakeLists.txt](../src/CMakeLists.txt) | 聚合业务模块，目前只 `add_subdirectory(communication)` |
| [src/communication/CMakeLists.txt](../src/communication/CMakeLists.txt) | 声明 `fosd_communication`、公共包含路径、Qt 依赖 |
| [apps/FOSD-Win/CMakeLists.txt](../apps/FOSD-Win/CMakeLists.txt) | 声明 `FOSD` 可执行程序并链接 `fosd_communication` |
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
| CMake | 3.17.1 | 顶层最低要求 **3.16**；使用 `CMakePresets.json` 需要 **3.19+**（见第 9 节） |
| Visual Studio | 2017 (v141, x64) | 生成器 `Visual Studio 15 2017` |
| Qt | 5.12.2 (`msvc2017_64`) | 需要 Core / Widgets / Network / SerialPort，测试另需 Test |

---

## 7. 构建与测试

### 方式一：直接调用（任意 CMake ≥ 3.16 均可）

```bash
cmake -S . -B build -G "Visual Studio 15 2017" -A x64
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

> VS 生成器是**多配置**的，Debug/Release 共用同一份 configure 结果，因此不必为不同配置准备不同构建目录。

### 方式二：使用预设（需要 CMake ≥ 3.19）

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
│   └── test_communication.exe
└── lib/<Config>/                 # 库
    └── fosd_communication.lib
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

- **`CMakePresets.json` 当前在本机无法生效。** 预设文件需要 CMake ≥ 3.19，而开发机 PATH 中的 CMake 为 3.17.1、VS 2017 自带的是 3.12，均不满足。在此之前请使用第 7 节的「方式一」直接调用；升级 CMake 后预设即可直接使用。
- `src/` 下的 `config/`、`core/`、`logging/`、`protocol/`、`storage/`、`visualization/` 仍为空目录，未纳入构建，`src/CMakeLists.txt` 中也尚未 `add_subdirectory`。
- `apps/FOSD-RK3588/` 为预留目录，未接入构建。
- 通信模块为纯 C++/Qt 实现，未依赖任何尚未实现的模块，因此 `fosd_communication` 目前没有对外的模块间依赖。
