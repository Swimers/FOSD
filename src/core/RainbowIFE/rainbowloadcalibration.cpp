#include "rainbowloadcalibration.h"

 rainbowloadcalibration:: rainbowloadcalibration(QObject *parent) : QObject(parent), m_packetLogFile("data/packet_log.csv")
{
    // 构造函数仅保留核心初始化逻辑：日志初始化 + 读取初始波长
    initLogFile();
    loadOriginalWavelength();

    // 标定数据加载、调试打印作为独立步骤调用（仍可在构造函数中触发，也可外部调用）
    loadAllCalibrationData();
    printCalibrationData();
}

/**
 * @brief 初始化日志文件（原代码中未实现，这里保留接口）
 */
void  rainbowloadcalibration::initLogFile()
{
    // 补充日志文件初始化逻辑（如创建文件、写入表头）
    QFile logFile(m_packetLogFile);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QTextStream ts(&logFile);
        ts << "时间,传感器ID,温度,波长\n"; // 示例表头
        logFile.close();
    }
}

/**
 * @brief 加载初始波长文件（原构造函数中这部分逻辑抽离）
 */
void  rainbowloadcalibration::loadOriginalWavelength()
{
    QFile file_txt("data/calibration/初始波长.txt");
    if (!file_txt.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "警告：无法打开初始波长.txt";
        return;
    }

    QTextStream TextRead(&file_txt);
    QString data_line;
    int wave_count = 0;
    while (!TextRead.atEnd() && wave_count < CALIB_SENSORS_MAX) { // 增加数组越界保护
        data_line = TextRead.readLine().trimmed();
        if (data_line.isEmpty()) continue;

        bool ok = false;
        float wave = data_line.toFloat(&ok);
        if (ok) {
            original_wavelength[wave_count] = wave;
            wave_count++;
        } else {
            qDebug() << "警告：初始波长.txt第" << wave_count+1 << "行数据无效：" << data_line;
        }
    }
    file_txt.close();

    qDebug() << "成功加载初始波长，共" << wave_count << "个波长值";
}

/**
 * @brief 加载矩阵格式的S表（温度→波长）
 */
bool  rainbowloadcalibration::loadCsvMatrixSensors_S(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开S表矩阵文件：" << path;
        return false;
    }

    QTextStream ts(&f);
    QVector<QVector<float>> per_sensor_w;  // 每个传感器一列（波长）
    QVector<QVector<float>> per_sensor_t;  // 每个传感器对应的温度列
    int max_cols = 0;

    while (!ts.atEnd()) {
        QString line = ts.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
        if (cols.size() < 2) continue; // 至少需要 温度 + 1个传感器

        // 解析温度列
        bool ok_t = false;
        float t = cols[0].toFloat(&ok_t);
        if (!ok_t) continue;

        max_cols = std::max(max_cols, cols.size());
        int sensors_cols = cols.size() - 1;

        // 确保容器大小足够
        if (per_sensor_w.size() < sensors_cols) {
            per_sensor_w.resize(sensors_cols);
            per_sensor_t.resize(sensors_cols);
        }

        // 解析每个传感器的波长
        for (int ci = 1; ci < cols.size(); ++ci) {
            bool ok_w = false;
            float w = cols[ci].toFloat(&ok_w);
            if (!ok_w) continue;

            int sidx = ci - 1;   // 传感器索引（从0开始）
            per_sensor_w[sidx].append(w);
            per_sensor_t[sidx].append(t);
        }
    }
    f.close();

    if (per_sensor_w.isEmpty()) {
        qDebug() << "S表矩阵文件无有效数据：" << path;
        return false;
    }

    // 填充S表数据（限制最大传感器数和点数）
    s_calibration_sensor_count = std::min(per_sensor_w.size(), (int)CALIB_SENSORS_MAX);
    for (int s = 0; s < s_calibration_sensor_count; ++s) {
        int n = std::min(per_sensor_w[s].size(), per_sensor_t[s].size());
        s_calibration_size_per_sensor[s] = std::min(n, CALIB_MAX);

        for (int k = 0; k < s_calibration_size_per_sensor[s]; ++k) {
            s_calibration_wavelength_mat[s][k] = per_sensor_w[s][k];
            s_calibration_temperature_mat[s][k] = per_sensor_t[s][k];
        }
    }

    // 同步通用插值接口数组（取第0号传感器）
    if (s_calibration_sensor_count > 0) {
        calibration_size_arr = s_calibration_size_per_sensor[0];
        for (int k = 0; k < calibration_size_arr; ++k) {
            calibration_temperature_arr[k] = s_calibration_temperature_mat[0][k];
            calibration_wavelength_arr[k]  = s_calibration_wavelength_mat[0][k];
        }
    }

    qDebug() << "成功加载S表矩阵：" << path << "，传感器数：" << s_calibration_sensor_count;
    return true;
}

