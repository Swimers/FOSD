#include "HYYIFE.h"

namespace fosd {
namespace Calculation {
namespace {
// Container adaptation is private to this implementation. It imposes no
// dimension or constraint configuration on the caller and never reports errors.
double component(const std::vector<double> &values, size_t index) {
    return index < values.size() ? values[index] : 0.0;
}
}

HYYIFE::HYYIFE() {
    initMatrix();
}

QString HYYIFE::IFEId() const {
    return QStringLiteral("HYY.IFE");
}

void HYYIFE::initMatrix() {
    m_matrix = {
        {0.2,0.0016321,9.97E-17,0.2,0.0016321,9.97E-17,8.11E-17,0.0011674,6.03E-16,8.11E-17,0.0011674,6.0321e-16},
        {1.5776e-17,-0.060485,2.00E-01,1.58E-17,-0.060485,2.00E-01,2.85E-17,-0.043262,6.69E-16,2.85E-17,-0.043262,6.692e-16},
        {-4.0,-0.33136,3.4265,4.00E+00,0.33136,-3.4265,1.97E-15,-0.29246,3.24E-01,-1.97E-15,0.29246,-0.32442},
        {-5.6374e-15,6.0502,-19.968,5.64E-15,-6.0502,19.968,-1.25E-14,4.3276,0.034584,1.25E-14,-4.3276,-0.034584},
        {20.0,0.16159,-0.024418,-2.00E+01,-0.16159,0.024418,1.39E-14,0.11536,-0.033466,-1.39E-14,-0.11536,0.033466},
        {0.2,-0.0035744,1.82E-16,0.2,-0.0035744,1.82E-16,5.00E-01,0.0053877,8.84E-16,5.00E-01,0.0053877,8.8446e-16},
        {1.3706e-16,0.0026531,2.00E-01,1.37E-16,0.0026531,2.00E-01,2.43E-16,-0.08131,5.00E-01,2.43E-16,-0.08131,0.5},
        {-24.0,0.001009,3.8254,2.40E+01,-0.001009,-3.8254,-2.50E+01,-0.42002,7.107,2.50E+01,0.42002,-7.107},
        {-3.8596e-14,-0.25636,-19.839,3.86E-14,0.25636,19.839,-7.80E-14,8.1434,-49.744,7.80E-14,-8.1434,49.744},
        {20.0,-0.35842,-0.023752,-2.00E+01,0.35842,0.023752,5.00E+01,0.53631,-0.044148,-5.00E+01,-0.53631,0.044148},
        {0.2,0.002578,-4.76E-17,0.2,0.002578,-4.76E-17,0.5,-0.0022256,-1.77E-16,0.5,-0.0022256,-1.77e-16},
        {1.377e-16,-0.038278,2.00E-01,1.38E-16,-0.038278,2.00E-01,2.44E-16,0.051868,5.00E-01,2.44E-16,0.051868,0.5},
        {-24.0,0.18354,-0.37795,2.40E+01,-0.18354,0.37795,-2.50E+01,-0.011775,-3.1847,2.50E+01,0.011775,3.1847},
        {-3.8988e-14,3.8364,-19.839,3.90E-14,-3.8364,19.839,-7.87E-14,-5.1742,-49.744,7.87E-14,5.1742,49.744},
        {20.0,0.25756,0.00099168,-2.00E+01,-0.25756,-0.00099168,5.00E+01,-0.22194,0.0072987,-5.00E+01,0.22194,-0.0072987},
        {0.2,-0.0014661,-8.96E-17,0.2,-0.0014661,-8.96E-17,2.84E-17,-0.0010486,-5.42E-16,2.84E-17,-0.0010486,-5.4183e-16},
        {1.5427e-17,0.057483,2.00E-01,1.54E-17,0.057483,2.00E-01,2.80E-17,0.041115,6.65E-16,2.80E-17,0.041115,6.652e-16},
        {-4.0,-0.33831,-3.0708,4.00E+00,0.33831,3.0708,-3.03E-15,-0.19218,-2.91E-01,3.03E-15,0.19218,0.29097},
        {-5.006e-15,-5.7463,-19.967,5.01E-15,5.7463,19.967,-1.17E-14,-4.11,0.034835,1.17E-14,4.11,-0.034835},
        {20.0,-0.14514,0.021933,-2.00E+01,0.14514,-0.021933,-6.63E-15,-0.10362,0.030061,6.63E-15,0.10362,-0.030061}
    };
}

NodeDofs HYYIFE::calculateDofs(const Strain &strain) const {
    NodeDofs dofs(m_matrix.size(), 0.0);
    for (size_t row = 0; row < m_matrix.size(); ++row)
        for (size_t column = 0; column < m_matrix[row].size(); ++column)
            dofs[row] += m_matrix[row][column] * component(strain, column);
    return dofs;
}

Displacement HYYIFE::calculateDisplacement(const Point3D& point, const NodeDofs& nodeDofs) const
{
    // The real legacy display path calls ShapeFunction_New_ZB, not the
    // similarly named, unused quadrilateral ShapeFunction_New_HYY.
    // Match its two beam elements and its metres/radians convention.
    if (point.x < 0.0 || point.x > 1400.0)
        return {{0.0, 0.0, 0.0}};
    const double xPara = point.x / 1000.0;
    const double yPara = point.y / 1000.0;
    const double zPara = point.z / 1000.0;
    const double elementLength = 0.7;
    const bool firstElement = xPara <= elementLength;
    const int left = firstElement ? 0 : 8;
    const int middle = firstElement ? 6 : 14;
    const int right = firstElement ? 8 : 16;
    const double leftX = firstElement ? 0.0 : elementLength;
    const double x = 2.0 * (xPara - leftX) / elementLength - 1.0;
    const double xSquared = x * x;
    const double leftWeight = 0.5 * (1.0 - x);
    const double rightWeight = 0.5 * (1.0 + x);
    const double quadraticLeft = 0.5 * x * (x - 1.0);
    const double quadraticMiddle = 1.0 - xSquared;
    const double quadraticRight = 0.5 * x * (x + 1.0);
    const double cubicFactor = elementLength / 24.0 * (1.0 - xSquared);
    const double cubicLeft = cubicFactor * (2.0 * x - 3.0);
    const double cubicMiddle = cubicFactor * (-4.0 * x);
    const double cubicRight = cubicFactor * (2.0 * x + 3.0);

    const double u = leftWeight * component(nodeDofs, left) + rightWeight * component(nodeDofs, right);
    const double v = leftWeight * component(nodeDofs, left + 1) - cubicLeft * component(nodeDofs, left + 5)
        - cubicMiddle * component(nodeDofs, middle + 1) + rightWeight * component(nodeDofs, right + 1)
        - cubicRight * component(nodeDofs, right + 5);
    const double w = leftWeight * component(nodeDofs, left + 2) + cubicLeft * component(nodeDofs, left + 4)
        + cubicMiddle * component(nodeDofs, middle) + rightWeight * component(nodeDofs, right + 2)
        + cubicRight * component(nodeDofs, right + 4);
    const double thetaX = leftWeight * component(nodeDofs, left + 3) + rightWeight * component(nodeDofs, right + 3);
    const double thetaY = quadraticLeft * component(nodeDofs, left + 4) + quadraticMiddle * component(nodeDofs, middle)
        + quadraticRight * component(nodeDofs, right + 4);
    const double thetaZ = quadraticLeft * component(nodeDofs, left + 5) + quadraticMiddle * component(nodeDofs, middle + 1)
        + quadraticRight * component(nodeDofs, right + 5);
    return {{u + zPara * thetaY - yPara * thetaZ, v - zPara * thetaX, w + yPara * thetaX}};
}

} // namespace Calculation
} // namespace fosd
