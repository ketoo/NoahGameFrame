// -------------------------------------------------------------------------
//    @FileName			:    NFCoroutineModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCoroutineManager.h"

void ExecuteBody(NFCoroutine* co)
{
    //std::cout << "ExecuteBody " << co->nID << std::endl;

    co->func(co->arg);

    co->state = FREE;
    co->pSchdule->RemoveRunningID(co->nID);



    //std::cout << "func finished -- swap " << co->nID << " to -1" << std::endl;
}

NFCoroutineManager::NFCoroutineManager()
{
    std::cout << "threadid " << std::this_thread::get_id() << std::endl;

    mnMainCoID = -1;
    mnRunningCoroutineID = -1;
    mnMaxIndex = 0;

    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        mxCoroutineList.push_back(new NFCoroutine(this, i));
    }

    std::cout << "created Coroutine number: " << MAX_COROUTINE_CAPACITY <<std::endl;
}

NFCoroutineManager::~NFCoroutineManager()
{
    for (int i = 0; i < MAX_COROUTINE_CAPACITY; i++)
    {
        delete mxCoroutineList[i];
    }
}

void NFCoroutineManager::Resume(int id)
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    if (id < 0 || id >= this->mnMaxIndex)
    {
        return;
    }

    NFCoroutine* t = GetCoroutine(id);
    if (t->state == SUSPEND)
    {
        std::cout << this->mnRunningCoroutineID << " swap to " << id << std::endl;

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
        t->state = SUSPEND;

        std::cout << "Yield " << this->mnRunningCoroutineID << " to -1" << std::endl;

        this->mnRunningCoroutineID = -1;

        swapcontext(&(t->ctx), &(mxMainCtx));
    }
#endif
}

void NFCoroutineManager::Init(CoroutineFunction func)
{
    mxMainFunc = func;
    mpMainArg = this;

    NewMainCoroutine();
}

void NFCoroutineManager::StartCoroutine()
{
    NewMainCoroutine();
}

void NFCoroutineManager::StartCoroutine(CoroutineFunction func)
{
    NewMainCoroutine();
    func(this);
}

void NFCoroutineManager::ScheduleJob()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    if (mxRunningList.size() > 0)
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

int NFCoroutineManager::GetRunningID()
{
    return mnRunningCoroutineID;
}

void NFCoroutineManager::SetRunningID(int id)
{
    mnRunningCoroutineID = id;
}

void NFCoroutineManager::RemoveRunningID(int id)
{
    mxRunningList.remove(id);
}

NFCoroutine* NFCoroutineManager::GetCoroutine(int id)
{
    if (id >= 0 && id < mnMaxIndex)
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
    for (; id < mnMaxIndex; ++id)
    {
        if (mxCoroutineList[id]->state == FREE)
        {
            break;
        }
    }

    if (id == mnMaxIndex)
    {
        mnMaxIndex++;
    }

    return mxCoroutineList[id];
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
    //std::cout << "create NewMainCoroutine " << newCo->nID << std::endl;
    mnMainCoID = newCo->nID;

    newCo->state = CoroutineState::SUSPEND;
    newCo->func = mxMainFunc;
    newCo->arg = mpMainArg;

    getcontext(&(newCo->ctx));

    newCo->ctx.uc_stack.ss_sp = newCo->stack;
    newCo->ctx.uc_stack.ss_size = MAX_COROUTINE_STACK_SIZE;
    newCo->ctx.uc_stack.ss_flags = 0;
    newCo->ctx.uc_link = &(this->mxMainCtx);

    makecontext(&(newCo->ctx), (void (*)(void)) (ExecuteBody), 1, newCo);

#endif
}

void NFCoroutineManager::YieldCo(const float fSecond)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    NFSLEEP(nMilliSecond);
#else
    if (this->mnRunningCoroutineID != -1)
    {
        NFCoroutine* t = GetRunningCoroutine();
        t->nYieldTime = fSecond * 1000 + NFGetTimeMS();

        while (1)
        {
            if (NFGetTimeMS() >= t->nYieldTime)
            {
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
