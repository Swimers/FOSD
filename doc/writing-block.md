# FOSD 可视化子模块架构设计

## 1. 技术环境与设计目标

- **技术栈**：C++11、Qt 5.12、VTK 8.2、Qt Widgets、MVP。
- **模块位置**：`src/visualization/`。
- **主要功能**：模型导入与优化、变形场显示、颜色映射、关键点管理、鼠标拾取、相机及场景控制。
- **核心原则**：可视化只负责数据展示，不执行波长标定、IFE 求解和变形场计算。
- **前端解耦**：只预留公共接口和 Qt 信号，不实现 View、Presenter，也不直接操作 QLabel 等前端控件。

## 2. 推荐目录结构

```text
src/visualization/
├── CMakeLists.txt
├── VisualizationControl.h/.cpp     # 模块统一入口、场景管理与刷新调度
├── VisualizationTypes.h           # 配置、选点结果等公共数据类型
│
├── mesh/
│   └── MeshManager.h/.cpp         # 模型导入、优化、原始点序列
├── render/
│   ├── DeformationRenderer.h/.cpp # 动态网格、Mapper、Actor
│   └── ScalarColorMapper.h/.cpp   # 标量数组、颜色映射
├── marker/
│   └── KeyPointManager.h/.cpp     # 关键点、追踪点及标签
├── interaction/
│   ├── ModelInteractorStyle.h/.cpp# 鼠标与相机交互
│   └── PointPicker.h/.cpp         # 拾取及邻近点查询
└── scene/
    └── SceneEnvironment.h/.cpp    # 坐标轴、网格、背景及相机
```

## 3. VTK 渲染管线及对象归属

渲染关系：

```text
QVTKOpenGLNativeWidget             [前端]
    └── vtkGenericOpenGLRenderWindow [前端]
         └── vtkRenderer            [VisualizationControl]
              ├── vtkActor          [DeformationRenderer]
              │    └── vtkPolyDataMapper
              │         └── vtkPolyData
              ├── 关键点 Actors     [KeyPointManager]
              └── 场景 Actors       [SceneEnvironment]
```

| VTK 对象 | 创建与管理组件 |
|---|---|
| `QVTKOpenGLNativeWidget` | 前端显示宿主 |
| `vtkGenericOpenGLRenderWindow` | 前端显示宿主 |
| `vtkRenderer` | `VisualizationControl` |
| 原始 `vtkPolyData` | `MeshManager` |
| 动态 `vtkPolyData` | `DeformationRenderer` |
| 模型 `vtkMapper` / `vtkActor` | `DeformationRenderer` |
| `vtkLookupTable`、标量配置 | `ScalarColorMapper` |
| 关键点与标签 Actor | `KeyPointManager` |
| `vtkPicker` / `vtkInteractorStyle` | 交互组件 |
| 相机、坐标轴、背景 Actor | `SceneEnvironment` |

**管理约定：**

- VTK 对象由对应组件通过 `vtkSmartPointer` 管理。
- 组件之间可传递非拥有的 VTK 原始指针，必须保证被引用对象仍然存活。
- `VisualizationControl` 统一管理 Renderer、组件协作与 `Render()`。
- 功能组件只修改自己的 VTK 对象，不自行调用 `Render()`。
- 前端不直接操作内部 Actor、Mapper、PolyData。

## 4. 子模块职责

| 组件 | 核心职责 |
|---|---|
| `VisualizationControl` | 对外 API、Renderer 管理、渲染刷新、错误与交互信号 |
| `MeshManager` | 加载 STL、网格优化、导出、获取原始点坐标及 ID |
| `DeformationRenderer` | 接收位移场、更新显示网格、控制变形放大倍数 |
| `ScalarColorMapper` | 显示 X/Y/Z 位移及位移模长、配置颜色范围 |
| `KeyPointManager` | 管理关键点小球、标签、可见性及位置更新 |
| `PointPicker` | 拾取模型顶点、查询邻近点、输出结构化结果 |
| `ModelInteractorStyle` | 旋转、平移、缩放及自定义鼠标交互 |
| `SceneEnvironment` | 背景、坐标轴、网格、相机视角与状态 |

## 5. 与 Core、Calculation 的数据接口

使用现有的数据模型：

```cpp
// Core / ModelData.h
core::ModelDataPtr
core::CalculationResultPtr
core::CalculationRepositoryPtr
```

**模型加载：**

```text
MeshManager 加载、优化模型
    ↓
vtkPolyData 提取 PointId + XYZ
    ↓
生成 core::ModelData
    ↓
VisualizationControl::modelLoaded(model)
    ↓
应用层调用 CalculationControl::setModel(*model)
```

- 原始点 ID 必须与 VTK 网格 ID 一致。
- 必须在网格优化完成后提取最终点序列。
- 模型点坐标统一使用毫米（mm）。
- 原始模型应保持不变。

**变形场显示：**

