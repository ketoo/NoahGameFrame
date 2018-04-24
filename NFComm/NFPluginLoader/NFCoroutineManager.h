// -------------------------------------------------------------------------
//    @FileName			:    NFCoroutineManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCoroutineManager
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_COROUTINE_MANAGER_H
#define NF_COROUTINE_MANAGER_H

#include <thread>
#include <vector>
#include <list>
#include <iostream>

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif

//#define NF_TEST

#ifdef NF_TEST
#define NF_PLATFORM_WIN 1
#define NF_PLATFORM_LINUX 2
#define NF_PLATFORM_APPLE 3

#define  NF_PLATFORM NF_PLATFORM_APPLE

//millisecond
inline int64_t NFGetTimeMS()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

typedef void (* CoroutineFunction)(void* arg);
#else
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFSingleton.hpp"
#endif

#if NF_PLATFORM != NF_PLATFORM_WIN
#include <ucontext.h>
#endif

#ifdef _DEBUG
#define MAX_COROUTINE_STACK_SIZE (1024 * 512)
#define MAX_COROUTINE_CAPACITY   (128 * 1)
#else
#define MAX_COROUTINE_STACK_SIZE (1024 * 512)
#define MAX_COROUTINE_CAPACITY   (1024 * 1)
#endif

enum CoroutineState
{
    FREE,
    SUSPEND
};


class NFCoroutine;

class NFCoroutineManager;


class NFCoroutine
{
public:
    NFCoroutine(NFCoroutineManager* p, int id)
    {
        pSchdule = p;
        state = CoroutineState::FREE;
        nID = id;
        nYieldTime = 0;
    }

    CoroutineFunction func;
    int64_t nYieldTime;
    void* arg;
    enum CoroutineState state;
    int nID;
    NFCoroutineManager* pSchdule;

#if NF_PLATFORM != NF_PLATFORM_WIN
    ucontext_t ctx;
#endif
    char stack[MAX_COROUTINE_STACK_SIZE];
};

class NFCoroutineManager
{
public:

    NFCoroutineManager();

    virtual ~NFCoroutineManager();

    void Init(CoroutineFunction func);

    void RemoveRunningID(int id);

    void YieldCo();

    void YieldCo(const int64_t nSecond);

    void ScheduleJob();

protected:

    void NewMainCoroutine();

    void Resume(int id);

    NFCoroutine* AllotCoroutine();

    NFCoroutine* GetCoroutine(int id);
    NFCoroutine* GetRunningCoroutine();



protected:
    CoroutineFunction mxMainFunc;
    void* mpMainArg;

#if NF_PLATFORM != NF_PLATFORM_WIN
    ucontext_t mxMainCtx;
#endif
    int mnRunningCoroutineID;
    int mnMainID;

    std::vector<NFCoroutine*> mxCoroutineList;
    std::list<int> mxRunningList;


};


#endif
