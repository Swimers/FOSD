# FOSD-Pro 的 CMake 多 Target 结构

## 结论

当前配置把 `src/utils` 的工具代码拆成独立 Target，主程序不再使用 `set(FOSD_SOURCES ...)` 汇总全部文件。应用、通信、核心、协议和测试辅助源码分别在自己的目录中用 `target_sources()` 加入 `FOSD-Pro`；工具库则用 `target_link_libraries()` 连接到主程序。

## 文件与 Target

```text
CMakeLists.txt                  项目、依赖、主程序及目录入口
lib/CustomUi/CMakeLists.txt     fosd_custom_ui 静态库
src/app/CMakeLists.txt          应用界面源码 → FOSD-Pro
src/Communication/CMakeLists.txt 通信源码 → FOSD-Pro
src/core/CMakeLists.txt         核心源码 → FOSD-Pro
src/protocol/CMakeLists.txt     协议源码 → FOSD-Pro
src/test/CMakeLists.txt         原有测试辅助源码 → FOSD-Pro
src/utils/CMakeLists.txt        六个工具 Target
```

| Target | 类型 | 负责的目录/内容 |
| --- | --- | --- |
| `FOSD-Pro` | 可执行程序 | 入口、Qt 资源和各业务目录的源码 |
| `fosd_custom_ui` | 静态库 | `lib/CustomUi` 的自定义控件 |
| `fosd_plot` | 静态库 | `utils/QCustomPlot` 绘图组件 |
| `fosd_storage` | 静态库 | `utils/Storage` 数据存储线程 |
| `fosd_config` | 静态库 | `utils/config` 配置读写 |
| `fosd_database` | 静态库 | `utils/database` 数据库封装 |
| `fosd_global` | 静态库 | `utils/global` 全局共享状态 |
| `fosd_timers` | INTERFACE 库 | `utils/Timers`，只有头文件，不产生 `.lib` |
| `fosd_build_options` | INTERFACE 库 | 各 Target 共用的 C++11、宏和编译器选项 |

## 构建过程如何连接

根目录先查找 Qt 和 VTK，再创建 `FOSD-Pro`，随后用 `add_subdirectory()` 进入模块配置：

```cmake
add_executable(FOSD-Pro WIN32
    src/main.cpp
    src/fosdplugin.cpp
    ${FOSD_RESOURCES}
)

add_subdirectory(lib/CustomUi)
add_subdirectory(src/app)
add_subdirectory(src/Communication)
add_subdirectory(src/core)
add_subdirectory(src/protocol)
add_subdirectory(src/test)
add_subdirectory(src/utils)
```

业务目录中的 `target_sources(FOSD-Pro PRIVATE ...)` 把源文件加入已有的可执行目标。例如 `src/protocol/CMakeLists.txt`：

```cmake
target_sources(FOSD-Pro PRIVATE
    analyzepackets.cpp
    analyzepackets.h
    pocketsconfig.h
)
```

工具目录使用真正独立的库 Target。例如绘图库：

```cmake
add_library(fosd_plot STATIC
    QCustomPlot/qcpdocumentobject.cpp
    QCustomPlot/qcpdocumentobject.h
    QCustomPlot/qcustomplot.cpp
    QCustomPlot/qcustomplot.h
)
target_include_directories(fosd_plot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/QCustomPlot)
target_link_libraries(fosd_plot
    PUBLIC Qt5::Widgets Qt5::PrintSupport
    PRIVATE fosd_build_options
)
```

`STATIC` 会生成静态库。`PUBLIC` 头文件路径和 Qt 依赖会传递给链接 `fosd_plot` 的主程序；`PRIVATE` 编译设置只用于该库自身。`fosd_timers` 只有 `Timer.h`，所以使用 `INTERFACE` 传递头文件路径，不创建空的静态库。根目录最终用 `target_link_libraries(FOSD-Pro PRIVATE ...)` 连接所有工具 Target、Qt 和 VTK。

`fosd_build_options` 把共同设置放在一个 INTERFACE Target：

```cmake
add_library(fosd_build_options INTERFACE)
target_compile_features(fosd_build_options INTERFACE cxx_std_11)
target_compile_definitions(fosd_build_options INTERFACE QT_DEPRECATED_WARNINGS)
if(MSVC)
    target_compile_options(fosd_build_options INTERFACE /utf-8)
else()
    target_compile_options(fosd_build_options INTERFACE -Wall)
endif()
```

每个有源码的项目 Target 都链接它，因此拆库后仍保留原来的编译宏和编译器选项。根目录的 `CMAKE_AUTOMOC`、`CMAKE_AUTOUIC` 在创建各 Target 前设置，Qt 的 MOC/UIC 会分别处理各库和主程序中的相关文件。

## 以后如何新增文件

- 新增应用、核心或协议源码：改对应目录的 `CMakeLists.txt` 中的 `target_sources()`。
- 新增现有工具库的源码：改 `src/utils/CMakeLists.txt` 中对应的 `add_library()`。
- 新增工具模块：在 `src/utils/CMakeLists.txt` 定义新 Target、声明自己的公开头文件路径及直接依赖，再在根目录 `target_link_libraries(FOSD-Pro ...)` 中连接它。
- 含 `Q_OBJECT` 的头文件应列入其所属 Target；含 `.ui` 的界面文件也应与对应 `.cpp` 放在同一 Target。

## 配置与构建

以下命令使用本机验证环境：CMake 3.17.1、Visual Studio 2017 x64、Qt 5.12.2。其他机器需调整 `Qt5_DIR`。

```powershell
cmake -S . -B build/cmake-modular -G "Visual Studio 15 2017 Win64" -DQt5_DIR=D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/lib/cmake/Qt5
cmake --build build/cmake-modular --config Release --parallel 4
cmake --build build/cmake-modular --config Debug --parallel 4
```

`-S` 是源码目录，`-B` 是构建目录。产物现在相对于 `CMAKE_BINARY_DIR` 输出，所以上述命令生成 `build/cmake-modular/bin/release/FOSD-Pro.exe` 和 `build/cmake-modular/bin/debug/FOSD-Pro.exe`；工具库位于对应的 `lib` 子目录。这样不同 `-B` 目录互不覆盖。

仓库的 `CMakePresets.json` 使用格式版本 3；本机 CMake 3.17.1 不支持 Preset 命令，因此上面使用显式命令。当前 VTK 查找仍针对仓库中的 Windows `.lib` 文件，跨平台依赖配置需另行处理。
