# VTK 8.2 依赖库清单与管理规范

> 项目: FOSD-Pro  
> VTK版本: 8.2.0  
> 最后更新: 2026-07-10  
> 库总数: 126  
> 当前使用: 39  
> 已移除: 87

---

## 一、依赖库清单

### Common 核心

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ✅ 使用 | `vtkCommonColor` | 颜色定义（命名颜色表） |
| ✅ 使用 | `vtkCommonComputationalGeometry` | 计算几何（插值/样条/线/三角） |
| ✅ 使用 | `vtkCommonCore` | 核心基础（SmartPointer/数组/类型/命令） |
| ✅ 使用 | `vtkCommonDataModel` | 数据模型（PolyData/Points/Cells/DataSet） |
| ✅ 使用 | `vtkCommonExecutionModel` | 执行模型（Filter管线基类） |
| ✅ 使用 | `vtkCommonMath` | 数学工具（矩阵/向量/vtkMath） |
| ✅ 使用 | `vtkCommonMisc` | 杂项工具（版本信息等） |
| ✅ 使用 | `vtkCommonSystem` | 系统工具（线程/目录/定时器） |
| ✅ 使用 | `vtkCommonTransforms` | 变换（vtkTransform/vtkMatrix4x4） |

### Filters 过滤器

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkFiltersAMR` | AMR自适应网格过滤器 |
| ✅ 使用 | `vtkFiltersCore` | 核心过滤器（Append/Clean/Clip/Threshold） |
| ❌ 移除 | `vtkFiltersExtraction` | 数据提取过滤器 |
| ❌ 移除 | `vtkFiltersFlowPaths` | 流线/迹线过滤器 |
| ✅ 使用 | `vtkFiltersGeneral` | 通用过滤器（Transform/Contour） |
| ❌ 移除 | `vtkFiltersGeneric` | 通用数据过滤器 |
| ❌ 移除 | `vtkFiltersGeometry` | 几何提取（表面/边提取） |
| ❌ 移除 | `vtkFiltersHybrid` | 混合过滤器（PolyData/图像交叉处理） |
| ❌ 移除 | `vtkFiltersHyperTree` | 超树网格过滤器 |
| ❌ 移除 | `vtkFiltersImaging` | 图像过滤器 |
| ❌ 移除 | `vtkFiltersModeling` | 建模过滤器（Delaunay/旋转体/挤压/Subdivision） |
| ❌ 移除 | `vtkFiltersParallel` | 并行过滤器 |
| ❌ 移除 | `vtkFiltersParallelImaging` | 并行图像过滤器 |
| ❌ 移除 | `vtkFiltersPoints` | 点云过滤器 |
| ❌ 移除 | `vtkFiltersProgrammable` | 可编程过滤器（自定义回调） |
| ❌ 移除 | `vtkFiltersSMP` | SMP多线程过滤器 |
| ❌ 移除 | `vtkFiltersSelection` | 选择过滤器 |
| ✅ 使用 | `vtkFiltersSources` | 几何源（Sphere/Cube/Plane/Arrow/Line/Cylinder） |
| ❌ 移除 | `vtkFiltersStatistics` | 统计过滤器 |
| ✅ 使用 | `vtkFiltersTexture` | 纹理映射过滤器（TextureMapToSphere/Plane/Cylinder） |
| ❌ 移除 | `vtkFiltersTopology` | 拓扑过滤器 |
| ❌ 移除 | `vtkFiltersVerdict` | 网格质量评估 |

### Rendering 渲染

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ✅ 使用 | `vtkRenderingAnnotation` | 标注渲染（坐标轴/标题/标量条） |
| ❌ 移除 | `vtkRenderingContext2D` | 2D上下文渲染 |
| ❌ 移除 | `vtkRenderingContextOpenGL2` | OpenGL2 2D上下文 |
| ✅ 使用 | `vtkRenderingCore` | 渲染核心（Renderer/Actor/Mapper/Camera） |
| ✅ 使用 | `vtkRenderingFreeType` | FreeType文本渲染 |
| ❌ 移除 | `vtkRenderingGL2PSOpenGL2` | GL2PS矢量导出 |
| ❌ 移除 | `vtkRenderingImage` | 图像渲染 |
| ❌ 移除 | `vtkRenderingLOD` | LOD细节层次渲染 |
| ❌ 移除 | `vtkRenderingLabel` | 标签渲染 |
| ✅ 使用 | `vtkRenderingOpenGL2` | OpenGL2渲染后端 |
| ✅ 使用 | `vtkRenderingQt` | Qt渲染集成 |
| ❌ 移除 | `vtkRenderingVolume` | 体渲染 |
| ❌ 移除 | `vtkRenderingVolumeOpenGL2` | OpenGL2体渲染 |

### Interaction 交互

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkInteractionImage` | 图像交互 |
| ✅ 使用 | `vtkInteractionStyle` | 交互样式（Trackball/Surface等） |
| ✅ 使用 | `vtkInteractionWidgets` | 交互控件（Picker/Slider/Box/Plane等） |

