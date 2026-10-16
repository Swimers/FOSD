#ifndef HYYIFE_H
#define HYYIFE_H

#pragma once

#include "IFrameIFE.h"

namespace fosd{
namespace Calculation{

class HYYIFE final : public IFrameIFE
{
public:
    // Every matrix row maps to one global DOF. All other DOFs retain the
    // supplied constraint values; there is no guessed default boundary model.
    explicit HYYIFE(std::vector<int> freeDofIndices = {}, NodeDofs constrainedDofs = {});

    QString IFEId() const override;
    NodeDofs calculateDofs(const Strain& strain, QString *error = nullptr) const override;
    Displacement calculateDisplacement(const Point3D& point, const NodeDofs& nodeDofs,
                                      QString *error = nullptr) const override;

private:
    bool initMatrix() override;
    std::vector<int> m_freeDofIndices;
    NodeDofs m_constrainedDofs;

};

}
} // namespace fosd::Calculation

#endif