/**
 * @brief 加载矩阵格式的T表（波长→温度）
 */
bool  rainbowloadcalibration::loadCsvMatrixSensors_T(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开T表矩阵文件：" << path;
        return false;
    }

    QTextStream ts(&f);
    QVector<QVector<float>> per_sensor_w;  // 每个传感器一列（波长）
    QVector<QVector<float>> per_sensor_t;  // 每个传感器对应的温度列

    while (!ts.atEnd()) {
        QString line = ts.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList cols = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
        if (cols.size() < 2) continue;

        // 解析温度列
        bool ok_t = false;
        float t = cols[0].toFloat(&ok_t);
        if (!ok_t) continue;

        int sensors_cols = cols.size() - 1;
        if (per_sensor_w.size() < sensors_cols) {
            per_sensor_w.resize(sensors_cols);
            per_sensor_t.resize(sensors_cols);
        }

        // 解析每个传感器的波长
        for (int ci = 1; ci < cols.size(); ++ci) {
            bool ok_w = false;
            float w = cols[ci].toFloat(&ok_w);
            if (!ok_w) continue;

            int sidx = ci - 1;
            per_sensor_w[sidx].append(w);
            per_sensor_t[sidx].append(t);
        }
    }
    f.close();

    if (per_sensor_w.isEmpty()) {
        qDebug() << "T表矩阵文件无有效数据：" << path;
        return false;
    }

    // 填充T表数据
    t_calibration_sensor_count = std::min(per_sensor_w.size(), (int)CALIB_SENSORS_MAX);
    for (int s = 0; s < t_calibration_sensor_count; ++s) {
        int n = std::min(per_sensor_w[s].size(), per_sensor_t[s].size());
        t_calibration_size_per_sensor[s] = std::min(n, CALIB_MAX);

        for (int k = 0; k < t_calibration_size_per_sensor[s]; ++k) {
            t_calibration_wavelength_mat[s][k] = per_sensor_w[s][k];
            t_calibration_temperature_mat[s][k] = per_sensor_t[s][k];
        }
    }

    qDebug() << "成功加载T表矩阵：" << path << "，传感器数：" << t_calibration_sensor_count;
    return true;
}

/**
 * @brief 加载单列格式的CSV文件（兼容模式）
 */
bool  rainbowloadcalibration::loadCsvSingleColumn(const QString& path, QVector<float>& out)
{
    out.clear(); // 清空输出容器

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开单列CSV文件：" << path;
        return false;
    }

    QTextStream ts(&f);
    int line_no = 0;
    while (!ts.atEnd()) {
        QString line = ts.readLine().trimmed();
        line_no++;
        if (line.isEmpty()) continue;

        QStringList cols = line.split(QRegExp(",|\\s+"), QString::SkipEmptyParts);
        if (cols.isEmpty()) continue;

        bool ok = false;
        float v = cols[0].toFloat(&ok);
        if (ok) {
            out.append(v);
        } else {
            qDebug() << "单列CSV第" << line_no << "行无效（跳过）：" << line;
        }
    }
    f.close();

    bool success = !out.isEmpty();
    if (success) {
        qDebug() << "成功加载单列CSV：" << path << "，有效数据数：" << out.size();
    } else {
        qDebug() << "单列CSV无有效数据：" << path;
    }
    return success;
}

/**
 * @brief 加载所有标定数据（整合矩阵+单列模式）
 */
