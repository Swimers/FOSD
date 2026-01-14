#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutexLocker>  // 新增

extern QMutex strainMutex;  // 新增: 用于保护传递 strain_float 的互斥锁

extern QMutex analyzeMutex;  // 新增: 用于保护解算过程的互斥锁

#endif // GLOBAL_H
