# QWidgets 三合一通信助手

QWidgets 版与 QML 版使用同一个 `CommunicationPresenter`。View 收集输入并调用 Presenter 的公开方法；Presenter 的通知信号驱动控件状态、收发记录、计数和错误显示。通信仍由 `ConnectControl` 的工作线程处理，并通过空拆包器工厂关闭所有协议拆包。

## 结构与生命周期

```text
apps/CommunicationAssistantWidgets/main.cpp
  QApplication → CommunicationPresenter → CommunicationWindow

ui/CommunicationAssistantWidgets/CommunicationWindow.h/.cpp
  QMainWindow View → CommunicationPresenter → ConnectControl → Serialport / UDP / Tcp
                  ← 状态、原始字节和错误通知 ←
```

窗口借用 Presenter 引用，Presenter 必须比窗口存活更久。入口先创建 Presenter，再创建窗口，退出时按相反顺序销毁；通信对象负责关闭通道并等待线程退出。View 不直接持有传输对象，也不承担参数校验、编码、协议解析或错误恢复逻辑。事件处理使用具名槽函数，没有新增 Lambda 表达式。

源码位于 `ui/CommunicationAssistantWidgets`，独立程序入口位于 `apps/CommunicationAssistantWidgets`。公共 Presenter 保留在 `ui/communication-assistant-QML`；该目录的 Presenter 构建仅依赖通信模块，与 QML 资源、引擎和 Qt Quick 无关。

## 功能与 QML 版一致

支持串口扫描及手动输入串口、自定义波特率、数据位、校验、停止位和流控；UDP 本地绑定与可修改的目标地址；TCP 客户端与单客户端服务端。切换通道保留其他连接，关闭仅作用于当前通道。连接期间禁用当前通道配置，发送按钮随通信状态启用或禁用。

支持 UTF-8 文本/HEX 发送、追加 CRLF、收发记录 HEX/文本切换、自动滚动、清空记录及计数清零。日志与字节计数汇总所有通道，接收区域只读且允许复制。HEX 校验、内嵌 NUL 显示、日志容量限制、TCP/串口原始读取块语义均继承 Presenter。`TX请求` 与发送计数表示提交请求，不代表对端收妥。详细数据约定见 [原通信助手说明](communication-assistant-QML.md)。

## 独立构建与运行

需要 MSVC 2017 x64、CMake ≥ 3.20、Qt 5.12 Core/Widgets/Network/SerialPort，测试另需 Test。Widgets 独立配置不查找 Qml/Quick/QuickControls2，不构建 QML 助手、主程序、core/protocol 或业务测试。

```powershell
cmake --preset communication-assistant-widgets
cmake --build --preset assistant-widgets-debug
ctest --preset assistant-widgets-debug
cmake --build --preset assistant-widgets-release
```

产物位于 `build/communication-assistant-widgets/bin/Debug` 或 `Release`，程序名为 `CommunicationAssistantWidgets.exe`。构建缓存与原 QML 版和主程序分开。

| 选项 | 默认值 | 作用 |
| --- | --- | --- |
| `FOSD_BUILD_WIDGETS_ASSISTANT` | OFF | 构建 Widgets 窗口及程序 |
| `FOSD_WIDGETS_ASSISTANT_BUILD_TESTS` | OFF | 构建共享 Presenter 和 Widgets 界面测试，需要启用 Widgets 助手 |

独立预设启用这两项，并明确关闭原有主程序、业务测试、QML 助手及其测试选项。也可以同时启用两个助手；Presenter 库只创建一次。现有构建选项默认值保持原样。

在开发机运行前把 Qt kit 的 `bin` 加入 PATH；分发 Release 版本时部署依赖：

```powershell
& 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64/bin/windeployqt.exe' `
    --release --no-compiler-runtime --no-translations `
    ./build/communication-assistant-widgets/bin/Release/CommunicationAssistantWidgets.exe
```

复制整个 Release 目录，目标机器需安装 MSVC x64 运行库。Widgets 版无需传入 `--qmldir`。Qt kit 不在预设路径时，配置时覆盖 `FOSD_QT_ROOT`，并调整测试预设的 PATH。

## 测试

独立预设运行 `test_assistant_presenter` 与 `test_assistant_widgets`。后者通过真实控件验证 UDP 任意二进制双向收发、HEX 与 CRLF、文本显示内嵌 NUL、跨通道状态保持、日志与计数按钮；通过 TCP 服务端界面验证 UTF-8 发送、原始字节接收、客户端断开后的监听恢复；验证串口输入错误与不存在串口的失败恢复。网络仅使用本机回环地址。

测试在 offscreen 环境运行，并生成 `bin/Debug/assistant-widgets-preview.png` 用于界面检查。串口实体收发需要真实设备或虚拟串口对，自动测试不会打开真实串口。