### IO 输入输出

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkIOAMR` | AMR数据IO |
| ❌ 移除 | `vtkIOAsynchronous` | 异步IO |
| ❌ 移除 | `vtkIOCityGML` | CityGML城市模型IO |
| ✅ 使用 | `vtkIOCore` | IO核心基类 |
| ❌ 移除 | `vtkIOEnSight` | EnSight数据格式IO |
| ❌ 移除 | `vtkIOExodus` | Exodus数据格式IO |
| ❌ 移除 | `vtkIOExport` | 数据导出（场景/模型） |
| ❌ 移除 | `vtkIOExportOpenGL2` | OpenGL2渲染导出 |
| ❌ 移除 | `vtkIOExportPDF` | PDF导出 |
| ✅ 使用 | `vtkIOGeometry` | 几何IO（STL/OBJ/PLY读写） |
| ✅ 使用 | `vtkIOImage` | 图像IO（JPEG/PNG/BMP/TIFF读写） |
| ❌ 移除 | `vtkIOImport` | 数据导入 |
| ❌ 移除 | `vtkIOInfovis` | 信息可视化IO |
| ❌ 移除 | `vtkIOLSDyna` | LS-DYNA数据IO |
| ✅ 使用 | `vtkIOLegacy` | VTK旧版格式IO（.vtk文件） |
| ❌ 移除 | `vtkIOMINC` | MINC医学图像IO |
| ❌ 移除 | `vtkIOMovie` | 视频/动画IO |
| ❌ 移除 | `vtkIONetCDF` | NetCDF科学数据IO |
| ❌ 移除 | `vtkIOPLY` | PLY点云格式IO |
| ❌ 移除 | `vtkIOParallel` | 并行IO |
| ❌ 移除 | `vtkIOParallelXML` | 并行XML IO |
| ❌ 移除 | `vtkIOSQL` | SQL数据库IO |
| ❌ 移除 | `vtkIOSegY` | Seg-Y地震数据IO |
| ❌ 移除 | `vtkIOTecplotTable` | Tecplot数据IO |
| ❌ 移除 | `vtkIOVeraOut` | Vera输出数据IO |
| ❌ 移除 | `vtkIOVideo` | 视频IO |
| ❌ 移除 | `vtkIOXML` | XML格式IO（VTU/VTP等） |
| ❌ 移除 | `vtkIOXMLParser` | XML解析器 |

### Imaging 图像处理

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkImagingColor` | 图像颜色处理 |
| ❌ 移除 | `vtkImagingCore` | 图像处理核心（缩放/翻转/切片） |
| ❌ 移除 | `vtkImagingFourier` | 图像傅里叶变换 |
| ❌ 移除 | `vtkImagingGeneral` | 图像通用处理（卷积/腐蚀/膨胀） |
| ❌ 移除 | `vtkImagingHybrid` | 图像混合处理 |
| ❌ 移除 | `vtkImagingMath` | 图像数学运算 |
| ❌ 移除 | `vtkImagingMorphological` | 图像形态学处理 |
| ❌ 移除 | `vtkImagingSources` | 图像数据源（渐变/噪声生成） |
| ❌ 移除 | `vtkImagingStatistics` | 图像统计 |
| ❌ 移除 | `vtkImagingStencil` | 图像模板处理 |

