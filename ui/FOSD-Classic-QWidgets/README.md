# Classic 界面

2026-11-08 整理后的源码边界：`apps/FOSD-Classic-QWidgets` 只保留 `main.cpp` 和可执行文件的 CMake 配置；所有界面、界面服务适配、Qt/VTK 装配与测试集中在本目录。

```text
apps/FOSD-Classic-QWidgets/
  main.cpp                     程序启动入口
  CMakeLists.txt               可执行文件及运行依赖部署
ui/FOSD-Classic-QWidgets/
  application/                 工作台 View/Presenter/Backend 与 ClassicWorkbench 装配
    runtime/                   通信、解算、存储、三维 Control 的界面服务适配
  features/
    mainwindow/                主窗口与导航
    connection/                通信、日志、波长曲线、通道标定
    visualization/             三维显示、模型配置、关键点与场景设置
      backends/                三维和模型请求适配
      presenters/              三维和模型交互路由
      views/
        docks/                 模型、场景、信息面板
        keypoints/             关键点编辑与显示点选择
        dialogs/               颜色选择色卡
        WorkbenchRenderSurface.ui  Qt/VTK 渲染宿主表单
    data/                      硬件数据表及对应交互（保留 Tracking 类名兼容现有接口）
  common/                      视图契约、绑定、格式化与资源初始化代码
  controls/                    自研通用控件
  third_party/qcustomplot/     第三方曲线控件，保留原许可证说明
  resources/
    classic.qrc                图标与主题资源清单
    icons/                     当前使用的图标
    styles/                    深浅主题样式
  tests/                       控件、运行时、选点与完整工作台集成测试
```

同一功能的 `.ui`、View、Presenter 和 Backend 位于同一 `features` 模块。公共控件和资源与功能源码分开；`src/` 中各业务 Control 的边界保持不变。删除已废弃账户/二维页面的空目录及未引用资源，保留现有图标和主题的资源 URL。

构建目标继续保留 `fosd_classic_contracts/backends/presenters/controls/widgets/runtime`，新增 `fosd_classic_application` 封装完整 Qt/VTK 工作台，供主程序和集成测试共同链接。完整工作台装配只在 `FOSD_BUILD_WORKBENCH_APP=ON` 时构建，其他入口仍可单独使用共享 Widgets 库。构建命令及输出 EXE 路径保持不变，测试源文件与测试注册统一归入 `tests/`。

目录整理后已通过 Classic Release 编译、18/18 项回归测试及独立配置的 FOSD-Win Release 编译，并检查全部 12 个 Designer 表单的资源路径和对象名、11 项资源清单及旧源码路径残留。

当前实现及验证见 [2026-11-04 迁移实施记录](../../doc/MigrationCompletion-2026-11-04/README.md)。使用 INI 配置，仅保留 HYY IFE；账户、独立追踪点和未实现旧功能已裁剪。View 只处理界面，Presenter 路由意图，应用层 WorkbenchRuntime 组合各后端 Control。

通信页最新调整见 [通信日志与通道标定记录](../../doc/Classic-Communication-Logs-Calibration-2026-11-07.md)：本地地址刷新、曲线通道筛选、彩色收发日志，以及 `channlCalibration` 标定入口；旧二维页面已移除。
