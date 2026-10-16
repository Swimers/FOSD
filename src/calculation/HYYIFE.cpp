#include "HYYIFE.h"
#include <cmath>
#include <utility>

namespace fosd{
namespace Calculation{

HYYIFE::HYYIFE(std::vector<int> freeDofIndices, NodeDofs constrainedDofs)
    : m_freeDofIndices(std::move(freeDofIndices)), m_constrainedDofs(std::move(constrainedDofs))
{
    initMatrix();
}

QString HYYIFE::IFEId() const
{
    return QStringLiteral("HYY.IFE");
}

bool HYYIFE::initMatrix()
{
    // Initialize the matrix with specific values for HYY IFE
    // Please input the nodedofs size first
    m_nodeDofs = 22;
    // Please input the matrix size first
    int rows = 20;  int cols = 12;
    // Initialize the matrix 
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
    m_numRows = m_matrix.size();
    m_numCols = m_matrix.empty() ? 0 : m_matrix[0].size();

    if (rows != m_numRows || cols != m_numCols) {
        return false; // Invalid matrix
    }
    for (const auto &row : m_matrix)
        if (row.size() != static_cast<size_t>(cols))
            return false;

    return true;
}

NodeDofs HYYIFE::calculateDofs(const Strain& strain, QString *error) const
{
    if (error)
        error->clear();
    QString message;
    if (m_matrix.size() != 20 || strain.size() != 12)
        message = QStringLiteral("HYY requires exactly 12 strain values and a 20x12 matrix");
    else if (m_freeDofIndices.size() != 20 || m_constrainedDofs.size() != 22)
        message = QStringLiteral("Configure 20 matrix-row DOF indices and 22 constraint values");
    std::vector<bool> used(22, false);
    if (message.isEmpty()) {
        for (int index : m_freeDofIndices) {
            if (index < 0 || index >= 22 || used[index]) {
                message = QStringLiteral("Global DOF indices must be unique and in 0..21");
                break;
            }
            used[index] = true;
        }
    }
    if (message.isEmpty()) {
        for (double value : strain)
            if (!std::isfinite(value))
                message = QStringLiteral("Strain values must be finite");
        for (double value : m_constrainedDofs)
            if (!std::isfinite(value))
                message = QStringLiteral("Constraint values must be finite");
    }
    if (!message.isEmpty()) {
        if (error)
            *error = message;
        return {};
    }
    NodeDofs dofs = m_constrainedDofs;
    for (size_t row = 0; row < m_matrix.size(); ++row) {
        if (m_matrix[row].size() != strain.size()) {
            if (error)
                *error = QStringLiteral("Invalid HYY matrix row length");
            return {};
        }
        double value = 0.0;
        for (size_t column = 0; column < strain.size(); ++column)
            value += m_matrix[row][column] * strain[column];
        if (!std::isfinite(value)) {
            if (error)
                *error = QStringLiteral("HYY matrix multiplication overflowed");
            return {};
        }
        dofs[m_freeDofIndices[row]] = value;
    }
    return dofs;
}

Displacement HYYIFE::calculateDisplacement(const Point3D& point, const NodeDofs& nodeDofs,
                                          QString *error) const
{
    Displacement displacement = {0.0, 0.0, 0.0};

    if (error)
        error->clear();
    bool finiteDofs = true;
    for (double value : nodeDofs)
        finiteDofs = finiteDofs && std::isfinite(value);
    if (nodeDofs.size() != 22 || !finiteDofs || !std::isfinite(point.x) ||
        !std::isfinite(point.y) || !std::isfinite(point.z)) {
        if (error)
            *error = QStringLiteral("HYY interpolation requires 22 finite DOFs and finite coordinates");
        return displacement;
    }

    const double xPara = point.x / 1000.0;
    const double yPara = point.y / 1000.0;
    const double zPara = point.z / 1000.0;

    const double totalLength = 1.4;
    const double elementLength = totalLength / 2.0;

    if (xPara < 0.0 || xPara > totalLength) {
        if (error)
            *error = QStringLiteral("HYY model X coordinate must be in 0..1400 mm");
        return displacement;
    }

    double localDofs[14] = {};
    double leftX = 0.0;

    if (xPara <= elementLength) {
        for (int i = 0; i < 14; ++i) {
            localDofs[i] = nodeDofs[i];
        }
    } else {
        leftX = elementLength;

        for (int i = 0; i < 6; ++i) {
            localDofs[i] = nodeDofs[i + 8];
        }

        localDofs[6] = nodeDofs[14];
        localDofs[7] = nodeDofs[15];

        for (int i = 0; i < 6; ++i) {
            localDofs[i + 8] = nodeDofs[i + 16];
        }
    }

    const double x =
        2.0 * (xPara - leftX) / elementLength - 1.0;

    const double linear[2] = {
        0.5 * (1.0 - x),
        0.5 * (1.0 + x)
    };

    const double quadratic[3] = {
        0.5 * x * (x - 1.0),
        1.0 - x * x,
        0.5 * x * (x + 1.0)
    };

    const double cubic[3] = {
        elementLength / 24.0
            * (1.0 - x * x) * (2.0 * x - 3.0),

        elementLength / 24.0
            * (1.0 - x * x) * (-4.0 * x),

        elementLength / 24.0
            * (1.0 - x * x) * (2.0 * x + 3.0)
    };

    const double u =
        linear[0] * localDofs[0]
        + linear[1] * localDofs[8];

    const double v =
        linear[0] * localDofs[1]
        - cubic[0] * localDofs[5]
        - cubic[1] * localDofs[7]
        + linear[1] * localDofs[9]
        - cubic[2] * localDofs[13];

    const double w =
        linear[0] * localDofs[2]
        + cubic[0] * localDofs[4]
        + cubic[1] * localDofs[6]
        + linear[1] * localDofs[10]
        + cubic[2] * localDofs[12];

    const double thetaX =
        linear[0] * localDofs[3]
        + linear[1] * localDofs[11];

    const double thetaY =
        quadratic[0] * localDofs[4]
        + quadratic[1] * localDofs[6]
        + quadratic[2] * localDofs[12];

    const double thetaZ =
        quadratic[0] * localDofs[5]
        + quadratic[1] * localDofs[7]
        + quadratic[2] * localDofs[13];

    displacement[0] =
        u + zPara * thetaY - yPara * thetaZ;

    displacement[1] =
        v - zPara * thetaX;

    displacement[2] =
        w + yPara * thetaX;

    for (double value : displacement) {
        if (!std::isfinite(value)) {
            if (error)
                *error = QStringLiteral("HYY interpolation overflowed");
            return {{0.0, 0.0, 0.0}};
        }
    }
    return displacement;
}

}
} // namespace fosd::Calculation
