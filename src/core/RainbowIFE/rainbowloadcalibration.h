#ifndef RAINBOWLOADCALIBRATION_H
#define RAINBOWLOADCALIBRATION_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <vector>
#include <algorithm>

// 先定义必要的宏（根据原代码推测，需与你的实际定义一致）
#define CALIB_SENSORS_MAX 128   // 最大传感器数量
#define CALIB_MAX 4096         // 每个传感器最大标定点数

class rainbowloadcalibration : public QObject
{
    Q_OBJECT
public:
    explicit  rainbowloadcalibration(QObject *parent = nullptr);

private:
    // ========== 成员变量声明（根据原代码整理） ==========
    QString m_packetLogFile;                      // 数据包日志文件路径
    float original_wavelength[CALIB_SENSORS_MAX]; // 初始波长数组

    // S表（温度→波长）相关
    int s_calibration_sensor_count = 0;                          // S表传感器数量
    int s_calibration_size_per_sensor[CALIB_SENSORS_MAX] = {0};  // 每个传感器的标定点数
    float s_calibration_temperature_mat[CALIB_SENSORS_MAX][CALIB_MAX] = {0.0f}; // 温度矩阵
    float s_calibration_wavelength_mat[CALIB_SENSORS_MAX][CALIB_MAX] = {0.0f};  // 波长矩阵

    // T表（波长→温度）相关
    int t_calibration_sensor_count = 0;                          // T表传感器数量
    int t_calibration_size_per_sensor[CALIB_SENSORS_MAX] = {0};  // 每个传感器的标定点数
    float t_calibration_temperature_mat[CALIB_SENSORS_MAX][CALIB_MAX] = {0.0f}; // 温度矩阵
    float t_calibration_wavelength_mat[CALIB_SENSORS_MAX][CALIB_MAX] = {0.0f};  // 波长矩阵

    // 通用插值接口数组
    int calibration_size_arr = 0;
    float calibration_temperature_arr[CALIB_MAX] = {0.0f};
    float calibration_wavelength_arr[CALIB_MAX] = {0.0f};
    QVector<float> calibration_temperature;  // 单列模式温度列表
    QVector<float> calibration_wavelength;   // 单列模式波长列表

    // ========== 抽离的成员函数声明 ==========
    /**
     * @brief 初始化日志文件（packet_log.csv）
     */
    void initLogFile();

    /**
     * @brief 加载初始波长文件（./初始波长.txt）
     */
    void loadOriginalWavelength();

    /**
     * @brief 加载所有标定CSV文件（S表+T表）
     *        优先矩阵模式，失败则回退单列模式
     */
    void loadAllCalibrationData();

    /**
     * @brief 加载矩阵格式的S表（温度→波长）
     * @param path CSV文件路径
     * @return 是否加载成功
     */
    bool loadCsvMatrixSensors_S(const QString& path);

    /**
     * @brief 加载矩阵格式的T表（波长→温度）
     * @param path CSV文件路径
     * @return 是否加载成功
     */
    bool loadCsvMatrixSensors_T(const QString& path);

    /**
     * @brief 加载单列格式的CSV文件（兼容模式）
     * @param path 文件路径
     * @param out 输出的数值列表
     * @return 是否加载成功
     */
    bool loadCsvSingleColumn(const QString& path, QVector<float>& out);

    /**
     * @brief 打印加载后的标定数据（调试用）
     */
    void printCalibrationData();
};

#endif // RAINBOWLOADCALIBRATION_H
