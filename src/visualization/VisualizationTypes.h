#pragma once
#include "core/Datamodel/ModelData.h"
#include <QMetaType>
#include <QString>
#include <array>

#include <vtkSmartPointer.h>
#include <vtkType.h>

class vtkActor;
class vtkBillboardTextActor3D;
class vtkSphereSource;

namespace fosd {

namespace visualization {

// 颜色映射相关
enum class ScalarField { 
    DisplacementX, 
    DisplacementY, 
    DisplacementZ, 
    Magnitude, 
    Custom };
enum class ScalarMappingMode { Normalized, Physical };
enum class ScalarValueMode { Signed, Absolute };
enum class ColorRangeMode { Automatic, Manual };
struct ScalarColorSettings {
    bool enabled = false;
    ScalarField field = ScalarField::Magnitude;
    ScalarMappingMode mappingMode = ScalarMappingMode::Normalized;
    ScalarValueMode valueMode = ScalarValueMode::Signed;
    ColorRangeMode rangeMode = ColorRangeMode::Manual;
    double minimum = 0.0;
    double maximum = 50.0;
    double hueStart = 0.6;
    double hueEnd = 0.1;
};
struct PointScalarField {               // 通用点标量场：与硬件应变数组解耦，任何逐点物理量都可经此结构送入着色。键必须与模型的 VTK 点 ID 一致；自定义场须显式绑定模型版本，版本不符则拒绝应用。
    QString id;                         // 场标识，用于判空与区分不同数据源
    QString label;                      // 显示名称，供图例/界面使用
    QString unit;                       // 物理单位，如 "mm"
    quint64 modelRevision = 0;          // 绑定的模型版本，防止旧数据套用到新模型
    QMap<core::PointId, double> values; // 键为 VTK 点 ID，值为该点的标量值
};
struct ScalarColorRange {
    double dataMinimum = 0.0; // After Signed/Absolute, before normalization.
    double dataMaximum = 0.0;
    double lookupMinimum = 0.0;
    double lookupMaximum = 1.0;
};

// 相机环境相关
enum class CameraPreset { Isometric, Front, Back, Left, Right, Top, Bottom };
enum class GridPlane { XZ, XY, YZ };
enum class CameraProjection { Perspective, Parallel };
struct AxesSettings {
    bool visible = false;
    bool labelsVisible = true;
    double lengthMm = 100.0;
    std::array<double, 3> originMm{{0, 0, 0}};
    int fontSize = 10;
};
struct GridSettings {
    std::array<bool, 3> visible{{false, false, false}}; // XZ, XY, YZ.
    double halfExtentMm = 1500.0;
    int divisions = 50; // Intervals from the origin to each edge.
    std::array<double, 3> originMm{{0, 0, 0}};
    std::array<double, 3> color{{0.7, 0.7, 0.7}};
    double opacity = 0.1;
    double lineWidth = 1.0;
};
struct BackgroundSettings {
    bool gradient = false;
    std::array<double, 3> bottomColor{{0, 0, 0}};
    std::array<double, 3> topColor{{0.2, 0.3, 0.5}};
};
struct CameraSettings {
    CameraProjection projection = CameraProjection::Perspective;
    double viewAngleDegrees = 30.0;
    double parallelScaleMm = 1000.0; // Half the visible vertical extent.
    bool locked = false;             // Mouse camera lock; explicit API changes remain allowed.
};
struct CameraPose {
    std::array<double, 3> positionMm{{0, 0, 1}};
    std::array<double, 3> focalPointMm{{0, 0, 0}};
    std::array<double, 3> viewUp{{0, 1, 0}};
};
struct WorldSettings {
    bool groundVisible = false;
    bool skyVisible = false;
    double groundHalfExtentMm = 10000.0;
    double groundYmm = -100.0; // Legacy ground lies in XZ (Y is constant).
    double skyRadiusMm = 20000.0;
    std::array<double, 3> centerMm{{0, 0, 0}};
    std::array<double, 3> groundColor{{0.3, 0.6, 0.3}};
    std::array<double, 3> skyColor{{0.6, 0.85, 1.0}};
    QString groundTexturePath; // Empty = solid color; file or Qt resource path.
    QString skyTexturePath;
};

// 关键点相关
struct KeyPointDefinition {
    vtkIdType pointId = -1;
    QString label;
};
struct KeyPointStyle {
    double radiusMm = 5.0;
    std::array<double, 3> color{{1.0, 0.3, 0.1}};
    std::array<double, 3> labelColor{{1.0, 1.0, 1.0}};
    std::array<double, 3> labelOffsetMm{{0.0, 0.0, 8.0}};
    int fontSize = 14;
    int sphereResolution = 16;
    bool labelsVisible = true;
};
struct KeyPointActors {
    vtkIdType pointId = -1;
    vtkSmartPointer<vtkSphereSource> source;
    vtkSmartPointer<vtkActor> sphere;
    vtkSmartPointer<vtkBillboardTextActor3D> label;
};

// 拾取点相关
struct PickingSettings {
    bool enabled = true;
    bool onlyKeyPoints = false;
    bool clearOnCameraInteraction = false;  // 标志位，旋转/平移/缩放相机时，是否自动取消当前选中的点
    double tolerance = 0.009;     // Fraction of render-window diagonal (VTK).
    int clickTolerancePixels = 4; // Device pixels, not Qt logical pixels.
};
struct PickedPointInfo {
    vtkIdType pointId = -1;
    std::array<double, 3> originalPositionMm{{0.0, 0.0, 0.0}};
    std::array<double, 3> displayPositionMm{{0.0, 0.0, 0.0}};
    std::array<double, 3> displacementMm{{0.0, 0.0, 0.0}}; // Unscaled physical displacement.
    bool valid = false;
};

} // namespace visualization
} // namespace fosd

Q_DECLARE_METATYPE(fosd::core::ModelDataPtr)
Q_DECLARE_METATYPE(fosd::visualization::ScalarField)
Q_DECLARE_METATYPE(fosd::visualization::ColorRangeMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarMappingMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarValueMode)
Q_DECLARE_METATYPE(fosd::visualization::ScalarColorSettings)
Q_DECLARE_METATYPE(fosd::visualization::PointScalarField)
Q_DECLARE_METATYPE(fosd::visualization::CameraPreset)
Q_DECLARE_METATYPE(fosd::visualization::KeyPointDefinition)
Q_DECLARE_METATYPE(fosd::visualization::KeyPointStyle)
Q_DECLARE_METATYPE(fosd::visualization::PickingSettings)
Q_DECLARE_METATYPE(fosd::visualization::PickedPointInfo)
