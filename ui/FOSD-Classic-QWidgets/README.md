# Classic 界面

当前实现及验证见 [2026-11-04 迁移实施记录](../../doc/MigrationCompletion-2026-11-04/README.md)。使用 INI 配置，仅保留 HYY IFE；账户、独立追踪点和未实现旧功能已裁剪。View 只处理界面，Presenter 路由意图，应用层 WorkbenchRuntime 组合各后端 Control。

通信页最新调整见 [通信日志与通道标定记录](../../doc/Classic-Communication-Logs-Calibration-2026-11-07.md)：本地地址刷新、曲线通道筛选、彩色收发日志，以及 `channlCalibration` 标定入口；旧二维页面已移除。
