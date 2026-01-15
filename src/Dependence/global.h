#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutex>
#include <QMutexLocker>

extern QMutex mutexAnalyze;     // 新增: 用于保护数据帧解析过程的互斥锁
extern QMutex mutexStrain;      // 新增: 用于保护解算过程的互斥锁
extern QMutex mutexModel;       // 新增: 用于保护模型更新过程的互斥锁

#endif // GLOBAL_H
