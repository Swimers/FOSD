# FOSD-Pro Windows 安装程序

当前安装包版本 **1.0.1**，发布日期 **2026-10-08**，面向 **Windows 10/11 x64**。
离线包包含最新 Release 程序、Qt 5.12.2、VTK 8.2、VC++ 运行库、Qt 插件、ANGLE、D3D 编译器和软件 OpenGL，以及模型、QSS、标定等项目数据。
目标电脑不需要安装 Qt、VTK 或 Visual Studio。Python 测试发送脚本仅在运行该脚本时需要 Python，不是应用启动依赖。

## 输出文件

- `installer/FOSD-Pro-Setup.exe`：可分发的完整离线安装程序。
- `installer/FOSD-Pro-Setup.exe.sha256`：安装程序 SHA256。
- `installer/FOSD-Pro-build-manifest.json`：版本、源码 commit、EXE/安装程序哈希、文件数量。
- `installer/FOSD-Pro-runtime-audit.json`：所有应用 EXE/DLL 的架构、哈希和导入依赖。

这些产物以及 `packages/com.fosd.pro/data/` 暂存文件均被 Git 忽略。

## 构建

在项目根目录执行：

```powershell
.\installer\build_installer.bat
```

批处理调用 `build_installer.ps1`，默认使用本机已验证的路径：

| 项目 | 默认路径 |
| --- | --- |
| Qt | `D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64` |
| VTK DLL | `D:/VTK/VTK-release/bin` |
| IFW | 在 `build/tools/qtifw-4.7` 下自动查找 `binarycreator.exe` |
| Release 构建 | `build/installer-release` |
| 输出目录 | `installer` |
| VC++ CRT | 自动查找 Visual Studio 的 v141 x64 redistributable |

构建电脑需要 CMake、Visual Studio C++ 工具和 v141 编译器、Qt、VTK DLL、Qt Installer Framework。
可覆盖路径，不必编辑脚本：

```powershell
.\installer\build_installer.bat -QtDir "D:/Qt/5.12.2/msvc2017_64" -IfwDir "D:/Qt/Tools/QtInstallerFramework/4.7/bin" -VtkDir "D:/VTK/VTK-release/bin"
```

本次使用 Qt 官方仓库的 IFW 4.7 工具包，下载后按官方 SHA1 校验，解压至项目 `build/tools`，未向系统安装 IFW。

- [IFW 官方工具包目录](https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/tools_ifw/qt.tools.ifw.47/)
- [Qt Windows 部署说明](https://doc.qt.io/archives/qt-5.15/windows-deployment.html)
- [IFW 离线安装包构建说明](https://doc.qt.io/qtinstallerframework/ifw-tools.html)

## 构建流程与依赖检查

1. 配置并编译当前源码的 x64 Release，拒绝使用未成功编译的旧 EXE。
2. 重建固定的包暂存目录，拷贝程序和项目数据，排除采集输出目录及历史 packet log。
3. 使用 `windeployqt` 部署 Qt 模块、平台/图像/打印/样式插件、ANGLE、D3D 和软件 OpenGL。VC++ DLL 来自 Visual Studio 官方可再分发目录。
4. 使用 `dumpbin` 递归扫描全部 EXE/DLL 导入依赖，补齐 VTK 及其间接 DLL；拒绝缺失 DLL、x86 DLL 和 Debug DLL，并核对 EXE 哈希。
5. 使用 `binarycreator --offline-only` 生成安装程序与校验报告。各步骤失败时立即退出，禁止将缺库状态当作成功。

`verify_runtime.ps1` 只允许 Windows 自带系统 DLL/API-set 在包外解析。Qt、VTK、VC++ 依赖必须在包内，不能靠开发电脑 PATH 蒙混通过。
在已初始化 MSVC x64 环境的终端可独立验证：

```powershell
.\installer\verify_runtime.ps1 -PayloadDir .\installer\packages\com.fosd.pro\data
```

## 安装布局

默认安装到当前用户目录 `FOSD-Pro`，无需管理员权限。安装程序创建桌面及开始菜单快捷方式，并提供维护/卸载工具。

```text
FOSD-Pro/
  FOSD-Pro.exe
  Qt5*.dll / vtk*.dll / VC++ DLL
  platforms/qwindows.dll
  imageformats/ / styles/ / printsupport/ / bearer/
  data/                       模型、QSS、标定等
  licenses/                   运行库许可资料
  send_wavelength_packets.py  新旧协议/回环适配器测试脚本
  wavelength-protocol.md      协议及测试说明
  FOSD-Pro Maintenance Tool.exe
```

程序与 `data` 同级，快捷方式的工作目录为安装根目录，匹配项目使用的 `./data/...` 相对路径。
安装器配置版本和组件版本必须一致；图标由构建时从 `res/icon/favicon.ico` 拷贝，避免引用不存在的图标。

## 验证方式

安装器支持 IFW 的命令行安装。测试使用项目 `build` 内的独立安装目录，传入 `SkipShortcuts=true` 避免写入用户桌面和开始菜单：

```powershell
.\installer\FOSD-Pro-Setup.exe --root "D:/Documents/GitHub/FOSD_Project/build/installer-smoke/FOSD Test" --accept-messages --confirm-command install SkipShortcuts=true
```

安装后应核对文件哈希，并在 PATH 仅含 Windows 系统目录、清除 Qt 插件环境变量的情况下启动应用，确认 Qt/VTK/CRT 从安装目录加载。
这类本机隔离验证不能替代其他 Windows 版本和显卡上的实机兼容性测试。

2026-10-08 本机验证通过：完整 Release 编译；73 个 x64 二进制依赖审计
（含 40 个 VTK DLL）；107 个安装文件逐一匹配打包源 SHA256；隔离安装成功；
干净 PATH 下应用正常启动，49 个实际加载的 Qt/VTK/VC++/软件 OpenGL 模块均来自安装目录；
维护工具卸载成功；故意遗漏 Qt5Core.dll 的测试包被依赖审计拒绝。
