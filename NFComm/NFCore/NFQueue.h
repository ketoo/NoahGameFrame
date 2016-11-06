// -------------------------------------------------------------------------
//    @FileName			:    NFQueue.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2011-01-21 21:49
//    @Module           :
//
// -------------------------------------------------------------------------

#ifndef NF_QUEUE_H
#define NF_QUEUE_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "NFComm/NFPluginModule/NFPlatform.h"

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
    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

protected:
    mutable std::atomic_flag flag;

private:
    NFLock& operator=(const NFLock& src);
};

template<typename T>
class NFQueue : public NFLock
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
        lock();

        mList.push_back(object);

        unlock();

        return true;
    }

    bool Pop(T& object)
    {
        lock();

        if (mList.empty())
        {
            unlock();

            return false;
        }

        object = mList.front();
        mList.pop_front();

        unlock();

        return true;
    }

private:
    std::list<T> mList;
};

#endif