#pragma once

#include "ITransportTypes.h"

#include <QByteArray>
#include <QObject>
#include <QVector>

namespace fosd {
namespace communication {

// One instance is created for each channel on the communication thread.
// For TCP and serial, feed() may receive arbitrary fragments and should buffer
// incomplete frames. For UDP, each feed() call contains one whole datagram.
class IPacketDecoder : public QObject {
    Q_OBJECT
public:
    explicit IPacketDecoder(Channel channel, QObject *parent = nullptr)
        : QObject(parent), m_channel(channel) {}
    ~IPacketDecoder() override = default;
    virtual QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) = 0;
    virtual void reset() {}

    // Call after connecting errorOccurred so configuration failures are observed.
    // feed() also calls this for standalone use. Validation runs only once;
    // reset() clears framing state, not initialization state.
    bool initialize() {
        if (!m_initialized) {
            m_initialized = true;
            const QString message = validateConfiguration();
            m_valid = message.isEmpty();
            if (!m_valid)
                reportError(QStringLiteral("initialize"), message);
        }
        return m_valid;
    }

    Channel channel() const { return m_channel; }

signals:
    void errorOccurred(const fosd::communication::DecoderError &error);

protected:
    virtual QString validateConfiguration() const { return {}; }

    void reportError(const QString &operation, const QString &message, int code = 0) {
        DecoderError error;
        error.channel = m_channel;
        error.operation = operation;
        error.message = message;
        error.code = code;
        emit errorOccurred(error);
    }

private:
    Channel m_channel;
    bool m_initialized = false;
    bool m_valid = false;
};

} // namespace communication
} // namespace fosd

