# Windows x64 发行版本

最新主程序发行日期：2026-11-07；同时保留 2026-11-05 的历史发行包和模拟器。以下 ZIP 均已包含运行所需的非系统 DLL，完整解压后即可运行，无需安装 Qt、VTK 或 Visual Studio。

| 发行包 | 用途 |
| --- | --- |
| [FOSD-Classic-QWidgets-0.1.0-win64-20261107.zip](FOSD-Classic-QWidgets-0.1.0-win64-20261107.zip) | 最新通信页面：移除诊断、统一 TCP 模式、完整串口参数、保存状态、深浅主题与曲线交互 |
| [FOSD-Classic-QWidgets-0.1.0-win64-20261105.zip](FOSD-Classic-QWidgets-0.1.0-win64-20261105.zip) | 最新 Classic 主程序，包含接收显示切换、串口快捷指令、UDP 摘要与日志清理、中文乱码修复 |
| [MPWavelengthSimulator-win64-20261105.zip](MPWavelengthSimulator-win64-20261105.zip) | MP 波长回环发包及生产解包链路自检工具 |

主程序解压后运行 `FOSD-Classic-QWidgets.exe`。模拟器解压后双击 `验证MP解包.cmd` 自动验证，或双击 `连续发送到9000.cmd` 向 `127.0.0.1:9000` 发包；主程序需先开启对应 UDP 监听。具体设置见包内使用说明。

## 源码与验证

2026-11-07 主程序包基于 `bafb8ab` 及本轮尚未提交的前端/运行时修改构建。Release 17/17 个测试目标通过，验证详情见 [本轮记录](../doc/Classic-Connection-UI-2026-11-07.md)。以下提交对应关系针对 2026-11-05 的历史发行包。

- Classic 功能提交：`27ad579`；含模拟器的完整源码提交：`4b3e31e`。
- 编译环境：MSVC 2017 x64、Qt 5.12.2；主程序使用 VTK 8.2，构建类型为 Release。
- 完整 Release 测试：17/17 通过，包含中文字符独立 UTF-8 校验、快捷指令 CRLF、日志上限与定时清理。
- MP 回环自检：真实接收 113 个数据报，105 个有效包解析匹配，8 个畸形包拒绝。
- 两个 ZIP 均验证了解压文件校验值和脱离开发环境 PATH 的运行；没有进行真实设备联调或在另一台电脑上验收。

二进制由上述已提交源码的同一内容在提交前生成，因此包内旧构建信息中的基准提交和“未提交修改”说明记录的是当时的构建状态；源码对应关系以本文件为准。

## 校验

每个 ZIP 附带同名 `.sha256` 文件，包内另有逐文件 `SHA256SUMS.txt`。可用 PowerShell 校验下载或复制后的发行包：

```powershell
Get-FileHash .\FOSD-Classic-QWidgets-0.1.0-win64-20261105.zip -Algorithm SHA256
Get-FileHash .\MPWavelengthSimulator-win64-20261105.zip -Algorithm SHA256
```

本目录只归档当前可分发版本；调试构建和中间打包结果仍保留在被 Git 忽略的 `build/` 中。迁移个人配置、模型、标定和采集数据时需另行复制。
