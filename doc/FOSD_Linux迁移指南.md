# FOSD 上位机 Windows → Linux 迁移指南

> 适用项目：FOSD_Project（Qt 5.12 + VTK 8.2 + QCustomPlot，C++11，qmake/CMake 双构建）
> 撰写日期：2026-09-21。文中的路径、文件名、命令基于对当前仓库的实际探查（已核对 .pro / CMakeLists.txt / 源码编码 / lib 目录），未在 Linux 真机上执行过的命令均已标注"待执行验证"。

## 0. 迁移前评估（已查证的项目现状）

### 有利条件（底子比想象好）

| 项 | 现状 | 说明 |
|---|---|---|
| qmake 工程 | `FOSD-Pro.pro` 已内置完整 `unix:!macx` 分支 | Linux 下 VTK 用 `-lvtkXXX-8.2` 链接，已含 Wayland/XKB 依赖、`/opt/FOSD-Pro/bin` 安装路径、BUILD_ROOT/DESTDIR 配置 |
| Linux 版 VTK 库 | `lib/VTK/VTK-Linux-debug/` 已有 125 个 `.so` + 完整头文件（2765 个 .h） | 不需要现场编译 VTK 即可起步（注意是 **debug** 版，release 需另行编译） |
| 源码编码 | 91 个源文件全部 UTF-8（0 个 GBK），仅 2 个带 BOM | 中文注释跨平台不会乱码，只需处理 BOM |
| 通信层 | `QSerialPort / QUdpSocket / QTcpServer` 全部跨平台 | 串口/TCP/UDP 代码基本零改动 |
| 渲染/绘图 | VTK Qt 渲染（QVTKOpenGL）、QCustomPlot 跨平台 | 三维/二维显示层可移植 |
| 历史成果 | 远端已有 `origin/FOSD-Linux`、`origin/FOSD-Rainbow-Linux` 分支 | 先拉取对比，可能有已完成的一半工作 |

### 需要处理的事项（工作量清单）

| # | 事项 | 位置 | 动作 |
|---|---|---|---|
| 1 | CMake 构建纯 Windows | `CMakeLists.txt`（硬编码 `D:/Qt/5.12.2/msvc2017_64`、`*.lib`、`WIN32`） | Linux 下**改用 qmake**（.pro 已就绪），CMake 暂不动 |
| 2 | .pro 的 Linux 分支缺变量定义 | `FOSD-Pro.pro` 的 `unix:!macx` 分支使用 `$$VTK_INCLUDE_DIR` / `$$VTK_LIB_DIR`，但全文未定义 | ① qmake 命令行传参，或 ② 在 .pro 里补 Linux 默认定义 |
| 3 | UTF-8 BOM | `src/core/IFE/IFE.cpp`、`src/core/IFE/IFE.h` | 去掉 BOM（GCC 一般能容忍，但建议清理统一） |
| 4 | 硬编码 Windows 路径 | `src/app/mainwindow/rainbowmainwindow.cpp:33`（`C:/Users/User/Desktop/蒲城/...txt`）；`src/test/testsavepointsdisp.cpp:5-6`（`D:/disp_point_*.txt`） | 改为相对路径 / QStandardPaths / 配置文件读取 |
| 5 | 宿主 demo 用 WinAPI | `PluginHost/main.cpp`、`PluginHost/mainwindow.cpp`（`#include <windows.h>` + LoadLibrary） | 主程序 FOSD-Pro 不依赖它，Linux 下跳过；如需编译改为 `QLibrary` |
| 6 | 串口设备与权限 | 串口配置在 UI，代码无硬编码 COM 口 | Linux 下设备名为 `/dev/ttyUSB0` 等；用户需加入 `dialout` 组 |
| 7 | 大小写敏感 | Linux 文件系统区分大小写 | 编译报错时检查 `#include` 与文件名大小写一致（Qt 的 qmake 会直接暴露） |

