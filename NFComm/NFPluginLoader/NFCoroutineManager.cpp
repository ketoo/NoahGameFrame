/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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


#include "NFCoroutineManager.h"

void ExecuteBody(NFCoroutine* co)
{
    //std::cout << "ExecuteBody " << co->nID << std::endl;

    co->func(co->arg);

    co->state = FREE;


    co->pSchdule->RemoveRunningID(co->nID);

    //std::cout << "Free " << co->nID << std::endl;

    co->pSchdule->YieldCo();
}

NFCoroutineManager::NFCoroutineManager()
{
    mnRunningCoroutineID = -1;
    mnMainID = -1;

    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        mxCoroutineList.push_back(new NFCoroutine(this, i));
    }
}

NFCoroutineManager::~NFCoroutineManager()
{
    for (int i = 0; i < mxCoroutineList.size(); i++)
    {
        delete mxCoroutineList[i];
    }

    mxCoroutineList.clear();
}

void NFCoroutineManager::Resume(int id)
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    if (id < 0 || id >= this->mxCoroutineList.size())
    {
        return;
    }

    NFCoroutine* t = GetCoroutine(id);
    if (t->state == SUSPEND)
    {
        //std::cout << this->mnRunningCoroutineID << " swap to " << id << std::endl;
        this->mnRunningCoroutineID = id;
        swapcontext(&(this->mxMainCtx), &(t->ctx));
    }
#endif
}

void NFCoroutineManager::YieldCo()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    if (this->mnRunningCoroutineID != -1)
    {
        NFCoroutine* t = GetRunningCoroutine();

        if (mnMainID == t->nID)
        {
            mnMainID = -1;
        }

        //std::cout << "Yield " << this->mnRunningCoroutineID << " to -1" << std::endl;

        this->mnRunningCoroutineID = -1;

        swapcontext(&(t->ctx), &(mxMainCtx));
    }
#endif
}

void NFCoroutineManager::YieldCo(const int64_t nSecond)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    NFSLEEP(nSecond);
#else

    if (this->mnRunningCoroutineID != -1)
    {
        NFCoroutine* t = GetRunningCoroutine();
        int64_t nTimeMS = NFGetTimeMS();
        t->nYieldTime = nSecond + nTimeMS;
        //std::cout << nTimeMS << std::endl;
        while (1)
        {
            nTimeMS = NFGetTimeMS();
            if (nTimeMS >= t->nYieldTime)
            {
                //std::cout << nTimeMS << std::endl;
                break;
            }
            else
            {
                YieldCo();
            }
        }
    }
#endif
}
void NFCoroutineManager::Init(CoroutineFunction func)
{
    mxMainFunc = func;
    mpMainArg = this;

    NewMainCoroutine();
}

void NFCoroutineManager::ScheduleJob()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    //std::cout << "ScheduleJob, mainID = " << mnMainID << std::endl;

    if (mxRunningList.size() > 0 && mnMainID >= 0)
    {
        int id = mxRunningList.front();
        mxRunningList.pop_front();

        NFCoroutine* pCoroutine = mxCoroutineList[id];

        if (pCoroutine->state == SUSPEND)
        {
            mxRunningList.push_back(id);

            Resume(id);
        }
    }
    else
    {
        NewMainCoroutine();
    }
#else
    mxMainFunc(this);
#endif
}

void NFCoroutineManager::RemoveRunningID(int id)
{
    const int lastID = mxRunningList.back();
    if (lastID == id)
    {
        mxRunningList.pop_back();
    }
    else
    {
        mxRunningList.remove(id);
    }
}

NFCoroutine* NFCoroutineManager::GetCoroutine(int id)
{
    if (id >= 0 && id < mxCoroutineList.size())
    {
        return mxCoroutineList[id];
    }

    return NULL;
}

NFCoroutine* NFCoroutineManager::GetRunningCoroutine()
{
    if (mnRunningCoroutineID < 0)
    {
        return NULL;
    }

    return mxCoroutineList[mnRunningCoroutineID];
}


NFCoroutine* NFCoroutineManager::AllotCoroutine()
{
    int id = 0;
    for (; id < this->mxCoroutineList.size(); ++id)
    {
        if (mxCoroutineList[id]->state == FREE)
        {
            break;
        }
    }

    if (id == this->mxCoroutineList.size())
    {
        this->mxCoroutineList.push_back(new NFCoroutine(this, id));
    }

    return this->mxCoroutineList[id];
}

void NFCoroutineManager::NewMainCoroutine()
{
#if NF_PLATFORM != NF_PLATFORM_WIN

    NFCoroutine* newCo = AllotCoroutine();
    if (newCo == NULL)
    {
        return;
    }

    mxRunningList.push_back(newCo->nID);
    mnMainID = newCo->nID;

    //std::cout << "create NewMainCoroutine " << newCo->nID << std::endl;

    newCo->state = CoroutineState::SUSPEND;
    newCo->func = mxMainFunc;
    newCo->arg = mpMainArg;
    newCo->nYieldTime = 0;

    getcontext(&(newCo->ctx));

    newCo->ctx.uc_stack.ss_sp = newCo->stack;
    newCo->ctx.uc_stack.ss_size = MAX_COROUTINE_STACK_SIZE;
    newCo->ctx.uc_stack.ss_flags = 0;
    newCo->ctx.uc_link = &(this->mxMainCtx);

    makecontext(&(newCo->ctx), (void (*)(void)) (ExecuteBody), 1, newCo);
#endif
}
