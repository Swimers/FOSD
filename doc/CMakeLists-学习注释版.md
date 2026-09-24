# 重构前 CMakeLists.txt 中文注释版

这份文档保留了重构前的单目标配置，用于对照学习；下文的构建命令和输出路径仅对应旧配置。当前的多 Target 结构与构建命令见 [CMake 模块化结构说明](CMake-模块化结构说明.md)。代码块中的 `#` 是 CMake 注释。

## 阅读顺序

环境与项目声明 → 输出目录 → 查找 Qt/VTK → 源文件与资源 → 创建目标 → 编译和链接设置 → 构建后复制与安装。

## 完整配置与注释

```cmake
# 指定能解析本文件的最低 CMake 版本；低于 3.16 时配置直接停止。
cmake_minimum_required(VERSION 3.16)

# 声明项目名、项目版本和使用的语言。VERSION 是项目版本，不是 CMake 版本。
project(FOSD-Pro VERSION 1.0 LANGUAGES C CXX)

# 将后续目标的默认 C++ 标准设为 C++11。
set(CMAKE_CXX_STANDARD 11)
# 要求编译器真正支持 C++11；不能静默退回较旧标准。
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 将当前源码目录和当前构建目录加入头文件搜索路径，便于包含 Qt 生成的头文件。
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 自动对使用 Q_OBJECT 等宏的 Qt 头文件运行 moc。
set(CMAKE_AUTOMOC ON)
# 自动从 .ui 文件生成 ui_*.h；配合目标中的 .ui 文件使用。
set(CMAKE_AUTOUIC ON)

# 提供 CMAKE_INSTALL_BINDIR 等标准安装目录变量。
include(GNUInstallDirs)

# CMAKE_CURRENT_SOURCE_DIR 是本 CMakeLists.txt 所在目录；此处固定产物根目录。
# 注意：即使用其他 -B 构建目录，exe 仍写到源码树下的 build/CMake。
set(FOSD_CMAKE_OUTPUT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/build/CMake")
# 为随后创建的目标设置默认输出目录：RUNTIME=可执行程序，LIBRARY=动态库，ARCHIVE=静态库/导入库，PDB=调试符号。
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/pdb")

# 遍历四种构建配置，给每种配置单独设置输出目录。
# 多配置生成器（如 Visual Studio）可用 --config Release 或 Debug 选择配置。
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    string(TOLOWER "${config}" config_lower)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/bin/${config_lower}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/lib/${config_lower}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/lib/${config_lower}")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/${config_lower}")
endforeach()

# 若开发机上存在该固定路径，就优先从那里查找 Qt。
# 其他机器可在配置命令中传入 -DQt5_DIR=实际路径，无需修改此处。
if(EXISTS "D:/Qt/5.12.2/msvc2017_64/lib/cmake/Qt5/Qt5Config.cmake")
    list(PREPEND CMAKE_PREFIX_PATH "D:/Qt/5.12.2/msvc2017_64")
endif()

# 查找 Qt 5.12 或兼容的较新 Qt 5，并要求列出的模块全部存在。
# REQUIRED 表示缺少依赖时直接配置失败；成功后可使用 Qt5::Core 等目标。
find_package(Qt5 5.12 REQUIRED COMPONENTS
    Core
    Gui
    Network
    OpenGL
    PrintSupport
    SerialPort
    Widgets
)

# 指向仓库中预编译的 Windows VTK 8.2 Release/Debug 目录。
set(FOSD_VTK_RELEASE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/VTK/VTK-release")
set(FOSD_VTK_DEBUG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/VTK/VTK-debug")

# 用 GLOB 搜索 Release 导入库；目前会匹配目录内所有符合命名规则的库。
file(GLOB FOSD_VTK_RELEASE_LIBS "${FOSD_VTK_RELEASE_ROOT}/lib/*-8.2.lib")
# 同样搜索 Debug 库；本变量在后文没有被使用，实际 Debug 匹配由下面的循环完成。
file(GLOB FOSD_VTK_DEBUG_LIBS "${FOSD_VTK_DEBUG_ROOT}/lib/*-8.2D.lib")

# 创建空列表，用来收集最终传给链接器的 VTK 库。
set(FOSD_VTK_LIBRARIES)
# 对每个 Release 库推导对应的 Debug 库名；存在则分别关联两种配置。
# optimized/debug 是 target_link_libraries 识别的配置关键字。
foreach(release_lib IN LISTS FOSD_VTK_RELEASE_LIBS)
    get_filename_component(release_name "${release_lib}" NAME)
    string(REPLACE "-8.2.lib" "-8.2D.lib" debug_name "${release_name}")
    set(debug_lib "${FOSD_VTK_DEBUG_ROOT}/lib/${debug_name}")
    if(EXISTS "${debug_lib}")
        list(APPEND FOSD_VTK_LIBRARIES
            optimized "${release_lib}"
            debug "${debug_lib}"
        )
# 注意：此分支若找不到对应 Debug 库，会退回使用 Release 库；混用配置需要谨慎检查。
    else()
        list(APPEND FOSD_VTK_LIBRARIES "${release_lib}")
    endif()
endforeach()

# 显式列出应用的 .cpp、.h、.ui 文件。
# .cpp 参与编译；头文件供 IDE 与 AUTOMOC 识别；.ui 供 AUTOUIC 生成界面头文件。
# 增删源码时需要同步更新此清单。
set(FOSD_SOURCES
    lib/CustomUi/switchcontrol.cpp
    lib/CustomUi/switchcontrol.h
    src/app/conncect/connectdebug.cpp
    src/app/conncect/connectdebug.h
    src/app/conncect/connectdebug.ui
    src/app/conncect/connectwindow.cpp
    src/app/conncect/connectwindow.h
    src/app/conncect/connectwindow.ui
    src/app/disp2d/disp2dpara.cpp
    src/app/disp2d/disp2dpara.h
    src/app/disp2d/disp2dpara.ui
    src/app/disp2d/disp2dwindow.cpp
    src/app/disp2d/disp2dwindow.h
    src/app/disp2d/disp2dwindow.ui
    src/app/disp3d/disp3dcolorpicker.cpp
    src/app/disp3d/disp3dcolorpicker.h
    src/app/disp3d/disp3dcolorpicker.ui
    src/app/disp3d/disp3dkeypointedit.cpp
    src/app/disp3d/disp3dkeypointedit.h
    src/app/disp3d/disp3dkeypointedit.ui
    src/app/disp3d/disp3dkeypointselect.cpp
    src/app/disp3d/disp3dkeypointselect.h
    src/app/disp3d/disp3dkeypointselect.ui
    src/app/disp3d/docks/disp3ddockmanager.cpp
    src/app/disp3d/docks/disp3ddockmanager.h
    src/app/disp3d/docks/disp3ddockpanel.h
    src/app/disp3d/docks/disp3dinfodockpanel.cpp
    src/app/disp3d/docks/disp3dinfodockpanel.h
    src/app/disp3d/docks/disp3dinfodockpanel.ui
    src/app/disp3d/docks/disp3dmodeldockpanel.cpp
    src/app/disp3d/docks/disp3dmodeldockpanel.h
    src/app/disp3d/docks/disp3dmodeldockpanel.ui
    src/app/disp3d/docks/disp3dviewdockpanel.cpp
    src/app/disp3d/docks/disp3dviewdockpanel.h
    src/app/disp3d/docks/disp3dviewdockpanel.ui
    src/app/disp3d/disp3dwindow.cpp
    src/app/disp3d/disp3dwindow.h
    src/app/disp3d/disp3dwindow.ui
    src/app/disp3d/dispdata.cpp
    src/app/disp3d/dispdata.h
    src/app/disp3d/dispdata.ui
    src/app/disp3d/trackingpointdatatable.cpp
    src/app/disp3d/trackingpointdatatable.h
    src/app/disp3d/trackingpointdatatable.ui
    src/app/disp3d/trackingpointmanager.cpp
    src/app/disp3d/trackingpointmanager.h
    src/app/disp3d/trackingpointmanager.ui
    src/app/mainwindow/oldmainwindow.cpp
    src/app/mainwindow/oldmainwindow.h
    src/app/mainwindow/oldmainwindow.ui
    src/app/mainwindow/rainbowmainwindow.cpp
    src/app/mainwindow/rainbowmainwindow.h
    src/app/mainwindow/rainbowmainwindow.ui
    src/app/user/login.cpp
    src/app/user/login.h
    src/app/user/login.ui
    src/Communication/serialtask.cpp
    src/Communication/serialtask.h
    src/Communication/tcptask.cpp
    src/Communication/tcptask.h
    src/Communication/udptask.cpp
    src/Communication/udptask.h
    src/core/IFE/IFE.cpp
    src/core/IFE/IFE.h
    src/core/RainbowIFE/rainbowife.cpp
    src/core/RainbowIFE/rainbowife.h
    src/core/RainbowIFE/rainbowloadcalibration.cpp
    src/core/RainbowIFE/rainbowloadcalibration.h
    src/core/model/disp3dbackground.cpp
    src/core/model/disp3dbackground.h
    src/core/model/disp3dfreshmodelthread.cpp
    src/core/model/disp3dfreshmodelthread.h
    src/core/model/disp3dglobal.h
    src/core/model/disp3dinteractor.cpp
    src/core/model/disp3dinteractor.h
    src/core/model/disp3dmodel.cpp
    src/core/model/disp3dmodel.h
    src/core/model/trackingpoint.cpp
    src/core/model/trackingpoint.h
    src/fosdplugin.cpp
    src/main.cpp
    src/protocol/analyzepackets.cpp
    src/protocol/analyzepackets.h
    src/protocol/pocketsconfig.h
    src/test/testdatainjector.cpp
    src/test/testdatainjector.h
    src/test/testsavepointsdisp.cpp
    src/test/testsavepointsdisp.h
    src/test/teststorge.cpp
    src/test/teststorge.h
    src/utils/QCustomPlot/qcpdocumentobject.cpp
    src/utils/QCustomPlot/qcpdocumentobject.h
    src/utils/QCustomPlot/qcustomplot.cpp
    src/utils/QCustomPlot/qcustomplot.h
    src/utils/Storage/DataStorageThread.cpp
    src/utils/Storage/DataStorageThread.h
    src/utils/Timers/Timer.h
    src/utils/config/AppConfig.h
    src/utils/config/ConfigManager.cpp
    src/utils/config/ConfigManager.h
    src/utils/database/database.cpp
    src/utils/database/database.h
    src/utils/global/global.cpp
    src/utils/global/global.h
)

# 调用 Qt 资源编译器 rcc，将图标等资源打包为生成的 C++ 源码。
qt5_add_resources(FOSD_RESOURCES res/resource.qrc)

# 创建名为 FOSD-Pro 的可执行目标，并加入普通源码与生成的资源源码。
# WIN32 在 Windows 上选择 GUI 子系统，程序启动时不显示命令行窗口。
add_executable(FOSD-Pro WIN32
    ${FOSD_SOURCES}
    ${FOSD_RESOURCES}
)

# 对 FOSD-Pro 单独设置输出路径；这些值与前面的全局目录设置重复。
# 目标属性优先决定本目标的实际产物位置。
set_target_properties(FOSD-Pro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib"
    PDB_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/pdb"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/bin/debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/lib/debug"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/lib/debug"
    PDB_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/bin/release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/lib/release"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/lib/release"
    PDB_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/bin/relwithdebinfo"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/lib/relwithdebinfo"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/lib/relwithdebinfo"
    PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/relwithdebinfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/bin/minsizerel"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/lib/minsizerel"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/lib/minsizerel"
    PDB_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/minsizerel"
)

# 为此目标添加头文件搜索目录。PRIVATE 表示这些目录只供 FOSD-Pro 自身使用。
# 此处 VTK 头文件固定取 Release 目录；Debug 也使用这套头文件。
target_include_directories(FOSD-Pro PRIVATE
    src
    lib/CustomUi
    src/app/conncect
    src/app/disp2d
    src/app/disp3d
    src/app/disp3d/docks
    src/app/mainwindow
    src/app/user
    src/Communication
    src/core/IFE
    src/core/RainbowIFE
    src/core/model
    src/main
    src/protocol
    src/test
    src/utils/QCustomPlot
    src/utils/Storage
    src/utils/Timers
    src/utils/config
    src/utils/database
    src/utils/global
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2"
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2/vtkhdf5"
)

# 定义编译宏 QT_DEPRECATED_WARNINGS，使使用过时 Qt API 时产生警告。
target_compile_definitions(FOSD-Pro PRIVATE
    QT_DEPRECATED_WARNINGS
)

# 根据编译器选择选项：MSVC 使用 /utf-8；其他编译器使用 -Wall。
if(MSVC)
    target_compile_options(FOSD-Pro PRIVATE /utf-8)
# 非 MSVC 编译器启用常见警告。
else()
    target_compile_options(FOSD-Pro PRIVATE -Wall)
endif()

# 链接 Qt 导入目标和前面收集的 VTK 库。
# PRIVATE 表示这些链接要求不向依赖本目标的其他目标传播。
target_link_libraries(FOSD-Pro PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::OpenGL
    Qt5::PrintSupport
    Qt5::SerialPort
    Qt5::Widgets
    ${FOSD_VTK_LIBRARIES}
)

# 每次成功构建 FOSD-Pro 后，将 data 目录复制到 exe 所在目录。
# $<TARGET_FILE_DIR:FOSD-Pro> 是生成器表达式，在构建时解析为目标文件目录。
add_custom_command(TARGET FOSD-Pro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/data"
            "$<TARGET_FILE_DIR:FOSD-Pro>/data"
)

# 定义 cmake --install 的安装规则；这一步不会随普通构建自动执行。
# RUNTIME 安装可执行文件；此规则本身不部署 Qt/VTK 的运行时 DLL。
install(TARGETS FOSD-Pro
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
```

