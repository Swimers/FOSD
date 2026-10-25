# 独立构建预设

在仓库根目录执行命令；CMake Presets 要求 CMake 3.20 及以上，当前工具链为 Visual Studio 2017 x64、Qt 5.12.2。每个配置使用独立的 CMakeCache 和输出目录；Debug/Release 由多配置生成器在同一目录下分别生成。

| configure 预设 | 构建目录 | 应用目标及范围 |
| --- | --- | --- |
| `windows-msvc2017` | `build/` | `FOSD` 主程序、业务模块、全项目测试；关闭独立预览、VTK 验证程序及通信助手 |
| `fosd-widgets` | `build/fosd-widgets/` | `FOSD-Classic-QWidgets` UI 预览、工作台库及 UI 测试；只依赖 Qt Core/Widgets/PrintSupport/Test |
| `vtk-native-widgets` | `build/vtk-native-widgets/` | `Test-VtkOpenGLNativeWidgets`、全部现有后端、原生 Qt/VTK 界面及全项目测试 |
| `communication-assistant-QML` | `build/communication-assistant-QML/` | 独立 QML 通信助手及其测试 |
| `communication-assistant-widgets` | `build/communication-assistant-widgets/` | 独立 Widgets 通信助手及其测试 |

`FOSD-Classic-QWidgets` 原先只有 UI 库，并随主程序或全项目测试启用。现在新增 `apps/FOSD-Classic-QWidgets/main.cpp` 作为纯 UI 预览入口，组合 WorkbenchView 与 WorkbenchPresenter，保留现有后端接口。主程序 `FOSD` 继续链接同一工作台库，并由 `apps/FOSD-Win` 组合通信、解算等业务模块。

## 配置、构建与测试

```powershell
# 主程序
cmake --preset windows-msvc2017
cmake --build --preset windows-debug
ctest --preset windows-debug

# 迁移工作台的独立预览与测试
cmake --preset fosd-widgets
cmake --build --preset fosd-widgets-debug
ctest --preset fosd-widgets-debug

# 原生 Qt/VTK 验证前端与全部后端测试
cmake --preset vtk-native-widgets
cmake --build --preset vtk-native-widgets-release
ctest --preset vtk-native-widgets-release
```

两套新配置都提供 `-debug`、`-release` 的 build/test 预设。build 默认构建应用及其测试，test 预设补充 Qt DLL 的 PATH。`fosd-widgets` 的测试使用 offscreen 平台；`vtk-native-widgets` 的默认测试覆盖全部后端及 Presenter，不创建真实 OpenGL 上下文。真实 OpenGL 检查见 [VTK 前端文档](../ui/Test-VtkOpenGLNativeWidgets/README.md)。

## 启动与配置开关

```powershell
$env:PATH = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Debug/FOSD-Classic-QWidgets.exe

# VTK 验证程序还需要与构建配置匹配的 VTK DLL。
$env:PATH = 'D:/VTK/VTK-release/bin;' + $env:PATH
& ./build/vtk-native-widgets/bin/Release/Test-VtkOpenGLNativeWidgets.exe
```

本机的 `D:/VTK/VTK-release/bin` 为 Release 运行库，Debug 启动必须另配 Debug DLL。可通过 `-DFOSD_VTK_NATIVE_RUNTIME_DIR=<目录>` 设置 VTK 目标的 Visual Studio 调试环境。

| 开关 | 默认值 | 作用 |
| --- | --- | --- |
| `FOSD_BUILD_WORKBENCH_APP` | OFF | 构建独立 `FOSD-Classic-QWidgets` 预览程序 |
| `FOSD_WORKBENCH_BUILD_TESTS` | OFF | 单独开启工作台 UI 测试 |
| `FOSD_BUILD_VTK_NATIVE_TEST_APP` | OFF | 构建独立原生 Qt/VTK 验证程序 |
| `FOSD_VTK_NATIVE_BUILD_TESTS` | OFF | 单独开启 VTK Presenter 测试，要求验证程序开关开启 |

两个新预设均关闭 `FOSD_BUILD_MAIN_APP` 和通信助手。工作台预览关闭全项目测试，仅开启其 UI 测试；VTK 集成验证开启 `FOSD_BUILD_TESTS`，构建全部后端并运行全项目测试。全项目测试仍包含工作台 UI 测试；手动同时开启 VTK 验证程序时，全项目测试也包含其 Presenter 测试。预设显式声明应用开关，重新配置已有目录时会覆盖此前手动缓存的应用开关。

只执行 `cmake -S . -B build` 会使用 CMakeLists 中的默认值或已有缓存；若要使用这里定义的隔离配置，请执行 `cmake --preset <名称>`。切换预设无需删除其他构建目录。