### Views 视图

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkViewsContext2D` | 2D上下文视图 |
| ❌ 移除 | `vtkViewsCore` | 视图核心 |
| ❌ 移除 | `vtkViewsInfovis` | 信息可视化视图 |
| ❌ 移除 | `vtkViewsQt` | Qt视图集成 |

### Infovis 信息可视化

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkInfovisCore` | 信息可视化核心 |
| ❌ 移除 | `vtkInfovisLayout` | 信息可视化布局算法 |

### Geovis 地理可视化

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkGeovisCore` | 地理可视化核心 |

### Charts 图表

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkChartsCore` | 图表核心（折线图/柱状图等） |

### Domains 领域

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkDomainsChemistry` | 化学领域可视化 |
| ❌ 移除 | `vtkDomainsChemistryOpenGL2` | 化学领域OpenGL2渲染 |

### Parallel 并行

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkParallelCore` | 并行计算核心（MPI抽象） |

### GUISupport Qt集成

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ✅ 使用 | `vtkGUISupportQt` | Qt GUI集成（QVTKOpenGLNativeWidget） |
| ❌ 移除 | `vtkGUISupportQtOpenGL` | — |
| ❌ 移除 | `vtkGUISupportQtSQL` | Qt SQL集成 |

### ThirdParty 第三方依赖

| 状态 | 库名 | 用途 |
|:----:|------|------|
| ❌ 移除 | `vtkDICOMParser` | DICOM文件解析器 |
| ❌ 移除 | `vtkNetCDF` | NetCDF科学数据格式 |
| ❌ 移除 | `vtkdoubleconversion` | 双精度浮点格式转换 |
| ❌ 移除 | `vtkexodusII` | Exodus II 数据格式支持 |
| ❌ 移除 | `vtkexpat` | XML解析（expat库） |
| ✅ 使用 | `vtkfreetype` | FreeType字体渲染引擎 |
| ✅ 使用 | `vtkgl2ps` | OpenGL到PostScript/PDF导出 |
| ✅ 使用 | `vtkglew` | OpenGL Extension Wrangler |
| ❌ 移除 | `vtkhdf5` | HDF5数据格式支持 |
| ❌ 移除 | `vtkhdf5_hl` | HDF5高级接口 |
| ✅ 使用 | `vtkjpeg` | JPEG图像编解码库 |
| ✅ 使用 | `vtkjsoncpp` | JSON解析库 |
| ❌ 移除 | `vtklibharu` | PDF生成库（libHaru） |
| ❌ 移除 | `vtklibxml2` | XML解析库（libxml2） |
| ✅ 使用 | `vtklz4` | LZ4压缩库 |
| ✅ 使用 | `vtklzma` | LZMA压缩库 |
| ✅ 使用 | `vtkmetaio` | MetaImage格式IO |
| ❌ 移除 | `vtkogg` | OGG多媒体容器格式 |
| ✅ 使用 | `vtkpng` | PNG图像编解码库 |
| ❌ 移除 | `vtkproj` | 地图投影库（PROJ） |
| ✅ 使用 | `vtkpugixml` | 轻量XML解析库 |
| ✅ 使用 | `vtksqlite` | SQLite数据库引擎 |
| ✅ 使用 | `vtksys` | VTK系统工具库（路径/字符串/进程） |
| ❌ 移除 | `vtktheora` | Theora视频编解码 |
| ✅ 使用 | `vtktiff` | TIFF图像编解码库 |
| ❌ 移除 | `vtkverdict` | 网格质量评估库 |
| ✅ 使用 | `vtkzlib` | zlib压缩库 |

