#pragma once
#include "core/Datamodel/ModelData.h"
#include <QString>
#include <vtkSmartPointer.h>
class vtkPolyData;
namespace fosd {
namespace visualization {
class MeshManager final {
public:
    MeshManager();
    ~MeshManager();
    
    bool loadModel(const QString &path, QString *error = nullptr);  // 加载模型
    bool optimize(QString *error = nullptr);    // 
    bool exportModel(const QString &path, QString *error = nullptr) const;  // 
    void clear();   // 

    core::ModelDataPtr originalModel() const;   // 
    qint64 polygonCount() const;
    const vtkPolyData *originalMesh() const; // Borrowed, immutable original mesh.
private:
    vtkSmartPointer<vtkPolyData> m_originalMesh;
    core::ModelDataPtr m_originalModel;
};

} // namespace visualization
} // namespace fosd
