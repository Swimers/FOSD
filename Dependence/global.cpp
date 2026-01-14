#include "global.h"

QMutex mutexAnalyze;  // 新增: 用于保护解算过程的互斥锁
QMutex mutexStrain;  // 新增: 用于保护解算过程的互斥锁

// 可以在这里进行 strainMutex 的初始化或其他操作