---

## 二、头文件 → 源文件 → 库 映射

| VTK头文件 | 所在源文件 | 所属库 |
|-----------|-----------|--------|
| `vtkAutoInit.h` | main.cpp / fosdplugin.cpp / disp3dmodel.h / disp3dwindow.cpp / disp3dinteractor.cpp | `vtkCommonCore` |
| `vtkSmartPointer.h` | disp3dmodel.h / disp3dglobal.h / disp3dbackground.h / trackingpoint.h / disp3dfreshmodelthread.h / disp3dkeypointedit.h | `vtkCommonCore` |
| `vtkDoubleArray.h` | disp3dglobal.h / disp3dfreshmodelthread.h | `vtkCommonCore` |
| `vtkCallbackCommand.h` | disp3dinteractor.cpp | `vtkCommonCore` |
| `vtkLookupTable.h` | disp3dmodel.cpp | `vtkCommonCore` |
| `vtkOutputWindow.h` | main.cpp / fosdplugin.cpp | `vtkCommonCore` |
| `vtkPoints.h` | disp3dmodel.h / disp3dwindow.cpp | `vtkCommonDataModel` |
| `vtkPolyData.h` | disp3dmodel.h / disp3dmodel.cpp | `vtkCommonDataModel` |
| `vtkPointData.h` | disp3dmodel.cpp | `vtkCommonDataModel` |
| `vtkPolyDataMapper.h` | disp3dmodel.h / disp3dwindow.cpp / disp3dinteractor.cpp / disp3dkeypointedit.cpp / disp3dkeypointselect.cpp / disp3dbackground.cpp | `vtkRenderingCore` |
| `vtkRenderWindow.h` | disp3dmodel.h / disp3dbackground.h / disp3dwindow.h / trackingpoint.h | `vtkRenderingCore` |
| `vtkRenderWindowInteractor.h` | disp3dmodel.h / disp3dwindow.h / disp3dinteractor.cpp | `vtkRenderingCore` |
| `vtkRenderer.h` | disp3dmodel.h / disp3dbackground.h / disp3dwindow.cpp / disp3dinteractor.cpp / trackingpoint.h / disp3dkeypointedit.cpp / disp3dkeypointselect.cpp | `vtkRenderingCore` |
| `vtkActor.h` | disp3dmodel.h / disp3dglobal.h / disp3dinteractor.h / trackingpoint.h / disp3dbackground.h / disp3dkeypointedit.h | `vtkRenderingCore` |
| `vtkCamera.h` | disp3dmodel.h / disp3dmodel.cpp / disp3dbackground.cpp | `vtkRenderingCore` |
| `vtkTextActor.h` | disp3dglobal.h / disp3dmodel.h / disp3dinteractor.h / disp3dbackground.cpp | `vtkRenderingCore` |
| `vtkProperty.h` | disp3dmodel.cpp / disp3dbackground.cpp / trackingpoint.cpp / disp3dkeypointedit.cpp | `vtkRenderingCore` |
| `vtkPointPicker.h` | disp3dinteractor.cpp | `vtkRenderingCore` |
| `vtkRenderWidget.h` | disp3dwindow.h | `vtkRenderingCore` |
| `vtkTexture.h` | disp3dbackground.h / disp3dbackground.cpp | `vtkRenderingCore` |
| `vtkGenericOpenGLRenderWindow.h` | disp3dmodel.h / disp3dwindow.cpp | `vtkRenderingOpenGL2` |
| `vtkTextProperty.h` | disp3dmodel.cpp / disp3dinteractor.cpp / disp3dbackground.cpp / trackingpoint.cpp / disp3dkeypointselect.cpp | `vtkRenderingFreeType` |
| `vtkAxesActor.h` | disp3dbackground.h / disp3dbackground.cpp | `vtkRenderingAnnotation` |
| `vtkCaptionActor2D.h` | disp3dwindow.cpp / disp3dbackground.cpp / disp3dfreshmodelthread.cpp / trackingpoint.h / trackingpoint.cpp | `vtkRenderingAnnotation` |
| `vtkSTLReader.h` | disp3dmodel.h / disp3dmodel.cpp | `vtkIOGeometry` |
| `vtkSTLWriter.h` | disp3dmodel.cpp | `vtkIOGeometry` |
| `vtkJPEGReader.h` | disp3dbackground.cpp | `vtkIOImage` |
| `vtkPNGReader.h` | disp3dbackground.cpp | `vtkIOImage` |
| `vtkSphereSource.h` | disp3dmodel.cpp / disp3dinteractor.cpp / disp3dbackground.cpp / trackingpoint.cpp / disp3dkeypointselect.cpp | `vtkFiltersSources` |
| `vtkCubeSource.h` | disp3dkeypointedit.cpp | `vtkFiltersSources` |
| `vtkArrowSource.h` | disp3dbackground.cpp | `vtkFiltersSources` |
| `vtkLineSource.h` | disp3dbackground.cpp | `vtkFiltersSources` |
| `vtkPlaneSource.h` | disp3dbackground.cpp | `vtkFiltersSources` |
| `vtkAppendPolyData.h` | disp3dmodel.cpp / disp3dbackground.cpp | `vtkFiltersCore` |
| `vtkCleanPolyData.h` | disp3dmodel.cpp | `vtkFiltersCore` |
| `vtkTextureMapToSphere.h` | disp3dbackground.cpp | `vtkFiltersTexture` |
| `vtkInteractorStyleTrackballCamera.h` | disp3dmodel.cpp / disp3dinteractor.h | `vtkInteractionStyle` |
| `QVTKOpenGLNativeWidget.h` | main.cpp / fosdplugin.cpp / disp3dmodel.h / disp3dwindow.h / disp3dwindow.cpp | `vtkGUISupportQt` |

