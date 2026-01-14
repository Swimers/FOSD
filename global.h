#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutexLocker>  // 新增

extern QMutex strainMutex;  // 新增: 用于保护 strain_float 的互斥锁


#endif // GLOBAL_H
