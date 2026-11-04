# INI 配置模块

`src/config` 提供 INI 参数读写能力，命名空间为 `fosd::config`。通用目标 `fosd_config` 仅依赖 Qt Core；新增的类型化参数层 `fosd_parameters` 已接到各业务 Control 的核心参数入口，默认模板、运行文件及校验规则见 [核心参数配置](core-parameters.md)。

## 格式与分区

一个 INI 文件通过命名分组划分模块：

```ini
[communication]
host=127.0.0.1
port=9000
enabled=true

[storage]
format=csv

[visualization]
deformationScale=1.0
```

采用 UTF-8 编码，底层使用 QSettings::IniFormat。模块名和参数名使用固定拼写，拒绝仅大小写不同的重复名称，避免平台规则造成参数覆盖；名称不能为空、不能带首尾空白、斜杠、反斜杠、换行或 NUL。点号是普通字符，例如 `tcp.host` 为一个完整参数名，不展开为嵌套对象。

支持 QString、QStringList、bool、int、uint、qint64、quint64 和有限 double。无效 QVariant、对象字典和通用 QVariantList 不可作为单项参数写入。INI 通常以文本保存标量；读取结果使用 QVariant，由调用方按约定调用 toInt、toDouble、toBool、toStringList 等方法，并检查转换结果，不依赖读回的 QVariant::type() 与写入前一致。

Qt 将空字符串列表保存为 `@Invalid()`，本模块读取时还原为一个空 QStringList。以 @ 开头的普通字符串由 QSettings 自动转义。引号、列表分隔符及特殊字符也采用 Qt 标准 INI 转义规则。`[General]` 和未分组的根参数统一归入名为 `General` 的模块。

空文件是合法的空配置。空模块不会持久化；writeModule(module, {}) 等价于清空并移除该分组，删除模块最后一个参数也会移除该分组。手工写入的空分组在读取时不会出现在结果中。重新保存时由 QSettings 规范化格式，不保留注释或原有字段顺序。

可以共用一个实例管理不同分组，也可以分别创建不同路径的 IniConfigStore，将配置分开存放到多个 INI 文件。文件路径由应用层明确传入，不使用单例或自动选择业务目录。

## 统一接口

`IConfigStore.h` 暴露统一协议，`IniConfigStore.h/.cpp` 实现文件访问：

```cpp
using ConfigSection = QVariantMap;
using ConfigData = QMap<QString, ConfigSection>;
```

| 接口 | 行为 |
| --- | --- |
| readAll(ConfigData*, error) | 读取整份文件 |
| writeAll(ConfigData, error) | 显式替换整份文件 |
| readModule(module, ConfigSection*, error) | 读取指定分组 |
| writeModule(module, ConfigSection, error) | 完整替换指定分组，保留其他分组 |
| readValue(module, key, QVariant*, error) | 读取指定参数 |
| writeValue(module, key, QVariant, error) | 新增或修改一个参数，保留其他参数和分组 |
| removeModule(module, error) | 删除指定分组 |
| removeValue(module, key, error) | 删除指定参数 |
| IniConfigStore::filePath() | 获取构造时确定的绝对文件路径 |

所有操作同步执行，返回 bool，可选 QString* error 接收详细错误。成功时清空 error，失败时不改变读取输出；文件、分组、参数缺失时返回 false，不伪装成默认值。读取到的值是副本，修改副本不会自动写盘。

writeAll、writeModule 和 writeValue 可创建文件和父目录。局部修改遇到 QSettings 报告的格式错误、无法读取的文件或不支持的名称/类型时失败并保留原文件；只有明确调用 writeAll 才整体替换，包括修复损坏文件。语法容忍度以 Qt INI 解析器为准，模块不提供业务字段、数值范围和设备参数校验。

## 使用示例

```cpp
#include "config/IniConfigStore.h"

fosd::config::IniConfigStore iniStore("config/application.ini");
fosd::config::IConfigStore &config = iniStore;
QString error;

if (!config.writeValue("communication", "port", 9000, &error)) {
    // 由调用方处理 error。
}

QVariant value;
if (config.readValue("communication", "port", &value, &error)) {
    bool ok = false;
    const int port = value.toInt(&ok);
    // 后续由应用层检查 ok 和业务范围。
}

fosd::config::ConfigSection storage{{"format", "csv"}, {"enabled", false}};
config.writeModule("storage", storage, &error);
```

整模块“读取、外部修改、再写回”是两个操作，可能覆盖期间其他调用者对该模块的更改；并发修改不同参数时使用 writeValue。模块不直接显示错误窗口、不创建工作线程，也不将配置数据直接绑定到已有业务 Control。

## 并发与文件保护

同一实例通过 QReadWriteLock 保护读写；多个实例或进程通过 `<文件路径>.lock` 的 QLockFile 串行化“读取—修改—提交”。默认锁等待 1000 ms，可用构造函数第二个参数设置非负超时，0 表示立即尝试。所有写入方需使用相同的绝对路径和锁定协议。

每次读取都对实际文件创建私有快照，再交给 QSettings 解析，避免 QSettings 跨实例文件缓存导致旧配置继续可见。写入先在临时文件中编码并检查 QSettings 状态，再通过 QSaveFile 原子提交到目标文件；禁止直接覆盖回退。QSettings 的临时文件锁与目标文件的外层锁相互独立，避免锁冲突，也避免写入失败后 QSettings 析构重试修改真实文件。

文件读写、临时文件和锁定失败均返回错误；不会抛出用于报告正常读写失败的异常。接口是同步磁盘操作，后续高频写入可由应用层安排到合适的线程。

## 构建和测试

通用消费者链接 fosd_config，并包含 config/IConfigStore.h 或 config/IniConfigStore.h；业务核心参数统一使用 fosd_parameters 和 config/ParameterStore.h。IniConfigStore 另提供 ensureModule / ensureDefaults，在文件锁内只补齐缺失默认键，保留已有用户值。

```powershell
cmake --preset windows-msvc2017
cmake --build --preset windows-debug --target test_config
ctest --preset windows-debug -R '^test_config$'
```

测试使用临时目录，覆盖分组及文件隔离、Qt INI 兼容、中文值、64 位整数、空/单元素/多元素字符串列表、特殊字符串、General 分组、手工编辑后重读、模块替换和删除、异常文件、无效类型、锁冲突、不可写路径，以及同实例/不同实例并发更新不同参数。
