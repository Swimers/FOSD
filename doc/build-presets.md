# 独立构建预设

在仓库根目录执行命令；CMake Presets 要求 CMake 3.20 及以上，当前工具链为 Visual Studio 2017 x64、Qt 5.12.2。每个配置使用独立的 CMakeCache 和输出目录；Debug/Release 由多配置生成器在同一目录下分别生成。

| configure 预设 | 构建目录 | 应用目标及范围 |
| --- | --- | --- |
| `windows-msvc2017` | `build/` | 原 FOSD-Win 入口和后端测试；不接入本轮 Classic 应用服务 |
| `fosd-widgets` | `build/fosd-widgets/` | `FOSD-Classic-QWidgets` 完整业务版本、全部后端、QVTK、Plate IFE、发送器及全链路测试；不构建 FOSD-Win |
| `vtk-native-widgets` | `build/vtk-native-widgets/` | `Test-VtkOpenGLNativeWidgets`、全部现有后端、原生 Qt/VTK 界面及全项目测试 |
| `communication-assistant-QML` | `build/communication-assistant-QML/` | 独立 QML 通信助手及其测试 |
| `communication-assistant-widgets` | `build/communication-assistant-widgets/` | 独立 Widgets 通信助手及其测试 |

本轮业务闭环仅在 `apps/FOSD-Classic-QWidgets` 实现：ClassicWorkbench 通过 WorkbenchRuntime 注入通信、协议、解算、存储和可视化服务，挂载真实 QVTK 控件。共享 UI 库仍保持分层；`apps/FOSD-Win` 的源码、入口和构建文件恢复原状。完整使用与验证见 [Classic 业务闭环](Architectural/formal-workbench-integration.md)。

## 配置、构建与测试

```powershell
# 原 FOSD-Win 版本
cmake --preset windows-msvc2017
cmake --build --preset windows-debug
ctest --preset windows-debug

# Classic 完整业务版本与全链路测试
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-release
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
ctest --preset fosd-widgets-release

# 原生 Qt/VTK 验证前端与全部后端测试
cmake --preset vtk-native-widgets
cmake --build --preset vtk-native-widgets-release
ctest --preset vtk-native-widgets-release
```

Classic 和 VTK 验证配置均提供 `-debug`、`-release` build/test 预设；test 预设补充 Qt DLL 路径，VTK DLL 需在外部 PATH 提供。Classic 默认构建全部后端及可视化测试，其中 `test_workbench_main` 创建真实 OpenGL 上下文并验证界面到存储的闭环，需可用桌面；单独的 `test_classic_widgets` 仍使用 offscreen。原生验证配置的图形冒烟检查见 [VTK 前端文档](../ui/Test-VtkOpenGLNativeWidgets/README.md)。

## 启动与配置开关

```powershell
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Release/FOSD-Classic-QWidgets.exe
# 独立 VTK 验证版本：
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe
```

本机的 `D:/VTK/VTK-release/bin` 为 Release 运行库，Debug 启动必须另配 Debug DLL。可通过 `-DFOSD_VTK_NATIVE_RUNTIME_DIR=<目录>` 设置 VTK 目标的 Visual Studio 调试环境。

| 开关 | 默认值 | 作用 |
| --- | --- | --- |
| `FOSD_BUILD_WORKBENCH_APP` | OFF | 构建独立 `FOSD-Classic-QWidgets` 完整业务版本及所需后端 |
| `FOSD_WORKBENCH_BUILD_TESTS` | OFF | 单独开启工作台 UI 测试 |
| `FOSD_BUILD_VTK_NATIVE_TEST_APP` | OFF | 构建独立原生 Qt/VTK 验证程序 |
| `FOSD_VTK_NATIVE_BUILD_TESTS` | OFF | 单独开启 VTK Presenter 测试，要求验证程序开关开启 |

Classic 和原生验证预设均关闭 `FOSD_BUILD_MAIN_APP` 和通信助手，并构建后端及相应测试。Classic 额外开启 `FOSD_BUILD_VISUALIZATION_TESTS`，仅在自身配置中构建 `fosd_classic_runtime` 及两项业务/界面测试；原 FOSD-Win 配置不链接它们。若 Classic 只构建应用而关闭测试，需要同时关闭 `FOSD_BUILD_TESTS`、`FOSD_WORKBENCH_BUILD_TESTS` 和 `FOSD_BUILD_VISUALIZATION_TESTS`。预设使用独立缓存，切换不需要删除其他版本的构建目录。

只执行 `cmake -S . -B build` 会使用 CMakeLists 中的默认值或已有缓存；若要使用这里定义的隔离配置，请执行 `cmake --preset <名称>`。切换预设无需删除其他构建目录。
