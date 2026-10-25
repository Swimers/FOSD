#include "PlateUdpSender.h"
#include <cmath>
#include <algorithm>
namespace fosd {
namespace simulation {
PlateUdpSender::PlateUdpSender(QObject *parent) : QObject(parent) {
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::PreciseTimer);
    connect(&m_timer, &QTimer::timeout, this, &PlateUdpSender::sendNext);
}

bool PlateUdpSender::start(const PlateSenderSettings &s, QString *error) {
    if (error)
        error->clear();
    if (s.address.isNull() || !s.port || !std::isfinite(s.rateHz) || s.rateHz < 1 ||
        s.rateHz > 200 || !std::isfinite(s.motion.amplitudeMm) || s.motion.amplitudeMm < 0 ||
        s.motion.amplitudeMm > 100 || !std::isfinite(s.motion.frequencyHz) ||
        s.motion.frequencyHz < 0 || s.motion.frequencyHz > 10) {
        if (error)
            *error = QStringLiteral(
                "Invalid sender: rate 1..200 Hz, amplitude 0..100 mm, motion 0..10 Hz");
        return false;
    }
    stop();
    m_settings = s;
    m_sequence = 0;
    m_nextTick = 0;
    m_running = true;
    m_clock.start();
    m_timer.start(0);
    return true;
}

void PlateUdpSender::stop() {
    m_running = false;
    m_timer.stop();
    m_socket.close();
}

void PlateUdpSender::sendNext() {
    const double seconds = m_clock.nsecsElapsed() * 1e-9;
    const quint32 sequence = ++m_sequence;
    const auto packet = wavelengthPacket(sequence, seconds, m_settings.motion);
    if (m_socket.writeDatagram(packet, m_settings.address, m_settings.port) != packet.size()) {
        const QString error = m_socket.errorString();
        stop();
        emit errorOccurred(error);
        return;
    }
    emit datagramSent(sequence, seconds);
    if (!m_running)
        return;
    const double nowMs = m_clock.nsecsElapsed() * 1e-6;
    // Deadlines use the monotonic clock: avoid accumulating rounded intervals.
    // Skip missed slots under load rather than producing a catch-up burst.
    m_nextTick =
        std::max(m_nextTick + 1, quint64(std::floor(nowMs * m_settings.rateHz / 1000.0)) + 1);
    const int delay = std::max(1, int(std::ceil(m_nextTick * 1000.0 / m_settings.rateHz - nowMs)));
    m_timer.start(delay);
}
} // namespace simulation
} // namespace fosd
