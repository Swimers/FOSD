# 开发文档入口

| 文档 | 适用场景 |
| --- | --- |
| [模块构建结构](build-structure.md) | CMake 目标、依赖、构建命令与新增模块 |
| [协议与共享数据对接](protocol-integration.md) | communication → protocol → Datamodel 数据流，storage/visualization 对接，协议扩展与测试 |
| [独立三合一通信助手](communication-assistant.md) | QML/MVP 结构、原始字节收发、独立构建、部署与测试 |
| [波长协议参考解析代码](QT解析代码.txt) | 当前 `0xA55A` 波长数据的字段与解码依据 |
| [参考收包代码](QT解析格式.txt) | 原始 UDP 接收和解析调用示例 |

接口行为以当前源码为准。新增协议或改变共享数据接口时，同步更新对接文档和对应测试。
