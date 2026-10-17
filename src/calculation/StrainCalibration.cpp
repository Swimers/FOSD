#include "StrainCalibration.h"
#include <cmath>

namespace fosd{
namespace Calculation{

StrainCalibration::StrainCalibration(QObject *parent)
    : QObject(parent)
{
    initializeCalibration();
}

void StrainCalibration::initializeCalibration()
{
    m_calibrationChannelData = CalibrationChannelData();
    m_calibrationStrainData = CalibrationStrainData();
    m_lastError.clear();
}

void StrainCalibration::setCalibrationChannelData(const CalibrationChannelData& channelData)
{
    m_calibrationChannelData = channelData;
}

void StrainCalibration::setCalibrationStrainData(const CalibrationStrainData& strainData)
{
    m_calibrationStrainData = strainData;
}

CalibrationChannelData StrainCalibration::getCalibrationChannelData() const
{
    return m_calibrationChannelData;
}

CalibrationStrainData StrainCalibration::getCalibrationStrainData() const
{
    return m_calibrationStrainData;
}

bool StrainCalibration::checkPara()
{
    m_lastError.clear();
    const auto &parameters = m_calibrationStrainData;
    const auto &channels = m_calibrationChannelData;
    if (parameters.originalWavelength.empty() || channels.channelIndices.empty())
        return fail(QStringLiteral("Wavelength references and channel mapping must not be empty"));
    if (channels.channelIndices.size() != channels.calibrationFactors.size())
        return fail(QStringLiteral("Channel indices and calibration factors size mismatch"));
    if (!std::isfinite(parameters.A) || parameters.A == 0.0 || !std::isfinite(parameters.B) ||
        !std::isfinite(parameters.calibrationFactor) || parameters.calibrationFactor <= 0.0)
        return fail(QStringLiteral("Calibration requires finite A/B, nonzero A and a positive finite factor"));
    for (double value : parameters.originalWavelength)
        if (!std::isfinite(value) || value <= 0.0)
            return fail(QStringLiteral("Original wavelengths must be positive and finite"));
    for (size_t index = 0; index < channels.channelIndices.size(); ++index) {
        const int source = channels.channelIndices[index];
        if (source < 0 || static_cast<size_t>(source) >= parameters.originalWavelength.size())
            return fail(QStringLiteral("Channel index is outside the wavelength array"));
        const double factor = channels.calibrationFactors[index];
        if (!std::isfinite(factor) || factor <= 0.0)
            return fail(QStringLiteral("Channel calibration factors must be positive and finite"));
    }
    return true;
}

bool StrainCalibration::fail(const QString &message) {
    m_lastError = message;
    emit errorOccurred(message);
    return false;
}

Strain StrainCalibration::applyCalibration(const Wavelength& inputWavelength)
{
    if (!checkPara())
        return {};

    if (inputWavelength.size() != m_calibrationStrainData.originalWavelength.size()) {
        fail(QStringLiteral("Input and reference wavelength counts must match"));
        return {};
    }
    Strain calibratedStrain(inputWavelength.size());
    for (size_t i = 0 ; i < inputWavelength.size(); ++i)
    {
        if (!std::isfinite(inputWavelength[i]) || inputWavelength[i] <= 0.0) {
            fail(QStringLiteral("Input wavelengths must be positive and finite"));
            return {};
        }
        calibratedStrain[i]   =(m_calibrationStrainData.calibrationFactor
                                *(inputWavelength[i]-m_calibrationStrainData.originalWavelength[i]))
                                /m_calibrationStrainData.A
                                +m_calibrationStrainData.B;   //计算得到应变
        if (!std::isfinite(calibratedStrain[i])) {
            fail(QStringLiteral("Wavelength calibration overflowed"));
            return {};
        }
    }
    return calibratedStrain;
}

Strain StrainCalibration::rearrangeCalibration(const Strain& calibratedStrain)
{
    if (!checkPara())
        return {};

    if (calibratedStrain.size() != m_calibrationStrainData.originalWavelength.size()) {
        fail(QStringLiteral("Strain and reference wavelength counts must match"));
        return {};
    }
    for (double value : calibratedStrain)
        if (!std::isfinite(value)) {
            fail(QStringLiteral("Strain values must be finite"));
            return {};
        }
    Strain rearrangedStrain(m_calibrationChannelData.channelIndices.size());
    for (size_t i = 0; i < rearrangedStrain.size(); ++i)
    {
        rearrangedStrain[i] = calibratedStrain[m_calibrationChannelData.channelIndices[i]] 
                                * m_calibrationChannelData.calibrationFactors[i];  //根据通道索引和校准因子重新排列应变
        if (!std::isfinite(rearrangedStrain[i])) {
            fail(QStringLiteral("Channel calibration overflowed"));
            return {};
        }
    }
    return rearrangedStrain;
}

}
}
