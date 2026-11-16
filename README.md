
# FOSD( Fiber Optic Sensing Demodulator) 光纤传感解调仪上位机
> 接收光纤解调仪数据，进行应变解算、温度解耦，首先对应模型变形的三维可视化


## 🆕 V3.0.0 重构版本
当前重构版本定义为 **V3.0.0**，保留原有通信、应变解算与三维形变显示功能，主要重构成果如下：

- **后端模块化**：按通信、协议、共享数据、配置、解算、存储和可视化划分独立模块，明确接口和线程边界，供不同前端复用。
- **前端架构简化**：FOSD-Classic-QWidgets 采用 View → Presenter → WorkbenchModel 三层 MVP，移除旧 Backend/runtime 转发链及废弃界面；入口、表单与业务、资源和公共控件分别组织。
- **业务链路完善**：串口、UDP、TCP 通信接入统一数据链路，支持波长协议解析、通道倍率与偏置标定、HYY IFE 解算、VTK 实时形变及 CSV/SQLite 数据存储。
- **界面与交互优化**：完善深浅主题、彩色收发日志、快捷指令、曲线交互、模型与场景配置、选中点信息，以及无边框缩放和自适应 16:9 启动布局。
- **构建与验证统一**：采用 CMake 模块目标和独立构建预设，分别构建 Classic 主程序、QML 通信助手及 QWidgets 通信助手，并提供模块测试、回环模拟器和工作台集成回归。

V3.0.0 的实际目录和架构见 [Classic 前端说明](ui/FOSD-Classic-QWidgets/README.md)，构建与运行方式见 [独立构建预设](doc/build-presets.md)，完整文档见 [开发文档入口](doc/README.md)。下文沿用原版 README 的介绍、环境要求、使用说明和历史目录示例。

## 📋 项目介绍
### 核心功能
- 功能1，实现与解调仪的USART、TCP/UDP连接通信控制，接收解算解调仪采集数据;
- 功能2，实现模型的显示和形变的实时显示;

### 技术栈
- 开发语言：C++ 11 / C
- 核心框架/库：VTK(8.2.0) / Qt(Qt 5.12.2)
- 开发工具：Qt Creater（4.8.2）
- 运行环境：Win 10+


## 🛠️ 环境要求与依赖安装
### 环境要求
- Qt 5.12.2；Cmake 3.17.1；VS 2017；VTK 8.2.0
- 内存：建议 2GB 及以上
- 存储空间：至少 200MB（不含测试数据）

### 依赖安装步骤
> To be continue

## 🚀 快速启动
### 基础功能示例 
> To be continue

### 配置参数说明
| 参数名 | 类型 | 说明 | 默认值 |
|--------|------|------|--------|
| \ | \ | \ | \ |


## 📁 项目目录结构
> FOSD-Pro 后版本开始沿用下述项目结构
```
{{项目仓库名}}/
├── bulid/              # 编译构建结果(Debug|Release在统一目录下)(不进行同步)
|   ├── Window
|   └── Linux
├── config/             # 软件配置文件
├── Data/               # 相关模型/点阵数据
├── Doc/                # 说明相关文档
├── lib/                # 软件依赖核心库
|   ├── CustomUi        # 外界UI控件依赖库
|   └── VTK/            # VTK的Debug|Release Win版本编译结果
├── res/                # 资源库
├── src/                # 核心源代码目录
│   ├── main.cpp
│   ├── app             # Qt UI界面(.cpp/.h/.ui)
│   ├── communication   # 通信接口函数
│   ├── core            # 核心代码/函数
│   ├── protocol        # 通信解码协议
│   ├── test            # 测试代码
|   ├── visualization   # 可视化代码
│   └── utils           # 通用功能单元
├── FOSD-Pro.pro        # Qmake编译配置
└── README.md           # 项目说明文档（本文档）
```

## 🤝 贡献指南
欢迎各位开发者贡献代码，助力项目完善！贡献流程如下：
1. Fork 本仓库（点击 GitHub 页面右上角 Fork 按钮）
2. 克隆 Fork 后的仓库到本地：`git clone https://github.com/你的用户名/{{项目仓库名}}.git`
3. 创建特性分支：`git checkout -b feature/{{特性名称，如：add-excel-export}}`
4. 编写代码并提交：`git commit -m "feat: 新增Excel导出功能"`
5. 推送分支到远程：`git push origin feature/{{特性名称}}`
6. 打开 Pull Request（PR）：在 GitHub 本仓库页面提交 PR，描述清楚修改内容

### 编码规范
- 提交信息格式：`类型: 描述`（类型：feat-新功能，fix-修复bug，docs-文档更新，style-代码格式调整）

## 🙏 致谢
- 感谢 VTK提供的技术支持
- 感谢 师兄楚辉超、凌立瞳的指导和帮助

## 📞 联系方式（可选）
- 作者：SuningJan
- 邮箱：1229419727@qq.com
- 项目地址：https://github.com/Swimers/FOSD_Project
