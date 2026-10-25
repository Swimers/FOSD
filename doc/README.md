# 开发文档入口

| 文档 | 适用场景 |
| --- | --- |
| [可视化迁移与标量映射](Architectural/visualization-integration.md) | 分步迁移安排、X/Y/Z/总位移着色、归一化与物理值、通用点标量入口及测试 |
| [独立构建预设](build-presets.md) | 主程序、FOSD-Classic-QWidgets 预览、原生 Qt/VTK 验证及通信助手的配置、构建、测试与运行 |
| [原生 Qt/VTK 验证前端](../ui/Test-VtkOpenGLNativeWidgets/README.md) | Designer UI、Presenter、全部后端连接、窗口指针传递及收包到变形显示验证 |
| [主工作台 Widgets 与 MVP](../ui/FOSD-Classic-QWidgets/README.md) | 旧版 19 个表单、资源、View/Presenter、后端请求及三维宿主对接 |
| [解算与模型对接](Architectural/calculation-integration.md) | 波长快照、标定、模型输入、逐点插值与只读结果输出 |
| [数据存储对接](Architectural/storage-integration.md) | 解算线程托管、CSV/SQLite、数据类型及点位选择、错误处理、扩展与测试 |
| [模块构建结构](build-structure.md) | CMake 目标、依赖、构建命令与新增模块 |
| [协议与共享数据对接](protocol-integration.md) | communication → protocol → Datamodel 数据流，storage/visualization 对接，协议扩展与测试 |
| [QML 三合一通信助手](communication-assistant-QML.md) | QML/MVP 结构、原始字节收发、独立构建、部署与测试 |
| [QWidgets 通信助手](communication-assistant-widgets.md) | 复用 Presenter 的 Widgets 前端、独立构建和界面测试 |
| [波长协议参考解析代码](QT解析代码.txt) | 当前 `0xA55A` 波长数据的字段与解码依据 |
| [参考收包代码](QT解析格式.txt) | 原始 UDP 接收和解析调用示例 |

接口行为以当前源码为准。新增协议或改变共享数据接口时，同步更新对接文档和对应测试。
