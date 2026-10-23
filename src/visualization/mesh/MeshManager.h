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
    bool loadModel(const QString &path, QString *error = nullptr);
    bool optimize(QString *error = nullptr);
    bool exportModel(const QString &path, QString *error = nullptr) const;
    void clear();
    
    core::ModelDataPtr originalModel() const;
    const vtkPolyData *originalMesh() const; // Borrowed, immutable original mesh.
private:
    vtkSmartPointer<vtkPolyData> m_originalMesh;
    core::ModelDataPtr m_originalModel;
};

}
}
