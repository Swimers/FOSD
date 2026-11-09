# 独立构建预设

> 2026-11-08：Classic 已迁移为三层 MVP；旧 Backend 请求转发链已删除，当前边界与测试命名见 [MVP 迁移指南](Architectural/classic-mvp-ui-backend-connection.md)。

在仓库根目录执行命令；CMake Presets 要求 CMake 3.20 及以上，当前工具链为 Visual Studio 2017 x64、Qt 5.12.2。每个配置使用独立的 CMakeCache 和输出目录；Debug/Release 由多配置生成器在同一目录下分别生成。

| configure 预设 | 构建目录 | 应用目标及范围 |
| --- | --- | --- |
| `windows-msvc2017` | `build/` | 原 FOSD-Win 入口和后端测试；不接入本轮 Classic 应用服务 |
| `fosd-widgets` | `build/fosd-widgets/` | `FOSD-Classic-QWidgets` 完整业务版本、全部后端、QVTK、Plate IFE、发送器及全链路测试；不构建 FOSD-Win |
| `communication-assistant-QML` | `build/communication-assistant-QML/` | 独立 QML 通信助手及其测试 |
| `communication-assistant-widgets` | `build/communication-assistant-widgets/` | 独立 Widgets 通信助手及其测试 |

Classic 业务闭环实现在 `ui/FOSD-Classic-QWidgets/application`：ClassicWorkbench 通过 WorkbenchModel 注入通信、协议、解算、存储和可视化服务，挂载真实 QVTK 控件；`apps/FOSD-Classic-QWidgets` 只负责启动入口和 EXE 构建。共享 UI 库仍保持分层，FOSD-Win 继续使用共享 Widgets。目录说明见 [Classic 界面](../ui/FOSD-Classic-QWidgets/README.md)，完整使用与验证见 [Classic 业务闭环](Architectural/formal-workbench-integration.md)。

## Classic 的 CMake 目标与编辑器诊断

根 CMakeLists 先加载 `src` 的业务目标，再加载 `ui/FOSD-Classic-QWidgets`，最后创建 `apps/FOSD-Classic-QWidgets` 的 EXE。UI 子目录是根工程的一部分，不是可单独配置的 CMake 工程。

| 目标 | 直接依赖与作用 |
| --- | --- |
| `fosd_classic_contracts` | Qt Core；视图契约和事件 |
| `fosd_classic_controls` | Qt Widgets、PrintSupport；自研控件和 controls/qcustomplot |
| `fosd_classic_widgets` | contracts、controls、Qt Widgets；表单与 View，不依赖 Model/Presenter |
| `fosd_classic_model` | protocol、calculation、visualization；业务协调 |
| `fosd_classic_presenters` | contracts、model、Qt Widgets；交互与界面状态 |
| `fosd_classic_application` | widgets、presenters、model、visualization_qt；工作台与 QVTK 装配 |
| `FOSD-Classic-QWidgets` | application；启动入口及运行库部署 |

业务库通过 PUBLIC include 目录向调用方传递 `src/`、Qt 和 VTK 头文件路径；Classic 的公开 include 根为 `ui/`。因此 `protocol/ProtocolControl.h` 从 `src/` 查找，`FOSD-Classic-QWidgets/model/WorkbenchModel.h` 从 `ui/` 查找，不需要为每个源码手工填写全局 includePath。

VS Code 应使用 `fosd-widgets` configure 预设和匹配的 build/test 预设，并由 `ms-vscode.cmake-tools` 提供 C/C++ 配置。Visual Studio 生成器不生成 compile_commands.json；设置 CMAKE_EXPORT_COMPILE_COMMANDS=ON 不能代替 CMake Tools 的 File API 配置。

2026-11-08 本机诊断记录：

