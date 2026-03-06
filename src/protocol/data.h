#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <atomic>
#include <memory>
#include "DseApplication.h"
#include "DseStructUtil.h"

class DemoData: public QObject
{
    Q_OBJECT
public:
    DemoData(QObject *parent = nullptr);
    ~DemoData();

    void listen();

private:
    std::shared_ptr<std::atomic_bool> m_aliveFlag;

public:

    typedef struct NET_SEND_BUFF4 {
        float strian[12];
        float temper[12];
        float free[16];
    }NET_SEND_BUFF4;

    NET_SEND_BUFF4 all_data;

public slots:
    void getDemoData(const DWR_FOSD::NET_SEND_BUFF4 data);
signals:
    void signal_data(const DWR_FOSD::NET_SEND_BUFF4 data);

};

#endif // DATA_H