---

## 1. 阶段 0：准备与评估（约半天）

1. **先拉取远端 Linux 分支**，避免重复劳动：
   ```bash
   git fetch origin
   git log --oneline origin/FOSD-Linux        # 看已有哪些迁移提交
   git diff FOSD-2026HYY origin/FOSD-Linux -- FOSD-Pro.pro src/   # 对比差异
   ```
2. 确认目标发行版（推荐 **Ubuntu 20.04 / 22.04**，仓库里的 VTK-Linux-debug 大概率在 Ubuntu 上构建）与 Qt 版本（系统 Qt 5.12.8 与项目 5.12.2 兼容，也可装官方 5.12.2）。
3. 备份：迁移前确保 git 工作区干净（`git status`），Linux 上从新克隆或拷贝一份干净源码。

## 2. 阶段 1：Linux 环境与依赖（约 1 天）

Ubuntu 下安装编译工具、Qt 模块与 OpenGL 依赖（**待执行验证**，包名以目标发行版为准）：

```bash
sudo apt update
sudo apt install -y build-essential git cmake
# Qt 5（Ubuntu 20.04 自带 5.12.8；用系统包即可，无需官网安装包）
sudo apt install -y qtbase5-dev qtchooser libqt5serialport5-dev \
    libqt5opengl5-dev libqt5printsupport5-dev qt5-qmake
# OpenGL / 窗口系统（.pro 的 unix 分支引用了 wayland/xkbcommon）
sudo apt install -y libgl1-mesa-dev libglew-dev \
    libwayland-dev libxkbcommon-dev mesa-utils
```

校验：
```bash
qmake --version        # 期望 Qt 5.12.x
```

## 3. 阶段 2：qmake 构建适配（约半天）

### 3.1 传 VTK 变量（方式 A，不改 .pro）

```bash
cd FOSD_Project
qmake FOSD-Pro.pro \
    VTK_INCLUDE_DIR="$PWD/lib/VTK/VTK-Linux-debug/include/vtk-8.2" \
    VTK_LIB_DIR="$PWD/lib/VTK/VTK-Linux-debug/lib" \
    CONFIG+=debug
make -j$(nproc)
```

### 3.2 或在 .pro 补 Linux 默认定义（方式 B，推荐长期维护）

在 `FOSD-Pro.pro` 的 `unix:!macx {` 分支内、`INCLUDEPATH += $$VTK_INCLUDE_DIR` 之前补：

```qmake
unix:!macx {
    isEmpty(VTK_INCLUDE_DIR): VTK_INCLUDE_DIR = $$clean_path($$PWD/lib/VTK/VTK-Linux-debug/include/vtk-8.2)
    isEmpty(VTK_LIB_DIR):     VTK_LIB_DIR     = $$clean_path($$PWD/lib/VTK/VTK-Linux-debug/lib)
    INCLUDEPATH += $$VTK_INCLUDE_DIR
    ...
}
```

> 注意：`VTK_DIR`（Windows 用）与 `VTK_INCLUDE_DIR/VTK_LIB_DIR`（Linux 用）是两套变量，不要混用。
> Linux 仓库里目前只有 **debug** 版 VTK；如果后续需要 release 性能，再单独编译 VTK 8.2（见第 6 节）。

## 4. 阶段 3：代码适配清单（约 1~2 天，改动很小）

```bash
# ① 去掉两个源文件的 UTF-8 BOM
sed -i '1s/^\xEF\xBB\xBF//' src/core/IFE/IFE.cpp src/core/IFE/IFE.h
# ② 检查是否还有残留：file src/core/IFE/IFE.cpp  应显示 "UTF-8 Unicode text"
```

