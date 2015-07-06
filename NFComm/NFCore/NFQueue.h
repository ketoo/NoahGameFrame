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

class spinlock_mutex;
class NFLock
{
public:
    explicit NFLock()
    {
        flag.clear();
    }

    ~NFLock()
    {
    }
protected:
    std::atomic_flag flag;

    friend spinlock_mutex;

private:
    NFLock& operator=(const NFLock& src);
};

class spinlock_mutex
{
public:
    explicit spinlock_mutex(NFLock& xGuard):mGuard(xGuard)
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
        while (mGuard.flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        mGuard.flag.clear(std::memory_order_release);
    }

private:
    spinlock_mutex& operator=(const spinlock_mutex& src);

private:
    NFLock& mGuard;
};

template<typename T>
class NFQueue :public NFLock
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
        spinlock_mutex(*this);
        mList.push_back(object);

        return true;
    }

    bool Pop(T& object)
    {
        spinlock_mutex(*this);
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