- 根 `build/` 仍有迁移前的 File API 元数据；`build/fosd-widgets/` 才是编辑器当前使用的构建目录。切换预设或移动源码后运行 **CMake: Configure**，必要时运行 **C/C++: Restart IntelliSense for Active File**。`C/C++(1696)` 在首行标红是 include 解析失败的汇总，不能据此认定 `#pragma once` 有语法问题。本次实时诊断中 WorkbenchModel.h 已关联 WorkbenchModel.cpp，头文件路径完整，编辑器问题计数为 0。
- 本机 CMake Tools 1.24.42 的 getTestsForOutline 无保护地读取 `test.command[0]`。未编译的测试可使 CTest JSON 缺少 command；本次 Debug 缺少新命名的 test_workbench_model，Release 缺少三个测试命令，因而触发 testCodeLensProvider 刷屏。构建当前配置的全部目标，再运行 **CMake: Refresh Tests**；只构建应用 EXE 不会补齐测试目标。配置成功和测试 EXE 已生成是两件事。
- 同一个历史构建目录还残留旧版 Qt AUTORCC 包装文件，其中偏置项缺失；普通增量构建曾复用旧资源。若出现“源码模板已有字段，但程序仍报告缺少该字段”，应检查并清理该目标的自动生成产物，再重新构建，而不是修改参数来迁就旧二进制。重建目录或改变 CMake 版本后尤其需要核对实际使用的生成文件。

诊断命令：`ctest --test-dir build/fosd-widgets -C Release --show-only=json-v1`。测试列表中每项都应有 command；Debug 与 Release 分别检查，不要混用其他构建目录中的 EXE。

本次恢复结果：当前 `build/fosd-widgets` 的 Debug/Release 均构建通过，两种配置均发现 18 项有效测试命令；Release 完整回归 18/18 通过，Debug 参数与 Model 回归 2/2 通过。执行 CMake: Configure 后切换源码验证，日志未再新增 CodeLens 异常。仅清理并重新生成构建缓存，不需要修改 CMake 链接关系或业务源码。

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

```

Classic 提供 `-debug`、`-release` build/test 预设；test 预设补充 Qt DLL 路径。默认构建全部后端及可视化测试，其中 `test_workbench_main` 创建真实 OpenGL 上下文并验证界面到存储的闭环，需可用桌面；`test_classic_widgets` 使用 offscreen。独立 VTK 验证前端及其预设已于 2026-11-09 移除，Classic 的 QVTK 宿主与可视化后端仍保留。

2026-11-09 前端移除记录：已删除 qml_Classic、widget_Classic 两个空目录和 Test-VtkOpenGLNativeWidgets 前端，并清理后者的独立 apps 入口、构建开关和五项 configure/build/test 预设。重新配置后四种构建配置均无旧验证程序目标，Classic 与 FOSD-Win Release 构建通过，完整回归 18/18 通过（47.50 秒）。记录见 build/fosd-widgets/retired-frontends-configure.log、retired-frontends-build.log、retired-frontends-ctest.log 和 build/classic-layout-compat/retired-frontends-build.log。

## 启动与配置开关

```powershell
$env:PATH = 'D:/VTK/VTK-release/bin;D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin;' + $env:PATH
& ./build/fosd-widgets/bin/Release/FOSD-Classic-QWidgets.exe
```

本机的 `D:/VTK/VTK-release/bin` 为 Release 运行库，Debug 启动必须另配 Debug DLL。Classic 使用 `FOSD_VISUALIZATION_VTK_RUNTIME_DIR` 指定匹配的 VTK 运行库目录，并设置 Visual Studio 调试环境。

| 开关 | 默认值 | 作用 |
| --- | --- | --- |
| `FOSD_BUILD_WORKBENCH_APP` | OFF | 构建独立 `FOSD-Classic-QWidgets` 完整业务版本及所需后端 |
| `FOSD_WORKBENCH_BUILD_TESTS` | OFF | 单独开启工作台 UI 测试 |

Classic 预设关闭 `FOSD_BUILD_MAIN_APP` 和通信助手，开启完整业务 Model、Presenter、工作台装配及相关测试，并开启 `FOSD_BUILD_VISUALIZATION_TESTS`。原 FOSD-Win 配置仍使用共享 View/Presenter，未装配完整业务 Model。若 Classic 只构建应用而关闭测试，需要同时关闭 `FOSD_BUILD_TESTS`、`FOSD_WORKBENCH_BUILD_TESTS` 和 `FOSD_BUILD_VISUALIZATION_TESTS`。各应用预设使用独立缓存。

只执行 `cmake -S . -B build` 会使用 CMakeLists 中的默认值或已有缓存；若要使用这里定义的隔离配置，请执行 `cmake --preset <名称>`。切换预设无需删除其他构建目录。
