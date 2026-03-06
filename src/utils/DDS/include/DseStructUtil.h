#ifndef DSESTRUCTUTIL_H
#define DSESTRUCTUTIL_H
#ifndef USE_RTIDDS
#define USE_RTIDDS
#endif

#include "dds/FOSD.h"
#include <string>
namespace DseStructUtil {

    bool ConvertFromJson(const std::string& _json_str, DWR_FOSD::NET_SEND_BUFF4& _message);
    std::string ConvertToJson(const DWR_FOSD::NET_SEND_BUFF4& _message, bool _pretty = false);

}
#endif
