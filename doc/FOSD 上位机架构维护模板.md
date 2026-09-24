# FOSD 上位机架构维护文档

## 1. 架构总览

```text
Presentation
    ↓
Application
    ↓
Domain / Data
    ↓
Processing
    ↓
Communication / Storage / Visualization
```

核心原则：

- UI 只负责显示和交互
- Presenter 负责页面协调
- Service 负责编排业务
- Protocol 只负责协议解析
- Processing 只负责计算
- Storage 只负责存储
- Visualization 只负责渲染
- 线程之间尽量传递数据对象，不共享可变裸指针

---

## 2. 模块职责

| 模块 | 主要职责 |
|---|---|
| Presentation | Qt View、Presenter、页面交互 |
| Application | DeviceControlService、AcquisitionService、RecordingService 等 |
| Communication | UDP、USART、TCP 数据收发 |
| Protocol | 分帧、协议解析、指令编码 |
| Processing | 应变计算、校准、IFE |
| Data | MeasurementFrame、StrainFrame、DeformationFrame、DataHub |
| Storage | 数据队列、CSV/数据库写入 |
| Visualization | VTK Scene、Model、Interactor、Mapping |

---

## 3. 关键数据链

```text
FOSD
 ↓
UDP / USART
 ↓
Transport
 ↓
PacketFramer
 ↓
ProtocolParser
 ↓
MeasurementFrame
 ↓
StrainProcessor
 ↓
StrainFrame
 ↓
DataHub
 ├──→ Recorder
 ├──→ 2D UI
 └──→ IFEEngine
          ↓
   DeformationFrame
          ↓
        3D UI
```

控制链：

```text
User
 ↓
View
 ↓
Presenter
 ↓
Service
 ↓
CommandEncoder
 ↓
Transport
 ↓
FOSD
```

VTK 链：

```text
DeformationFrame
 ↓
VtkAdapter
 ↓
vtkPolyData
 ↓
Mapper
 ↓
Actor
 ↓
Renderer
 ↓
RenderWindow
 ↓
QVTK Widget
```

---

## 4. 线程规划

| 线程 | 主要职责 |
|---|---|
| GUI Thread | QWidget、Presenter、VTK Render |
| IO Thread | UDP、USART、TCP |
| Processing Thread | 分帧、协议解析、应变计算 |
| Algorithm Thread | IFE |
| Storage Thread | 文件写入 |

原则：

```text
IO
 ↓ QByteArray

Processing
 ↓ StrainFrame

Algorithm
 ↓ DeformationFrame

GUI
```

禁止跨线程直接共享：

```text
QWidget*
vtkRenderer*
IFE*
float*
可变全局数组
```

---

## 5. 架构约束

开发新功能前检查：

- 功能属于哪个模块？
- 输入和输出是什么？
- 所在线程是什么？
- 是否产生新的跨层依赖？
- 是否又把业务逻辑写进 View？
- 是否又开始传裸指针？
- 是否能独立测试？

长期禁止：

```text
View → Transport
View → Protocol
View → IFE

Protocol → UI

Processing → QWidget

Storage → UI

Algorithm → VTK
```

推荐依赖：

```text
View
 ↓
Presenter
 ↓
Service
 ↓
Domain / Processing
 ↓
Infrastructure
```

---

## 6. 架构变更记录

| 日期 | 版本 | 变更内容 | 影响模块 |
|---|---|---|---|
| YYYY-MM-DD | vX.X | XXX | XXX |

重大变更建议单独记录 ADR：

```text
ADR-001 UI采用MVP
ADR-002 通信统一ITransport
ADR-003 使用DataHub
ADR-004 VTK只在GUI线程更新
```

---

## 7. 当前重构重点

- [ ] 消除 `SendStrainPtr / GetStrainPtr`
- [ ] 消除 `SendIFEPtr / GetIFEPtr`
- [ ] 拆分 `analyzepackets`
- [ ] 建立统一 `ITransport`
- [ ] 建立 `MeasurementFrame / StrainFrame / DeformationFrame`
- [ ] 将 IFE 从 View 中移出
- [ ] 将 VTK 管线从 `disp3dwindow` 中拆出
- [ ] UI 逐步改为 View + Presenter