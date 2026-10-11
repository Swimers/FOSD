#pragma once

#include "TransportTypes.h"

#include <QByteArray>
#include <QVector>

namespace fosd {
namespace communication {

// One instance is created for each channel on the communication thread.
// For TCP and serial, feed() may receive arbitrary fragments and should buffer
// incomplete frames. For UDP, each feed() call contains one whole datagram.
class IPacketDecoder {
public:
    virtual ~IPacketDecoder() = default;
    virtual QVector<QByteArray> feed(const QByteArray &bytes, const Endpoint &peer) = 0;
    virtual void reset() {}
};

} // namespace communication
} // namespace fosd