## 本项目如何配置和构建

以下命令适用于本机已验证的 CMake 3.17.1、Visual Studio 2017 x64 和 Qt 5.12.2。`Qt5_DIR` 请按实际 Qt 安装位置调整。

```powershell
cmake -S . -B build/cmake-study -G "Visual Studio 15 2017 Win64" -DQt5_DIR=D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/lib/cmake/Qt5
cmake --build build/cmake-study --config Release --parallel 4
cmake --build build/cmake-study --config Debug --parallel 4
```

`-S` 指定源码目录，`-B` 指定生成的构建文件目录，`-G` 选择生成器，`-D` 设置缓存变量。当前文件固定了输出位置，所以两个配置的 exe 分别位于 `build/CMake/bin/release` 和 `build/CMake/bin/debug`。

仓库还有 `CMakePresets.json`，其格式版本为 3。本机 CMake 3.17.1 不支持 Preset 命令，因此使用上面的显式命令。项目声明的最低 CMake 3.16 与 Preset 的可用版本并不一致。

## 核心方法速查

| 命令 | 在本项目中的作用 |
| --- | --- |
| `project()` | 声明项目、版本和语言 |
| `set()` | 设置变量或构建默认值 |
| `find_package()` | 查找 Qt 模块 |
| `add_executable()` | 创建可执行目标 |
| `target_include_directories()` | 设置目标的头文件路径 |
| `target_compile_definitions()` / `target_compile_options()` | 设置宏和编译器选项 |
| `target_link_libraries()` | 设置目标依赖与链接库 |
| `add_custom_command()` | 构建成功后复制数据 |
| `install()` | 定义单独执行的安装步骤 |

学习时重点区分**配置阶段**（查找依赖、生成构建系统）、**构建阶段**（编译与链接）和**安装阶段**（执行安装规则）。
