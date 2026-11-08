#ifndef STRAINCALIBRATION_H
#define STRAINCALIBRATION_H

#pragma once
#include "config/CoreParameters.h"

#include <vector>
#include <array>
#include <QObject>
#include <QString>

namespace fosd {
namespace Calculation {
using Strain = std::vector<double>;     // 节点自由度数组
using Wavelength = std::vector<double>; // 波长数组

class StrainCalibration final : public QObject {
    Q_OBJECT
public:
    explicit StrainCalibration(QObject *parent = nullptr);
    ~StrainCalibration() = default;

    void initializeCalibration();
    // 设置映射、比例及偏置；偏置为空时按零处理，否则长度须与映射一致。
    // 参数在 checkPara() 和计算入口校验，不接受非有限偏置。
    void setCalibrationChannelData(const CalibrationChannelData &channelData);
    void setCalibrationStrainData(const CalibrationStrainData &strainData);
    CalibrationChannelData getCalibrationChannelData() const;
    CalibrationStrainData getCalibrationStrainData() const;
    QString lastError() const {
        return m_lastError;
    }

    bool checkPara();

    Strain applyCalibration(const Wavelength &inputWavelength);
    Strain rearrangeCalibration(const Strain &calibratedStrain);

signals:
    void errorOccurred(const QString &errorMessage);

private:
    bool fail(const QString &message);
    QString m_lastError;
    CalibrationChannelData m_calibrationChannelData; // 校准通道数据
    CalibrationStrainData m_calibrationStrainData;   // 校准应变数据
};

} // namespace Calculation
} // namespace fosd

#endif

Q_DECLARE_METATYPE(fosd::Calculation::CalibrationStrainData)
Q_DECLARE_METATYPE(fosd::Calculation::CalibrationChannelData)