void  rainbowloadcalibration::loadAllCalibrationData()
{
    // 1. 优先加载矩阵模式S表
    bool loaded_s = loadCsvMatrixSensors_S("data/calibration/data_s.csv");
    if (!loaded_s) {
        // 回退单列模式
        QVector<float> t_list, s_list;
        bool ok_t = loadCsvSingleColumn("data/calibration/data_t.csv", t_list);
        bool ok_s = loadCsvSingleColumn("data/calibration/data_s.csv", s_list);

        if (ok_t && ok_s) {
            int n = std::min(t_list.size(), s_list.size());
            if (t_list.size() != s_list.size()) {
                qDebug() << "警告：单列标定文件行数不一致，按最小对齐：" << t_list.size() << "/" << s_list.size();
            }

            calibration_temperature = t_list;
            calibration_wavelength  = s_list;
            calibration_size_arr = std::min(n, CALIB_MAX);

            // 填充通用插值数组
            for (int i = 0; i < calibration_size_arr; ++i) {
                calibration_temperature_arr[i] = calibration_temperature[i];
                calibration_wavelength_arr[i]  = calibration_wavelength[i];
            }

            qDebug() << "S表单列模式加载完成，有效点数：" << calibration_size_arr;
        } else {
            qDebug() << "警告：S表加载失败（矩阵+单列均失败），插值将返回NaN";
            calibration_size_arr = 0;
        }
    }

    // 2. 优先加载矩阵模式T表
    bool loaded_t = loadCsvMatrixSensors_T("data/calibration/data_t.csv");
    if (!loaded_t) {
        // 回退单列模式
        QVector<float> t_list_only, s_list_only;
        bool ok_t2 = loadCsvSingleColumn("data/calibration/data_t.csv", t_list_only);
        bool ok_s2 = loadCsvSingleColumn("data/calibration/data_s.csv", s_list_only);

        if (ok_t2 && ok_s2) {
            int n2 = std::min(t_list_only.size(), s_list_only.size());
            t_calibration_sensor_count = 1;
            t_calibration_size_per_sensor[0] = std::min(n2, CALIB_MAX);

            // 填充T表第0号传感器
            for (int k = 0; k < t_calibration_size_per_sensor[0]; ++k) {
                t_calibration_temperature_mat[0][k] = t_list_only[k];
                t_calibration_wavelength_mat [0][k] = s_list_only[k];
            }

            qDebug() << "T表单列模式加载完成，有效点数：" << t_calibration_size_per_sensor[0];
        }
    }
}

/**
 * @brief 打印标定数据（调试用）
 */
void  rainbowloadcalibration::printCalibrationData()
{
    qDebug() << "\n========== S 表 (温度 → 波长) ==========";
    qDebug() << "传感器数量:" << s_calibration_sensor_count;

    for (int s = 0; s < s_calibration_sensor_count; ++s) {
        qDebug() << QString("---- S表 传感器 #%1 ----").arg(s);
        int n = s_calibration_size_per_sensor[s];
        for (int k = 0; k < n; ++k) {
            qDebug().noquote()
                << QString("T[%1] = %2 °C , W[%1] = %3 nm")
                       .arg(k, 2)
                       .arg(s_calibration_temperature_mat[s][k], 8, 'f', 3)
                       .arg(s_calibration_wavelength_mat[s][k], 10, 'f', 3);
        }
    }

    qDebug() << "\n========== T 表 (波长 → 温度) ==========";
    qDebug() << "传感器数量:" << t_calibration_sensor_count;

    for (int s = 0; s < t_calibration_sensor_count; ++s) {
        qDebug() << QString("---- T表 传感器 #%1 ----").arg(s);
        int n = t_calibration_size_per_sensor[s];
        for (int k = 0; k < n; ++k) {
            qDebug().noquote()
                << QString("W[%1] = %2 nm , T[%1] = %3 °C")
                       .arg(k, 2)
                       .arg(t_calibration_wavelength_mat[s][k], 10, 'f', 3)
                       .arg(t_calibration_temperature_mat[s][k], 8, 'f', 3);
        }
    }
}
