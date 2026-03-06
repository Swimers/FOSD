#ifndef DSECOMMON_H
#define DSECOMMON_H

#include <string>
#include <cstdint>
#include <functional>
#include <vector>

namespace DseApplication {
using WorkModeSetHandler = std::function<void(const std::string& workmode, const std::string& previousWorkMode)>;
static const WorkModeSetHandler emptyWorkModeSetHandler = [](const std::string& workmode, const std::string& previousWorkMode){};
struct DseDate {
public:
    DseDate();
    std::string str() const;
    uint64_t timestamp() const; // in millisecond
    void fromTimestamp(uint64_t timestamp);  //timestamp in millisecond
public:
    uint16_t year;
    uint8_t month;  //1 ~ 12
    uint8_t day;
    uint8_t weekday;   //1 ~ 7
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t millisecond;
    int8_t isdst;
#ifdef __linux__
    char tz[8];  //time zone abbreviation
    int32_t gmtoffset; //offset to UTC (in second)
#endif
};

class DseStatus final {
public:
    static DseStatus fine();
    static DseStatus error(const std::string& _str);
    static DseStatus error(std::string&& _str);
    DseStatus(const DseStatus& _dseStatus);
    DseStatus(DseStatus&& _dseStatus);
    DseStatus& operator=(const DseStatus& _dseStatus);
    DseStatus& operator=(DseStatus&& _dseStatus);
    operator bool() const;
    std::string str() const;
private:
    DseStatus();
    DseStatus(bool _fine, const std::string& _str);
    DseStatus(bool _fine, std::string&& _str);
private:
    bool m_fine;
    std::string m_str;
};

void GetDate(DseDate& date);
}
#endif
