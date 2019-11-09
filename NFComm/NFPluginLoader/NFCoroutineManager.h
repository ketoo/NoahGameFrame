/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


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
