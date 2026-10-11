#pragma once
#include "IFrameParser.h"
namespace fosd { namespace protocol {
class WavelengthFrameParser final : public IFrameParser {
public:
    QString protocolId() const override;
    bool matches(const QByteArray &packet) const override;
    core::FrameDataPtr parse(const QByteArray &packet, QString *error = nullptr) const override;
};
} } // namespace fosd::protocol
