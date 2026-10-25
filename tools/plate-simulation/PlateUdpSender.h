#pragma once
#include "PlateSimulation.h"
#include <QElapsedTimer>
#include <QHostAddress>
#include <QTimer>
#include <QUdpSocket>
namespace fosd {
namespace simulation {
struct PlateSenderSettings {
    QHostAddress address = QHostAddress::LocalHost;
    quint16 port = 9000;
    double rateHz = 50.0;
    PlateMotion motion;
};
class PlateUdpSender final : public QObject {
    Q_OBJECT
public:
    explicit PlateUdpSender(QObject *parent = nullptr);
    bool start(const PlateSenderSettings &settings, QString *error = nullptr);
    bool isRunning() const {
        return m_running;
    }
public slots:
    void stop();
signals:
    void datagramSent(quint32 sequence, double seconds);
    void errorOccurred(const QString &message);
private slots:
    void sendNext();

private:
    PlateSenderSettings m_settings;
    QUdpSocket m_socket;
    QTimer m_timer;
    QElapsedTimer m_clock;
    quint32 m_sequence = 0;
    quint64 m_nextTick = 0;
    bool m_running = false;
};
} // namespace simulation
} // namespace fosd
