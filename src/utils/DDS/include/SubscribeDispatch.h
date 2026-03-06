#ifndef SUBSCRIBEDISPATCH_H
#define SUBSCRIBEDISPATCH_H

//本文件基于DseApplication.h中的subscribe_xxx实现了dispatch_xxx，方便同进程内的消息分发
//基本原理是维护了一个回调函数的vector，当收到消息时，依次执行vector中的回调函数
//定义宏USE_SUBSCRIBEDISPATCH启用这些函数

#ifdef USE_SUBSCRIBEDISPATCH

#include <vector>
#include <mutex>
#include "DseApplication.h"

#define CREATE_SUBINIT(topicname, ddstypename) struct SubInit_##topicname { \
    SubInit_##topicname(); \
    static std::mutex m_mutex; \
    static std::vector<std::function<void(const ddstypename &)>> m_callbackVec; \
}; \
inline SubInit_##topicname::SubInit_##topicname() { \
    Subscribe_##topicname([](const ddstypename & message){ \
        for(auto& e: m_callbackVec) e(message); \
    }); \
}

#define PUSHBACK_CALLBACK(topicname) static SubInit_##topicname subinit; \
    SubInit_##topicname::m_mutex.lock(); \
    SubInit_##topicname::m_callbackVec.push_back(callback); \
    SubInit_##topicname::m_mutex.unlock();

namespace DseApplication {
namespace FOSD {

    CREATE_SUBINIT(NET_SEND_BUFF4, DWR_FOSD::NET_SEND_BUFF4)


    inline void dispatch_NET_SEND_BUFF4(const std::function<void(const DWR_FOSD::NET_SEND_BUFF4 &)>& callback) { PUSHBACK_CALLBACK(NET_SEND_BUFF4) }

}
}

#endif

#endif