---

## 三、依赖管理规范

### 3.1 添加新 VTK 功能的标准流程

```
第1步：确认头文件 → 第2步：确认所属模块 → 第3步：.pro添加库 → 第4步：更新本文档 → 第5步：编译验证
```

**具体操作：**

1. **找到头文件所属模块** — 头文件所在目录即模块名：
   ```
   # 在 VTK 安装目录中搜索
   find lib/VTK/VTK-release/include/vtk-8.2 -name "vtkXxxFilter.h"
   # 输出: vtk-8.2/vtkFiltersCore/vtkXxxFilter.h → 模块: vtkFiltersCore
   ```

2. **在 .pro 中添加库（三个平台都要加）：**
   ```pro
   # Release — 放在对应分类注释下
   LIBS += $$VTK_DIR/lib/vtkFiltersCore-8.2.lib
   # Debug
   LIBS +=$$VTK_DIR/lib/vtkFiltersCore-8.2D.lib
   # Linux
   LIBS += -lvtkFiltersCore-8.2
   ```

3. **更新本文档** — 在「依赖库清单」对应分类中添加条目

4. **编译验证** — 确认无 LNK2019 链接错误

### 3.2 头文件引用原则

```cpp
// ✅ 正确：每个 .h 只引入自己直接使用的 VTK 类型
// trackingpoint.h
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkCaptionActor2D.h>  // 直接使用了 vtkCaptionActor2D*

// ❌ 错误：依赖传递链（别人清理头文件时会断）
// trackingpoint.h
#include <disp3dmodel.h>  // 期望间接引入 vtkCaptionActor2D.h
```

**核心规则：谁用谁 include，不依赖传递链。**

### 3.3 常见 VTK 类型 → 模块速查表

