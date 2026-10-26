#include "MeshManager.h"

#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkPoints.h>
#include <QFile>
#include <cmath>
namespace fosd {
namespace visualization {
MeshManager::MeshManager() = default;
MeshManager::~MeshManager() = default;

bool MeshManager::loadModel(const QString &path, QString *error) {
    // 读取STL模型
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(QFile::encodeName(path).constData());
    reader->Update();
    auto *mesh = reader->GetOutput();
    if (!mesh || !mesh->GetNumberOfPoints() || !mesh->GetNumberOfCells()) {
        if (error)
            *error = QStringLiteral("Cannot read a nonempty STL mesh: %1").arg(path);
        return false;
    }

    // 模型映射到 QMap
    core::ModelData points;
    for (vtkIdType id = 0; id < mesh->GetNumberOfPoints(); ++id) {
        double xyz[3];
        mesh->GetPoint(id, xyz);
        if (!std::isfinite(xyz[0]) || !std::isfinite(xyz[1]) || !std::isfinite(xyz[2])) {
            if (error)
                *error = QStringLiteral("Model contains nonfinite coordinates");
            return false;
        }
        points.insert(id, {xyz[0], xyz[1], xyz[2]});
    }

    // 保留原始模型 vtkPolyData/QMap
    m_originalMesh = vtkSmartPointer<vtkPolyData>::New();
    m_originalMesh->DeepCopy(mesh);
    m_originalModel = std::make_shared<const core::ModelData>(std::move(points));
    
    return true;
}

void MeshManager::clear() {
    m_originalMesh = nullptr;
    m_originalModel.reset();
}

core::ModelDataPtr MeshManager::originalModel() const {
    return m_originalModel;
}

qint64 MeshManager::polygonCount() const {
    return m_originalMesh ? qint64(m_originalMesh->GetNumberOfPolys()) : 0;
}

const vtkPolyData *MeshManager::originalMesh() const {
    return m_originalMesh;
}

} // namespace visualization
} // namespace fosd
