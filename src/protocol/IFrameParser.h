#pragma once
#include "core/Datamodel/FrameData.h"
#include <QByteArray>
#include <memory>
#include <vector>

namespace fosd { namespace protocol {
// Receive complete frames; first matching parser owns validation. Extensions
// must construct immutable payloads and retain no writable aliases to them.
// Add an implementation and include it in the injected ParserFactory.
class IFrameParser {
public:
    virtual ~IFrameParser() = default;
    virtual QString protocolId() const = 0;
    virtual bool matches(const QByteArray &packet) const = 0;
    virtual core::FrameDataPtr parse(const QByteArray &packet, QString *error = nullptr) const = 0;
};
using FrameParsers = std::vector<std::unique_ptr<IFrameParser>>;
FrameParsers createFrameParsers();
} } // namespace fosd::protocol
