// -------------------------------------------------------------------------
//    @FileName      :    NFQueue.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2011-01-21 21:49
//    @Module    :
//
// -------------------------------------------------------------------------

#ifndef _NF_QUEUE_H_
#define _NF_QUEUE_H_

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "NFComm/NFPluginModule/NFPlatform.h"

class spinlock_mutex
{
public:
    spinlock_mutex()
    {
        lock();
    }

    ~spinlock_mutex()
    {
        unlock();
    }

protected:
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag;
};

template<typename T>
class NFQueue
{
public:
    NFQueue()
    {
    }

    virtual ~NFQueue()
    {
    }

    bool Push(const T& object)
    {
        spinlock_mutex();

        mList.push_back(object);

        return true;
    }

    bool Pop(T& object)
    {
        spinlock_mutex();

        if (mList.empty())
        {
            return false;
        }

        object = mList.front();
        mList.pop_front();

        return true;
    }

private:
    std::list<T> mList;
};

#endif