#include "PlateIFE.h"
#include <cmath>
namespace fosd {
namespace Calculation {
constexpr double PlateSpec::lengthMm;
constexpr double PlateSpec::widthMm;
constexpr double PlateSpec::thicknessMm;
constexpr int PlateSpec::sensors;
double plateSensorEta(int index) {
    return -0.9 + 1.8 * index / (PlateSpec::sensors - 1);
}

PlateIFE::PlateIFE() {
    // Solve the fixed 3x3 normal matrix once; no matrix operations per point.
    double basis[PlateSpec::sensors][3];
    double augmented[3][6] = {};
    const double k = -PlateSpec::thicknessMm * 1e6 / (PlateSpec::lengthMm * PlateSpec::lengthMm);
    for (int i = 0; i < PlateSpec::sensors; ++i) {
        const double eta = plateSensorEta(i);
        basis[i][0] = k;
        basis[i][1] = k * eta;
        basis[i][2] = k * eta * eta;
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                augmented[r][c] += basis[i][r] * basis[i][c];
    }
    for (int i = 0; i < 3; ++i)
        augmented[i][i + 3] = 1.0;
    for (int pivot = 0; pivot < 3; ++pivot) {
        const double diagonal = augmented[pivot][pivot];
        for (int c = 0; c < 6; ++c)
            augmented[pivot][c] /= diagonal;
        for (int r = 0; r < 3; ++r) {
            if (r == pivot)
                continue;
            const double factor = augmented[r][pivot];
            for (int c = 0; c < 6; ++c)
                augmented[r][c] -= factor * augmented[pivot][c];
        }
    }
    for (int mode = 0; mode < 3; ++mode)
        for (int sensor = 0; sensor < PlateSpec::sensors; ++sensor)
            for (int c = 0; c < 3; ++c)
                m_inverse[size_t(mode)][size_t(sensor)] +=
                    augmented[mode][c + 3] * basis[sensor][c];
}

QString PlateIFE::IFEId() const {
    return QStringLiteral("demo.plate.modal-ife");
}

Calculation::NodeDofs PlateIFE::calculateDofs(const Calculation::Strain &strain) const {
    Calculation::NodeDofs dofs(3, 0.0);
    if (strain.size() != PlateSpec::sensors)
        return dofs;
    for (int mode = 0; mode < 3; ++mode)
        for (int i = 0; i < PlateSpec::sensors; ++i)
            dofs[size_t(mode)] += m_inverse[size_t(mode)][size_t(i)] * strain[size_t(i)];
    return dofs;
}

Calculation::Displacement PlateIFE::calculateDisplacement(const Calculation::Point3D &p,
                                                          const Calculation::NodeDofs &dofs) const {
    if (dofs.size() != 3)
        return {{0, 0, 0}};
    const double xi = p.x / PlateSpec::lengthMm, eta = 2.0 * p.y / PlateSpec::widthMm;
    const double polynomial = dofs[0] + dofs[1] * eta + dofs[2] * eta * eta;
    const double w = xi * xi * polynomial;
    const double wx = 2.0 * xi / PlateSpec::lengthMm * polynomial;
    const double wy = 2.0 * xi * xi / PlateSpec::widthMm * (dofs[1] + 2.0 * dofs[2] * eta);
    return {{-p.z * wx / 1000.0, -p.z * wy / 1000.0, w / 1000.0}};
}

std::unique_ptr<Calculation::IFrameIFE> createPlateIFE() {
    return std::unique_ptr<Calculation::IFrameIFE>(new PlateIFE);
}
} // namespace Calculation
} // namespace fosd
