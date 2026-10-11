#pragma once

#include "TransportTypes.h"

#include <QByteArray>
#include <QObject>

namespace fosd {
namespace communication {

class ITransport : public QObject {
    Q_OBJECT
public:
    explicit ITransport(Channel channel, QObject *parent = nullptr)
        : QObject(parent), m_channel(channel) {}
    ~ITransport() override = default;

    virtual void close() = 0;
    virtual void send(const QByteArray &bytes, const Endpoint &peer = Endpoint()) = 0;

signals:
    void bytesReceived(const QByteArray &bytes, const Endpoint &peer);
    void stateChanged(fosd::communication::ConnectionState state);
    void errorOccurred(const fosd::communication::CommunicationError &error);

protected:
    void reportError(const QString &operation, const QString &message, int nativeCode = 0) {
        CommunicationError error;
        error.channel = m_channel;
        error.operation = operation;
        error.message = message;
        error.nativeCode = nativeCode;
        emit errorOccurred(error);
    }

private:
    Channel m_channel;
};

} // namespace communication
} // namespace fosd

