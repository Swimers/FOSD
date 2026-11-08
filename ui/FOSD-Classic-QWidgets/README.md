# Classic 界面

当前采用三层 MVP：View（表单与展示）→ Presenter（交互和界面状态）→ WorkbenchModel（业务编排）。Model 使用 src 的通信、协议、解算、存储和三维 Control。

```text
apps/FOSD-Classic-QWidgets/
  main.cpp、CMakeLists.txt       程序启动与可执行文件部署
ui/FOSD-Classic-QWidgets/
  application/                 ClassicWorkbench 装配、工作台容器与 Presenter 协调
  model/                       WorkbenchModel、业务状态及点集合操作
  features/
    mainwindow/                主窗口导航和主题
    connection/                通信、日志、曲线、12 路倍率/偏置标定
      presenters/              输入转换与展示状态
      views/                   表单、View 和 ConnectionRenderers
    visualization/             三维、模型、场景和关键点
      presenters/              VisualizationPresenter、ModelPanelPresenter
      views/docks/             停靠面板
      views/keypoints/         点集合界面
      views/dialogs/           颜色选择色卡
    data/                      HardwareDataPresenter 与硬件数据表
  common/                      IView、基础绑定、格式化和资源初始化
  controls/                    自研通用控件
  third_party/qcustomplot/     第三方曲线控件及原许可证说明
  resources/                   classic.qrc、icons、styles
  tests/                       控件、Model/Presenter、选点与完整工作台回归
```

旧 FeatureBackend、IBackend、WorkbenchBackend、WorkbenchServices 和通用 UiRequest/UiResponse 转发链已移除，WorkbenchRuntime 的业务职责进入 Model，展示职责进入 Presenter/View。已有业务后端接口与线程边界保持不变。

CMake 保留 contracts、controls、widgets、presenters、application，新增 model 并移除 backends/runtime 目标。widgets 只依赖控件和视图契约；完整工作台由 fosd_classic_application 装配。构建预设和 EXE 输出路径保持不变。

迁移说明、状态机制和审阅要点见 [三层 MVP 连接指南](../../doc/Architectural/classic-mvp-ui-backend-connection.md)。历史功能记录见 [通信与标定](../../doc/Classic-Communication-Logs-Calibration-2026-11-07.md)、[2026-11-04 迁移实施记录](../../doc/MigrationCompletion-2026-11-04/README.md)。
