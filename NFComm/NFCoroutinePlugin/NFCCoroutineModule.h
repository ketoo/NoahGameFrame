// -------------------------------------------------------------------------
//    @FileName			:    NFCCoroutineModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_COROUTINE_MODULE_H
#define NF_COROUTINE_MODULE_H

#include <stdint.h>
#include <type_traits>
#include <functional>
#include <list>
#include <vector>
#include <set>
#include <stddef.h>
#include <functional>
#include <exception>
#include <iostream>
#include <ucontext.h>
#include <thread>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

typedef void (*CoroutineFun)(void *arg);

enum NFCoroutineState
{
    FREE,
    READY,
    RUNNING,
    FINISHED,
    BACKING
};


#endif
