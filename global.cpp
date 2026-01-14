#include "global.h"

<<<<<<< Updated upstream:Dependence/global.cpp
QMutex mutexAnalyze;  // 新增: 用于保护解算过程的互斥锁
QMutex mutexStrain;  // 新增: 用于保护解算过程的互斥锁
=======
QMutex strainMutex;  // 定义全局的互斥锁变量
>>>>>>> Stashed changes:global.cpp

// 可以在这里进行 strainMutex 的初始化或其他操作
