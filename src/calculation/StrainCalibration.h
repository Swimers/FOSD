#ifndef STRAINCALIBRATION_H
#define STRAINCALIBRATION_H

#pragma once

#include <vector>
#include <array>
#include <QObject>

namespace fosd{
namespace Calculation{

using Strain = std::vector<double>;   // 节点自由度数组
using Wavelength =std::vector<double>; // 波长数组

struct CalibrationStrainData
{
    std::vector<float> originalWavelength = {0.0};  // 原始波长
    float calibrationFactor = 0.00075;  // 校准因子
    float A = 0.0;  // 校准参数A
    float B = 0.0;  // 校准参数B
};
struct CalibrationChannelData
{
    std::vector<int> channelIndices;  // 校准通道索引数组
    std::vector<double> calibrationFactors;  // 校准因子数组
};

class StrainCalibration final : public QObject
{
    Q_OBJECT
public:
    explicit StrainCalibration(QObject *parent = nullptr);
    ~StrainCalibration();

    void initializeCalibration();
    void setCalibrationChannelData(const CalibrationChannelData& channelData);
    void setCalibrationStrainData(const CalibrationStrainData& strainData);
    CalibrationChannelData getCalibrationChannelData();
    CalibrationStrainData getCalibrationStrainData();

    bool checkPara();

    Strain applyCalibration(const Wavelength& inputWavelength);
    Strain rearrangeCalibration(const Strain& calibratedStrain);

signals:
    void errorOccurred(const QString &errorMessage);

private:
    CalibrationChannelData m_calibrationChannelData;  // 校准通道数据
    CalibrationStrainData m_calibrationStrainData;  // 校准应变数据
};

}
} // namespace fosd::Calculation

#endif