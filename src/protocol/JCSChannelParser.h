#pragma once

#include "IFrameParser.h"

namespace fosd {
namespace protocol {
// Legacy FA kk FA kk curve packets (kk=0..5, as dispatched by the old code).
// Retired protocol: even a valid packet returns no payload. An empty error
// means decoding succeeded and the samples were discarded. Not registered in
// the production factory, whose current contract treats null data as failure.
class JCSChannelParser final : public IFrameParser {
public:
    QString protocolId() const override;
    bool matches(const QByteArray &packet) const override;
    core::FrameDataPtr parse(const QByteArray &packet, QString *error = nullptr) const override;
};

} // namespace protocol
} // namespace fosd
