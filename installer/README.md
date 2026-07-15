# FOSD-Pro 安装程序构建指南

## 目录结构

```
installer/
├── config/
│   └── config.xml                  # 安装程序全局配置
├── packages/
│   └── com.fosd.pro/
│       ├── meta/
│       │   ├── package.xml         # 包元数据
│       │   └── installscript.qs    # 安装脚本（快捷方式等）
│       └── data/                   # [构建时生成] 安装文件
├── build_installer.bat             # Windows 构建脚本
├── .gitignore                      # 忽略构建产物
└── README.md                       # 本文件
```

## 前置条件

1. **Qt Installer Framework** 已安装
   - 通过 Qt Maintenance Tool 安装：`Tools → Qt Installer Framework`
   - 或从 https://download.qt.io/official_releases/qt-installer-framework/ 下载

2. **FOSD-Pro Release 编译完成**
   - 确保 `build/QMake/windows/bin/release/FOSD-Pro.exe` 存在

3. **Qt 环境** 已安装（需要 `windeployqt` 工具）

## 快速开始

### 1. 修改构建脚本配置

编辑 `build_installer.bat` 顶部的配置变量：

```batch
:: 修改为你的 Qt 安装路径
set QT_DIR=C:\Qt\5.15.2\msvc2019_64

:: 修改为你的 IFW 安装路径
set IFW_DIR=C:\Qt\Tools\QtInstallerFramework\4.x\bin

:: VTK DLL 路径（如果 VTK 是动态链接的）
set VTK_DLL_DIR=%PROJECT_DIR%\lib\VTK\VTK-release\bin
```

**如何找到你的 Qt 路径？**
- 打开 Qt Creator → 工具 → 选项 → Kits → 查看 Qt 版本路径
- 或在 Qt Command Prompt 中执行：`qmake -v`

**如何找到 IFW 路径？**
- 通常在 Qt 安装目录下的 `Tools/QtInstallerFramework/4.x/bin/`
- 如果通过 Maintenance Tool 安装，默认在 Qt 根目录

### 2. 运行构建

双击 `build_installer.bat`，或在命令行中执行：

```cmd
cd C:\Users\Swimers\Documents\GitHub\FOSD_Project\installer
build_installer.bat
```

### 3. 获取安装程序

构建成功后，安装程序位于：

```
installer_output/FOSD-Pro-Setup.exe
```

## 构建流程说明

```
build_installer.bat 执行流程：

[1/4] windeployqt 收集 Qt 依赖
      → 自动拷贝 Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll 等
      → 自动拷贝 platforms/qwindows.dll 等插件

[2/4] 拷贝所有文件到打包目录
      → FOSD-Pro.exe + Qt DLL + VTK DLL + 数据文件

[3/4] 统计打包内容

[4/4] binarycreator 生成安装程序
      → 读取 config/config.xml（全局配置）
      → 读取 packages/com.fosd.pro/meta/package.xml（包信息）
      → 执行 installscript.qs（创建快捷方式）
      → 输出 FOSD-Pro-Setup.exe
```

## 安装程序功能

安装完成后，安装程序会自动：

- ✅ 创建桌面快捷方式 `FOSD-Pro`
- ✅ 创建开始菜单快捷方式 `FOSD-Pro`
- ✅ 创建开始菜单卸载快捷方式
- ✅ 创建数据目录快捷方式
- ✅ 自带卸载工具 `FOSD-Pro Maintenance Tool.exe`

## 自定义修改

### 修改安装程序名称/版本

编辑 `config/config.xml`：

```xml
<Name>FOSD-Pro</Name>           <!-- 程序名称 -->
<Version>1.0.0</Version>        <!-- 版本号 -->
<Title>FOSD-Pro 安装程序</Title>  <!-- 安装窗口标题 -->
<Publisher>FOSD Team</Publisher> <!-- 发布者 -->
```

### 修改默认安装路径

编辑 `config/config.xml` 中的 `TargetDir`：

```xml
<!-- 安装到用户目录 -->
<TargetDir>@HomeDir@/FOSD-Pro</TargetDir>

<!-- 安装到 Program Files -->
<TargetDir>@ApplicationsDir@/FOSD-Pro</TargetDir>

<!-- 安装到自定义路径 -->
<TargetDir>C:/FOSD-Pro</TargetDir>
```

### 添加许可协议

1. 将 `license.txt` 放到 `packages/com.fosd.pro/meta/` 目录
2. 编辑 `package.xml`，取消注释许可协议部分：

```xml
<Licenses>
    <License name="许可协议" file="license.txt" />
</Licenses>
```

### 修改快捷方式

编辑 `installscript.qs` 中的 `CreateOperations` 部分。

## 常见问题

### Q: 运行构建脚本提示"未找到 windeployqt"
A: 检查 `QT_DIR` 是否正确指向 Qt 安装目录（包含 `bin/windeployqt.exe`）

### Q: 安装后运行 FOSD-Pro.exe 报错"缺少 DLL"
A: 可能缺少 VTK DLL 或 VC++ 运行时。检查：
1. VTK DLL 是否已拷贝到打包目录
2. 目标机器是否安装了 VC++ 2015-2022 Redistributable

### Q: 如何添加更多组件（如示例数据、文档）
A: 在 `packages/` 下新建子包目录，如 `com.fosd.examples/`，包含 `meta/package.xml` 和 `data/`

### Q: 如何生成在线安装程序
A: 修改 `build_installer.bat` 中的 `binarycreator` 命令，去掉 `--offline-only` 参数，
   并使用 `repogen` 生成仓库，`archivegen` 打包数据
