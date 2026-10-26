#pragma once
#include "IFrameIFE.h"
#include <memory>
namespace fosd {
namespace Calculation {
struct PlateSpec {
    static constexpr double lengthMm = 1000.0;
    static constexpr double widthMm = 400.0;
    static constexpr double thicknessMm = 4.0;
    static constexpr int sensors = 12;
};
double plateSensorEta(int index);
// Validation plate algorithm; input microstrain, output displacement in metres.
class PlateIFE final : public IFrameIFE {
public:
    PlateIFE();
    QString IFEId() const override;
    NodeDofs calculateDofs(const Strain &strain) const override;
    Displacement calculateDisplacement(const Point3D &point, const NodeDofs &dofs) const override;

private:
    std::array<std::array<double, PlateSpec::sensors>, 3> m_inverse{};
};
std::unique_ptr<IFrameIFE> createPlateIFE();
} // namespace Calculation
} // namespace fosd
