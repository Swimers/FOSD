#include "PlateSimulation.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QtEndian>
#include <cmath>
#include <cstring>
namespace fosd {
namespace simulation {
double sensorEta(int index) {
    return Calculation::plateSensorEta(index);
}

double referenceWavelength(int index) {
    return 1550.0 + 0.5 * index;
}

PlateModes modesAt(double seconds, const PlateMotion &motion) {
    const double phase = 6.2831853071795864769 * motion.frequencyHz * seconds;
    return {{motion.amplitudeMm * std::sin(phase),
             0.35 * motion.amplitudeMm * std::sin(1.3 * phase),
             0.2 * motion.amplitudeMm * std::sin(0.6 * phase)}};
}

Calculation::Strain sensorStrains(const PlateModes &modes) {
    Calculation::Strain strains(PlateSpec::sensors);
    // epsilon_xx = -z * d²w/dx²; z=t/2; convert to microstrain.
    const double coefficient =
        -PlateSpec::thicknessMm * 1e6 / (PlateSpec::lengthMm * PlateSpec::lengthMm);
    for (int i = 0; i < PlateSpec::sensors; ++i) {
        const double eta = sensorEta(i);
        strains[size_t(i)] = coefficient * (modes[0] + modes[1] * eta + modes[2] * eta * eta);
    }
    return strains;
}

Calculation::CalculationConfiguration defaultCalibration() {
    Calculation::CalculationConfiguration config;
    config.algorithm = Calculation::CalculationAlgorithm::Plate;
    // Existing pipeline: strain = factor * (lambda-lambda0) / A + B.
    config.strainCalibration.calibrationFactor = 1250.0; // microstrain/nm
    config.strainCalibration.A = 1.0;
    config.strainCalibration.B = 0.0;
    for (int i = 0; i < PlateSpec::sensors; ++i) {
        config.strainCalibration.originalWavelength.push_back(referenceWavelength(i));
        config.channelCalibration.channelIndices.push_back(i);
        config.channelCalibration.calibrationFactors.push_back(1.0);
    }
    return config;
}

QByteArray wavelengthPacket(quint32 sequence, double seconds, const PlateMotion &motion) {
    const auto strains = sensorStrains(modesAt(seconds, motion));
    QByteArray bytes(10, '\0');
    bytes[0] = char(0x5a);
    bytes[1] = char(0xa5);
    bytes[2] = 1;
    bytes[3] = 1;
    qToLittleEndian<quint16>(PlateSpec::sensors, reinterpret_cast<uchar *>(bytes.data() + 4));
    qToLittleEndian(sequence, reinterpret_cast<uchar *>(bytes.data() + 6));
    for (int i = 0; i < PlateSpec::sensors; ++i) {
        bytes.append(char(i + 1));
        bytes.append(char(1));
        const float wavelength = float(referenceWavelength(i) + strains[size_t(i)] / 1250.0);
        quint32 bits;
        std::memcpy(&bits, &wavelength, sizeof(bits));
        char encoded[4];
        qToLittleEndian(bits, reinterpret_cast<uchar *>(encoded));
        bytes.append(encoded, 4);
    }
    return bytes;
}

QString defaultModelPath() {
    const QString deployed = QDir(QCoreApplication::applicationDirPath())
                                 .filePath("tools/plate-simulation/models/plate.stl");
    return QFileInfo::exists(deployed) ? deployed : QString::fromUtf8(FOSD_PLATE_MODEL_FILE);
}
} // namespace simulation
} // namespace fosd
