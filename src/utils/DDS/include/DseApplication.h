#ifndef DSEAPPLICATION_H
#define DSEAPPLICATION_H

#ifndef USE_RTIDDS
#define USE_RTIDDS
#endif
#include <memory>
#include <ATimer.h>
#include <AWsSender.h>
#include "DseCommon.h"
#include "dds/FOSD.h"
#include "dds/FOSDSupport.h"
namespace DseApplication {
    std::string GetDseVersion();
    /**
        组件:FOSD
        描述:
        导出时间:2025-11-26 14:10:24.088

    */
    namespace FOSD {
        std::string getComponetVersion();

        //初始化调用一次即可，多次调用仅第一次有效
        //初始化组件
        //参数workmodeSetHandler为工作模式下发回调函数
        void Init(const WorkModeSetHandler& workmodeSetHandler = emptyWorkModeSetHandler);

        //以下是各主题的订阅函数
        //用户可以基于这些函数自行实现消息分发

        /**

        */
        void Subscribe_NET_SEND_BUFF4(const std::function<void(const DWR_FOSD::NET_SEND_BUFF4& message)>& callback);



        //获取当前工作模式
        std::string GetCurrentWorkMode();
    }

    //异步执行work
    void PostWork(const std::function<void()>& work);

    //创建定时器
    std::shared_ptr<ATimer> CreateTimer();

    //注册HTTP POST Api（回调函数会在子线程中执行）
    //参数1：apiPath  Api的路径，比如"/api/abcd"
    //参数2：apiFunc  Api的回调函数
    void RegisterHttpPostApi(const std::string& apiPath, const std::function<std::string(const std::string&)>& apiFunc);
    void RegisterHttpGetApi(const std::string& apiPath, const std::function<std::string(const std::string&)>& apiFunc);


    //注册websocket回调函数（回调函数会在子线程中执行）
    void RegisterWebsocketApi(const std::string& apiPath, const std::function<void(std::shared_ptr<AWsSender>)>& onWebsocketOpen);

    //启动消息循环，如果不执行该函数，post_work，CreateTimer无法正常工作
    //如果是开发的是Qt程序则不需要调用该函数，定时器及其他功能使用Qt提供的即可
    int Run();
};

#endif
