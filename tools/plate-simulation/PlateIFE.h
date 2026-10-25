#pragma once
#include "PlateSimulation.h"
namespace fosd {
namespace simulation {
// Reduced-order inverse strain fit for the demo plate. Outputs metres as
// required by IFrameIFE; CalculationWorker converts displacement to mm.
class PlateIFE final : public Calculation::IFrameIFE {
public:
    PlateIFE();
    QString IFEId() const override;
    Calculation::NodeDofs calculateDofs(const Calculation::Strain &strain) const override;
    Calculation::Displacement
    calculateDisplacement(const Calculation::Point3D &point,
                          const Calculation::NodeDofs &dofs) const override;

private:
    std::array<std::array<double, PlateSpec::sensors>, 3> m_inverse{};
};
std::unique_ptr<Calculation::IFrameIFE> createPlateIFE();
} // namespace simulation
} // namespace fosd
