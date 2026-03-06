#include "data.h"
#include "global.h"
#include <QtEndian>
#include <QPointer>
#include <cstring>

// 大端转主机序（float）
static float bigEndianToHostFloat(float value) {
    quint32 tmp;
    memcpy(&tmp, &value, sizeof(float));
    tmp = qFromBigEndian(tmp);
    float result;
    memcpy(&result, &tmp, sizeof(float));
    return result;
}

DemoData::DemoData(QObject *parent) : QObject(parent) {
    m_aliveFlag = std::make_shared<std::atomic_bool>(true);
    DseApplication::FOSD::Init([](const std::string& workmode, const std::string& previousWorkmode){});
    qRegisterMetaType<DWR_FOSD::NET_SEND_BUFF4>("DWR_FOSD::NET_SEND_BUFF4");
//    this->listen();

//    connect(this, &DemoData::signal_data, this, &DemoData::getDemoData);
}

DemoData::~DemoData() {
    if (m_aliveFlag) {
        m_aliveFlag->store(false, std::memory_order_release);
    }

}

void DemoData::listen() {
    QPointer<DemoData> self(this);
    auto aliveFlag = m_aliveFlag;

    DseApplication::FOSD::Subscribe_NET_SEND_BUFF4([self, aliveFlag](const DWR_FOSD::NET_SEND_BUFF4 & message){
        if (!aliveFlag || !aliveFlag->load(std::memory_order_acquire) || !self) {
            return;
        }
        // Convert big-endian float payload to host-endian.
        DWR_FOSD::NET_SEND_BUFF4 converted = message;
        for (int i = 0; i < 12; i++) {
            converted.strian[i] = bigEndianToHostFloat(message.strian[i]);
        }
        for (int i = 0; i < 12; i++) {
            converted.temper[i] = bigEndianToHostFloat(message.temper[i]);
        }
        for (int i = 0; i < 16; i++) {
            converted.free[i] = bigEndianToHostFloat(message.free[i]);
        }

        emit self->signal_data(converted);
    });
}

void DemoData::getDemoData(const DWR_FOSD::NET_SEND_BUFF4 data) {

    QMutexLocker locker(&strainMutex);
    // 大端转主机序
    for (int i = 0; i < 12; i++){
        all_data.strian[i] = bigEndianToHostFloat(data.strian[i]);
    }
    for (int i = 0; i < 12; i++){
        all_data.temper[i] = bigEndianToHostFloat(data.temper[i]);
    }
    for (int i = 0; i < 16; i++){
        all_data.free[i] = bigEndianToHostFloat(data.free[i]);
    }

}
