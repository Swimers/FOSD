# Classic 通信页面改进与验证（2026-11-07）

本轮修改基于已有通信、存储和绘图能力，未新增设备协议。界面控件定义和布局均在 `connectwindow.ui` 中完成，C++ 只进行绑定、参数转换及绘图交互。

## 功能变化

| 要求 | 实现 |
| --- | --- |
| 删除状态指示/诊断 | 删除 `groupBox_3`、`pbtn_Diagnosis`、动态状态标签，以及 `connectdebug.cpp/.h/.ui`；同步移除枚举、工厂、后端面板注册、Presenter 和 Runtime 诊断路由。异步通信/存储失败仍通过已有错误提示反馈 |
| TCP 模式统一 | `cbox_CommuteChoose` 提供 UDP、TCP Server、TCP Client；移除独立的底部 TCP 角色选择。服务端启用本地监听参数，客户端启用目标参数；运行中禁止直接切换网络类型 |
| 刷新接口 | “刷新”同时枚举串口和本地 IPv4 地址，尽量保留当前有效选择，不重启连接 |
| 串口参数 | 增加数据位、校验位、停止位、流控 QLineEdit，接入完整 `SerialConfig` 校验、保存与恢复；未由请求覆盖的字段保留原配置 |
| 串口名称 | 显示 `COM号:设备名称`，通过 ComboBox 的 item data 保存实际 COM 标识；连接配置中不包含设备描述 |
| 保存交互 | 删除手动快照按钮及对应逻辑。开始保存选择 CSV/SQLite 文件，运行中红色高亮并显示“停止保存”；根据真实 started/stopped/error 回执恢复状态，处理中防止重复操作，取消选文件不启动保存 |
| 曲线主题 | 同步更新画布、坐标区、网格、坐标轴、刻度、图例和曲线的深浅配色 |
| 曲线交互 | 左键拖动平移，滚轮缩放，单击曲线数据点显示通道、时间与波长；手动交互暂停自动跟随，复位按钮/双击恢复；实时更新保留视图范围和仍在采样窗口内的点选 |
| Designer 布局 | 通信、数据、参数/保存三列布局；快捷指令、解算控制及曲线工具栏全部由 `.ui` 定义，不再由 C++ 动态创建 |

串口输入示例：数据位 `5/6/7/8`，校验位 `None/Odd/Even/Mark/Space`（也接受无、奇、偶等界面提示形式），停止位 `1/1.5/2`，流控 `None/Hardware/Software`。串口参数在打开通道时提交；端口的物理支持范围由 Qt/设备驱动确认。

## 验证

Release 构建完成，17/17 个 CTest 目标通过。新增/调整测试覆盖：

- 移除控件与静态布局入口，TCP Server/Client 通过真实回环连接接收 JCS 数据。
- 串口显示文本与实际 COM 标识分离，7 数据位/偶校验/2 停止位/硬件流控的请求与 INI 保存，非法参数拒绝及高级参数保留。
- 同时刷新本地地址与串口，TCP 模式切换时相关输入框的启用状态。
- 保存开始、停止、取消、非法扩展名和异步文件打开失败后的状态恢复，以及 CSV 实际写入。
- 实际鼠标点选、拖拽和滚轮缩放，更新曲线时保留手动范围与点选，深浅主题、视图复位。

已生成并目视检查 `connection-spectrum-dark.png`、`connection-spectrum-light.png`、`connection-serial-layout.png`、`connection-saving.png`，位于构建目录的 Classic 主程序测试文件夹。没有进行真实串口硬件联调。

构建/测试命令：

```powershell
cmake --build build/fosd-widgets-package --config Release --parallel 4
ctest --test-dir build/fosd-widgets-package -C Release --output-on-failure --timeout 120
```

本轮未修改此前审阅报告中的历史结论；该报告记录的是修改前基准，本文件记录本轮完成内容。