| 你要用的类 | 所属模块 | .pro 库名 |
|-----------|---------|----------|
| vtkPolyData, vtkPoints, vtkPointData | vtkCommonDataModel | vtkCommonDataModel |
| vtkSmartPointer, vtkDoubleArray, vtkLookupTable | vtkCommonCore | vtkCommonCore |
| vtkMath | vtkCommonMath | vtkCommonMath |
| vtkTransform, vtkMatrix4x4 | vtkCommonTransforms | vtkCommonTransforms |
| vtkActor, vtkRenderer, vtkCamera | vtkRenderingCore | vtkRenderingCore |
| vtkPolyDataMapper, vtkDataSetMapper | vtkRenderingCore | vtkRenderingCore |
| vtkProperty, vtkTexture | vtkRenderingCore | vtkRenderingCore |
| vtkTextProperty | vtkRenderingFreeType | vtkRenderingFreeType |
| vtkAxesActor, vtkCaptionActor2D, vtkScalarBarActor | vtkRenderingAnnotation | vtkRenderingAnnotation |
| vtkGenericOpenGLRenderWindow | vtkRenderingOpenGL2 | vtkRenderingOpenGL2 |
| vtkSTLReader, vtkSTLWriter, vtkPLYReader | vtkIOGeometry | vtkIOGeometry |
| vtkJPEGReader, vtkPNGReader, vtkBMPReader | vtkIOImage | vtkIOImage |
| vtkSphereSource, vtkCubeSource, vtkPlaneSource | vtkFiltersSources | vtkFiltersSources |
| vtkAppendPolyData, vtkCleanPolyData | vtkFiltersCore | vtkFiltersCore |
| vtkClipPolyData, vtkThreshold, vtkStripper | vtkFiltersCore | vtkFiltersCore |
| vtkContourFilter, vtkMarchingCubes | vtkFiltersCore / vtkFiltersModeling | 对应模块 |
| vtkTransformPolyDataFilter, vtkTextureMapToPlane | vtkFiltersGeneral | vtkFiltersGeneral |
| vtkTextureMapToSphere, vtkTextureMapToCylinder | vtkFiltersTexture | vtkFiltersTexture |
| vtkLoopSubdivisionFilter, vtkButterflySubdivisionFilter | vtkFiltersModeling | vtkFiltersModeling |
| vtkWindowedSincPolyDataFilter, vtkSmoothPolyDataFilter | vtkFiltersCore | vtkFiltersCore |
| vtkDelaunay2D, vtkDelaunay3D | vtkFiltersCore | vtkFiltersCore |
| vtkInteractorStyleTrackballCamera | vtkInteractionStyle | vtkInteractionStyle |
| vtkPointPicker, vtkCellPicker | vtkRenderingCore | vtkRenderingCore |
| vtkDistancePolyDataFilter, vtkImplicitPolyDataDistance | vtkFiltersCore | vtkFiltersCore |
| QVTKOpenGLNativeWidget | vtkGUISupportQt | vtkGUISupportQt |

### 3.4 .pro 文件维护规范

- 每个分类下保留 `# ---- 分类名 ----` 注释
- 新增库放在对应分类注释下方
- **三个平台（Release/Debug/Linux）必须同步添加**
- 修改后校验三个平台库数量一致：
  ```bash
  # Release 和 Debug 的 .lib 数量应相等
  grep -c '\.lib' FOSD-Pro.pro  # 总数应为偶数
  ```

### 3.5 定期检查

当项目引入新的 VTK 类型后，运行以下检查确保头文件完整性：

```bash
# 在项目根目录执行 — 检查所有 .h 文件中 VTK 类型是否有对应 #include
grep -rn 'vtk[A-Z]' src/*.h src/**/*.h | grep -v '#include' | grep -v '//' 
```

如果发现链接错误（LNK2019 无法解析的外部符号）：

1. 从错误信息中提取类名（如 `vtkTextureMapToSphere::New`）
2. 在 VTK 安装目录搜索头文件位置确认模块
3. 在 .pro 三平台中补充对应 `.lib`
4. 更新本文档