```text
CalculationWorker
    ↓
CalculationRepository::publish()
    ↓
VisualizationControl 定时读取 latest()
    ↓
DeformationRenderer 更新顶点
    ↓
ScalarColorMapper 更新颜色
    ↓
KeyPointManager 更新标记
    ↓
VisualizationControl 统一 Render()
```

变形显示计算：

```text
displayPosition = originalPosition
                + scale * displacement
```

- `scale` 仅影响显示，不修改真实解算结果。
- 检查 `modelRevision` 与当前已生效模型版本一致。
- 颜色映射使用真实位移值，不应受到显示放大倍数影响。
- 原始网格与动态显示网格分别保存。

## 6. VisualizationControl 对外预留接口

```cpp
// 渲染窗口绑定
bool attach(vtkRenderWindow *window);
void detach();

// 模型管理
bool loadModel(const QString &path);
void unloadModel();
core::ModelDataPtr originalModel() const;

// 解算结果
void setResultRepository(core::CalculationRepositoryPtr repo);
void applyResult(const core::CalculationResultPtr &result);
void setExpectedModelRevision(quint64 revision);
void setDeformationScale(double scale);
void resetDeformation();

// 颜色映射
void setScalarField(ScalarField field);
void setColorRangeMode(ColorRangeMode mode);
void setColorRange(double min, double max);

// 关键点
void setKeyPoints(const QVector<KeyPointDefinition> &points);
void showKeyPoints(bool visible);
void clearSelection();

// 场景与相机
void showAxes(bool visible);
void showGrid(bool visible);
void setCameraPreset(CameraPreset preset);
void setCameraLocked(bool locked);
void resetCamera();

// 对外信号
signals:
    void modelLoaded(const core::ModelDataPtr &model);
    void pointPicked(const PickedPointInfo &info);
    void errorOccurred(const QString &message);
```

以上为接口草案，公共类型统一放入 `VisualizationTypes.h`。

## 7. Qt 与 VTK 绑定

前端创建显示宿主：

```cpp
auto *widget = new QVTKOpenGLNativeWidget(parent);

auto window =
    vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

widget->SetRenderWindow(window);
```

应用层完成绑定：

```cpp
visualization->attach(window.GetPointer());
```

`VisualizationControl::attach()` 内部：

```cpp
m_renderWindow = window; // 非拥有指针
m_renderWindow->AddRenderer(m_renderer);
```

渲染时：

```cpp
m_renderWindow->Render();
```

约定：

- Qt 5.12 + VTK 8.2 使用 `QVTKOpenGLNativeWidget` 与 `vtkGenericOpenGLRenderWindow`。
- 前端负责显示窗口的生命周期。
- VisualizationControl 负责 Renderer 的生命周期。
- 前端销毁 RenderWindow 之前必须调用 `detach()`。
- 初始化 QVTK 控件前设置合适的默认 `QSurfaceFormat`。

## 8. 线程及刷新约束

| 线程 | 职责 |
|---|---|
| 通信/协议线程 | 接收并解包数据 |
| 解算线程 | 应变标定、IFE 求解、完整变形场计算 |
| GUI 线程 | VTK 对象更新、鼠标交互、渲染 |
| 存储线程 | 原始数据及解算结果保存 |

- **所有 VTK 渲染管线操作原则上在 GUI 线程执行。**
- 可视化模块不再创建旧版的 `disp3dfreshmodelthread`。
- 使用 GUI 线程 `QTimer` 定时获取 `CalculationRepository::latest()`。
- 可采用约 33 ms 刷新周期，实际频率按性能调整。
- 相同 `revision` 不重复更新。
- 一次刷新只调用一次 `Render()`。
- 显示允许跳过中间帧；完整存储必须使用逐帧信号或队列。

## 9. 模块依赖与开发约束

```text
               Core / Datamodel
                ▲           ▲
                │           │
          Calculation   Visualization
                            │
                           VTK

MVP 前端 → VisualizationControl
```

- `visualization` 依赖 Core、Qt、VTK。
- `calculation` 不直接依赖 Visualization。
- `visualization` 不直接依赖 CalculationWorker、IFE、存储模块。
- 跨模块传递 Core 数据结构，不传递 VTK 对象。
- 关键点拾取结果通过 Qt 信号传给 MVP 层，不直接修改前端控件。
- 独立空间追踪点若需要插值位移，应由解算模块提供对应结果。
- 暂时保留现有 `core::ModelData` 中的 `vtkIdType`，未来可考虑解耦。

## 10. 推荐开发顺序

1. `MeshManager`：STL 加载、原始网格、点序列导出。
2. `VisualizationControl`：Renderer 和 RenderWindow 绑定。
3. `DeformationRenderer`：接入解算结果、显示变形场。
4. `ScalarColorMapper`：实现位移颜色映射。
5. `KeyPointManager`、`PointPicker`、`ModelInteractorStyle`：实现标记与交互。
6. `SceneEnvironment`：完善相机、坐标轴、背景及网格。

**最终设计原则：前端负责显示容器，VisualizationControl 负责场景管理，各 VTK 功能组件负责自己的渲染对象，Calculation 负责生成变形场，Core 负责跨模块共享数据。**