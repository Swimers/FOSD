#pragma once

// Authoritative parameter structures. Moved without changing their defaults;
// original module headers re-export these names for source compatibility.
#include "core/Datamodel/ModelData.h"
#include <QHostAddress>
#include <QSerialPort>
#include <QFlags>
#include <array>
#include <vector>

namespace fosd {
namespace communication {
enum class Channel { Serial, Udp, Tcp };

enum class ConnectionState { Closed, Opening, Ready, Listening, Connected };

enum class TcpRole { Client, Server };

struct Endpoint {
    QHostAddress address;
    quint16 port = 0;

    // A peer needs both a concrete address and a nonzero port.
    bool isValid() const;
};

struct SerialConfig {
    QString portName;
    qint32 baudRate = 115200;
    QSerialPort::DataBits dataBits = QSerialPort::Data8;
    QSerialPort::Parity parity = QSerialPort::NoParity;
    QSerialPort::StopBits stopBits = QSerialPort::OneStop;
    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;
};

struct UdpConfig {
    QHostAddress bindAddress = QHostAddress::AnyIPv4;
    quint16 localPort = 0; // 0 asks the OS for an available port.
    Endpoint defaultPeer;
};

struct TcpConfig {
    TcpRole role = TcpRole::Client;
    QString host = QStringLiteral("127.0.0.1"); // Client address or host name.
    QHostAddress listenAddress = QHostAddress::AnyIPv4;
    quint16 port = 0;
};
} // namespace communication
namespace Calculation {
struct CalibrationStrainData {
    std::vector<double> originalWavelength;
    double calibrationFactor = 0.00075;
    double A = 0.0;
    double B = 0.0;
};

struct CalibrationChannelData {
    std::vector<int> channelIndices;        // 校准通道索引数组
    std::vector<double> calibrationFactors; // 校准因子数组
};

enum class CalculationAlgorithm { HYY };

struct CalculationConfiguration {
    CalculationAlgorithm algorithm = CalculationAlgorithm::HYY;
    CalibrationStrainData strainCalibration;   // 校准应变数据
    CalibrationChannelData channelCalibration; // 校准通道数据
};
} // namespace Calculation
namespace storage {
using PointId = core::PointId;
enum class StorageFormat { Csv, SQLite };

enum class DataType { Wavelength = 1, CalibratedHardware = 2, PointDisplacement = 4 };
Q_DECLARE_FLAGS(DataTypes, DataType)
struct StorageConfiguration {
    StorageFormat format = StorageFormat::Csv;  // 写入方式
    QString path;                               // A single append-only CSV or SQLite file.
    DataTypes dataTypes = DataType::Wavelength; // 写入数据类型
    QVector<PointId> pointIds;
    bool allPoints = false;
};
} // namespace storage
namespace visualization {
struct VisualizationRuntimeSettings {
    double deformationScale = 1.0;
    int refreshIntervalMs = 33;
};

enum class ScalarField { DisplacementX, DisplacementY, DisplacementZ, Magnitude, Custom };

enum class ScalarMappingMode { Normalized, Physical };

enum class ScalarValueMode { Signed, Absolute };

enum class ColorRangeMode { Automatic, Manual };

enum class CameraPreset { Isometric, Front, Back, Left, Right, Top, Bottom };

enum class GridPlane { XZ, XY, YZ };

enum class CameraProjection { Perspective, Parallel };

struct ScalarColorSettings {
    bool enabled = false;
    ScalarField field = ScalarField::Magnitude;
    ScalarMappingMode mappingMode = ScalarMappingMode::Normalized;
    ScalarValueMode valueMode = ScalarValueMode::Signed;
    ColorRangeMode rangeMode = ColorRangeMode::Manual;
    double minimum = 0.0;
    double maximum = 50.0;
    double hueStart = 0.6;
    double hueEnd = 0.1;
};

struct AxesSettings {
    bool visible = false;
    bool labelsVisible = true;
    double lengthMm = 100.0;
    std::array<double, 3> originMm{{0, 0, 0}};
    int fontSize = 10;
};

struct GridSettings {
    std::array<bool, 3> visible{{false, false, false}}; // XZ, XY, YZ.
    double halfExtentMm = 1500.0;
    int divisions = 50; // Intervals from the origin to each edge.
    std::array<double, 3> originMm{{0, 0, 0}};
    std::array<double, 3> color{{0.7, 0.7, 0.7}};
    double opacity = 0.1;
    double lineWidth = 1.0;
};

struct BackgroundSettings {
    bool gradient = false;
    std::array<double, 3> bottomColor{{0, 0, 0}};
    std::array<double, 3> topColor{{0.2, 0.3, 0.5}};
};

struct CameraSettings {
    CameraProjection projection = CameraProjection::Perspective;
    double viewAngleDegrees = 30.0;
    double parallelScaleMm = 1000.0; // Half the visible vertical extent.
    bool locked = false;             // Mouse camera lock; explicit API changes remain allowed.
};

struct WorldSettings {
    bool groundVisible = false;
    bool skyVisible = false;
    double groundHalfExtentMm = 10000.0;
    double groundYmm = -100.0; // Legacy ground lies in XZ (Y is constant).
    double skyRadiusMm = 20000.0;
    std::array<double, 3> centerMm{{0, 0, 0}};
    std::array<double, 3> groundColor{{0.3, 0.6, 0.3}};
    std::array<double, 3> skyColor{{0.6, 0.85, 1.0}};
    QString groundTexturePath; // Empty = solid color; file or Qt resource path.
    QString skyTexturePath;
};

struct KeyPointStyle {
    double radiusMm = 5.0;
    std::array<double, 3> color{{1.0, 0.3, 0.1}};
    std::array<double, 3> labelColor{{1.0, 1.0, 1.0}};
    std::array<double, 3> labelOffsetMm{{0.0, 0.0, 8.0}};
    int fontSize = 14;
    int sphereResolution = 16;
    bool labelsVisible = true;
};

struct PickingSettings {
    bool enabled = true;
    bool onlyKeyPoints = false;
    bool clearOnCameraInteraction = false; // 标志位，旋转/平移/缩放相机时，是否自动取消当前选中的点
    double tolerance = 0.009;     // Fraction of render-window diagonal (VTK).
    int clickTolerancePixels = 4; // Device pixels, not Qt logical pixels.
};

struct SelectionStyleSettings : KeyPointStyle {
    SelectionStyleSettings();
};
} // namespace visualization
} // namespace fosd
