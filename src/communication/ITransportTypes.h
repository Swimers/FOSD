#pragma once
#include "config/CoreParameters.h"

#include <QHostAddress>
#include <QMetaType>
#include <QSerialPort>
#include <QString>

namespace fosd {
namespace communication {












struct CommunicationError {
    Channel channel = Channel::Serial;
    QString operation;
    QString message;
    int nativeCode = 0;
};

struct DecoderError {
    Channel channel = Channel::Serial;
    QString operation;
    QString message;
    int code = 0;
};

} // namespace communication
} // namespace fosd

Q_DECLARE_METATYPE(fosd::communication::Channel)
Q_DECLARE_METATYPE(fosd::communication::ConnectionState)
Q_DECLARE_METATYPE(fosd::communication::Endpoint)
Q_DECLARE_METATYPE(fosd::communication::SerialConfig)
Q_DECLARE_METATYPE(fosd::communication::UdpConfig)
Q_DECLARE_METATYPE(fosd::communication::TcpConfig)
Q_DECLARE_METATYPE(fosd::communication::CommunicationError)
Q_DECLARE_METATYPE(fosd::communication::DecoderError)
