#pragma once
#include "calculation/CalculationControl.h"
#include "calculation/PlateIFE.h"
#include <QByteArray>
#include <QString>
#include <array>
namespace fosd {
namespace simulation {
using PlateSpec = Calculation::PlateSpec;
struct PlateMotion {
    double amplitudeMm = 15.0;
    double frequencyHz = 0.35;
};
using PlateModes = std::array<double, 3>; // Modal transverse deflections, mm.
double sensorEta(int index);
double referenceWavelength(int index);
PlateModes modesAt(double seconds, const PlateMotion &motion);
Calculation::Strain sensorStrains(const PlateModes &modes); // microstrain
Calculation::CalculationConfiguration defaultCalibration();
QByteArray wavelengthPacket(quint32 sequence, double seconds, const PlateMotion &motion);
QString defaultModelPath();
} // namespace simulation
} // namespace fosd
