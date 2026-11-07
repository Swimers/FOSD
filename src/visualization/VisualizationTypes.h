#pragma once
#include "config/CoreParameters.h"
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
struct CameraPose {
    std::array<double, 3> positionMm{{0, 0, 1}};
    std::array<double, 3> focalPointMm{{0, 0, 0}};
    std::array<double, 3> viewUp{{0, 1, 0}};
};


// 关键点相关
struct KeyPointDefinition {
    vtkIdType pointId = -1;
    QString label;
};

enum class DisplayedPointSource { Model, External };

// Model points follow the deformed mesh; external points keep fixed world coordinates.
// An external ID must not identify a vertex in the current model.
struct DisplayedPointDefinition {
    vtkIdType pointId = -1;
    DisplayedPointSource source = DisplayedPointSource::Model;
    std::array<double, 3> positionMm{{0.0, 0.0, 0.0}};
    QString label;
};

struct KeyPointActors {
    vtkIdType pointId = -1;
    QString text;
    vtkSmartPointer<vtkSphereSource> source;
    vtkSmartPointer<vtkActor> sphere;
    vtkSmartPointer<vtkBillboardTextActor3D> label;
};

// 拾取点相关
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
