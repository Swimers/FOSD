#include "StrainCalibration.h"

namespace fosd{
namespace Calculation{

StrainCalibration::StrainCalibration(QObject *parent = nullptr)
    : QObject(parent)
{
    initializeCalibration();
}

void StrainCalibration::initializeCalibration()
{
    m_calibrationChannelData.channelIndices.clear();
    m_calibrationChannelData.calibrationFactors.clear();

    m_calibrationStrainData.originalWavelength.clear();
    m_calibrationStrainData.calibrationFactor = 0.00075;
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
    if (m_calibrationChannelData.channelIndices.size() != m_calibrationChannelData.calibrationFactors.size()){
        emit errorOccurred(QStringLiteral("Channel indices and calibration factors size mismatch."));
        return false;
    }
    if (m_calibrationStrainData.A == 0.0){
        emit errorOccurred(QStringLiteral("Calibration parameter A cannot be zero."));
        return false;
    }
        
    for (const auto& wavelength : m_calibrationStrainData.originalWavelength)
    {
        if (wavelength <= 0.0){
            emit errorOccurred(QStringLiteral("Original wavelength values must be positive."));
            return false;
        }
    }
    for (const auto& factor : m_calibrationChannelData.calibrationFactors)
    {
        if (factor <= 0.0){
            emit errorOccurred(QStringLiteral("Calibration factors must be positive."));
            return false;
        }
    }

    return true;
}

Strain StrainCalibration::applyCalibration(const Wavelength& inputWavelength)
{
    if (!checkPara())
        return {};

    Strain calibratedStrain(inputWavelength.size());
    for (size_t i = 0 ; i < inputWavelength.size(); ++i)
    {
        calibratedStrain[i]   =(m_calibrationStrainData.calibrationFactor
                                *(inputWavelength[i]-m_calibrationStrainData.originalWavelength[i]))
                                /m_calibrationStrainData.A
                                +m_calibrationStrainData.B;   //计算得到应变
    }
    return calibratedStrain;
}

Strain StrainCalibration::rearrangeCalibration(const Strain& calibratedStrain)
{
    if (!checkPara())
        return {};

    Strain rearrangedStrain(calibratedStrain.size());
    for (size_t i = 0; i < calibratedStrain.size(); ++i)
    {
        rearrangedStrain[i] = calibratedStrain[m_calibrationChannelData.channelIndices[i]] 
                                * m_calibrationChannelData.calibrationFactors[i];  //根据通道索引和校准因子重新排列应变
    }
    return rearrangedStrain;
}

}
}