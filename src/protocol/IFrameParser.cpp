#include "IFrameParser.h"
#include "MPWavelengthParser.h"
#include "JCSWavelengthParser.h"

namespace fosd { namespace protocol {
FrameParsers createFrameParsers() {
    FrameParsers parsers;
    parsers.emplace_back(new MPWavelengthParser);
    parsers.emplace_back(new JCSWavelengthParser);
    return parsers;
}
} }
