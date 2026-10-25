#ifndef JCSWAVELENGTHPARSER_H
#define JCSWAVELENGTHPARSER_H

#pragma once
#include "IFrameParser.h"

namespace fosd {
namespace protocol {
class JCSWavelengthParser final : public IFrameParser {
public:
    QString protocolId() const override;
    bool matches(const QByteArray &packet) const override;
    core::FrameDataPtr parse(const QByteArray &packet, QString *error = nullptr) const override;
};

} // namespace protocol
} // namespace fosd

#endif