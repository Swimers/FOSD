# 可视化模块骨架

目录和接口依据 `doc/writing-block.md`，命名空间为 `fosd::visualization`。

当前只实现类的构造/析构、Qt 元对象以及 VTK 交互类所需的标准 New/类型声明；所有业务 API 仅声明，尚无定义，不能调用。没有加载模型、分配渲染对象、绑定窗口、启动定时器或连接业务信号。Control 内的组件指针也保持为空。后续按组件逐步补齐实现，不以空函数或假成功返回值替代业务。

`VisualizationControl` 与 `PointPicker` 预留 Qt 信号，其余组件各自保留 VTK 对象的 `vtkSmartPointer`。RenderWindow 由前端拥有；跨组件的原始指针均为非拥有引用。渲染操作在 GUI 线程完成，后续由 Control 统一调度；本模块不创建工作线程，不依赖 calculation 或 storage。

构建目标为 `fosd_visualization`，依赖 `fosd_core`、Qt Core/Widgets 及 VTK。主程序已链接目标，尚未实例化或调用可视化业务。仓库裁剪版 VTK 包的 CMake 导出仍引用未保留的库/DLL，因此只查找当前所需库，并分别选择 Debug/Release。`FOSD_VISUALIZATION_VTK_ROOT` 指定 SDK 库目录的父目录，头文件沿用 Core 的 `FOSD_VTK_TYPES_INCLUDE_DIR`，两者须属于同一套 VTK 8.2 构建。运行渲染功能时还需部署对应配置的 VTK DLL；骨架阶段不执行渲染。

```powershell
cmake -S . -B build
cmake --build build --config Debug --target fosd_visualization FOSD
cmake --build build --config Release --target fosd_visualization FOSD
```