- **硬编码路径**：
  - `src/app/mainwindow/rainbowmainwindow.cpp:33` 的 `C:/Users/User/Desktop/蒲城/数据处理/2026-04-18data.txt` → 改为 `QFileDialog::getOpenFileName` 或读取 ConfigManager 配置（项目已有 JSON 配置体系），至少改为 `QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)`。
  - `src/test/testsavepointsdisp.cpp` 的 `D:/disp_point_*.txt` → 改为测试工作目录下的相对路径。
- **PluginHost**：主程序不依赖，Linux 下直接从工程中排除（.pro 不包含）即可；若要编译，把 `LoadLibrary/GetProcAddress` 换成 Qt 的 `QLibrary`（自动处理 `.so`）。
- **串口权限**（接硬件时）：
  ```bash
  sudo usermod -aG dialout $USER   # 重新登录生效
  ls /dev/ttyUSB*                  # 确认解调仪枚举出的设备名
  ```
- **其余跨平台检查**：qmake 编译报错优先看 `#include` 大小写；`res/resource.qrc` 里的资源路径用 `/` 分隔（Qt 规范，一般无问题）。

## 5. 阶段 4：验证方案（约 1 天）

按"先无硬件、后有硬件、再比一致性"的顺序：

1. **无硬件自测**：用项目已有的 `TestDataInjector`（txt 逐帧回放）跑通"通信注入 → 解析 → iFEM 解算 → 2D/3D 显示"全链路，确认算法与渲染在 Linux 下正常。
2. **渲染验证**：加载 `data/` 下的 STL 模型，确认 3D 窗口可旋转/缩放/拾取（若黑屏，检查 OpenGL 驱动：`glxinfo | grep OpenGL`，装 mesa 或 NVIDIA 官方驱动）。
3. **算法一致性**：用同一份输入数据（如蒲城 txt）分别在 Windows 与 Linux 下跑，对比 `node_u` 位移场输出是否一致——这是"迁移没破坏算法"的最硬验证。
4. **硬件通信**（有条件时）：串口连接解调仪，验证收包解析与 Windows 行为一致；TCP/UDP 用本地回环先验证。
5. **打包/安装**（可选）：`.pro` 已配置 `target.path=/opt/FOSD-Pro/bin`，`make install` 可部署；桌面运行依赖可用 `ldd` 检查 `.so`，或做 AppImage。

## 6. 风险与注意事项

| 风险 | 等级 | 应对 |
|---|---|---|
| VTK 只有 debug 版（性能与体积） | 中 | 先用 debug 跑通；需要 release 时编译 VTK 8.2：`cmake -DVTK_QT_VERSION=5 -DVTK_GROUP_ENABLE_Qt=YES -DCMAKE_BUILD_TYPE=Release ..`（依赖 qtbase5-dev） |
| CMakeLists.txt 无法在 Linux 用 | 低 | 用 qmake；如坚持统一 CMake，需补 `if(UNIX)` 分支 + `find_package(VTK)`，工作量约半天 |
| OpenGL 驱动（虚拟机/集显黑屏） | 中 | 装 mesa-utils 诊断；虚拟机开 3D 加速或用 NVIDIA 驱动 |
| 串口权限 / 设备名差异 | 低 | dialout 组 + `/dev/ttyUSB*` 检测，UI 已有串口配置界面 |
| 远端 Linux 分支与当前分支差异大 | 低 | 先 diff，只取可复用的构建/脚本部分，代码以当前分支为主 |

## 7. 附：远端 Linux 分支参考

- `origin/FOSD-Linux`
- `origin/FOSD-Rainbow-Linux`

拉取方式：`git fetch origin && git log --oneline origin/FOSD-Linux`，重点看其对 `.pro`/构建脚本/路径的处理，可大幅缩短本指南阶段 2、3 的实际耗时。

---

*本指南中所有"已查证"内容均来自对仓库的本地探查（.pro 平台分支、VTK-Linux-debug 库、源码编码扫描、硬编码路径 grep、远端分支列表）；未执行过的命令已标注"待执行验证"，请在目标 Linux 环境实测后更新本文。*
