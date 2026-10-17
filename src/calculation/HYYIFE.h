#ifndef HYYIFE_H
#define HYYIFE_H

#pragma once

#include "IFrameIFE.h"

namespace fosd{
namespace Calculation{

class HYYIFE final : public IFrameIFE
{
public:
    HYYIFE();

    QString IFEId() const override;
    NodeDofs calculateDofs(const Strain& strain) const override;
    Displacement calculateDisplacement(const Point3D& point, const NodeDofs& nodeDofs) const override;

private:
    void initMatrix();
    Matrix m_matrix;

};

}
} // namespace fosd::Calculation

#endif
