#pragma once

#include <vector>
#include <array>
#include <QString>

namespace fosd { 
namespace Calculation {

using Matrix = std::vector<std::vector<double>>;    //通用动态矩阵
using Strain = std::vector<double>;   // 应变数组
using NodeDofs = std::vector<double>;   // 节点自由度数组
using Displacement = std::array<double, 3>; // 三维位移

struct Point3D
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};      // 待插值点坐标

class IFrameIFE{
public:
    IFrameIFE() = default;
    virtual ~IFrameIFE() = default;

    virtual QString IFEId() const = 0;

    // Intermediate data is opaque to callers. Matrix setup and numerical
    // details belong to each algorithm, not the orchestration interface.
    virtual NodeDofs calculateDofs(const Strain& strain) const = 0;
    virtual Displacement calculateDisplacement(const Point3D& point, const NodeDofs& nodeDofs) const = 0;

};

} 
} // namespace fosd::Calculation
