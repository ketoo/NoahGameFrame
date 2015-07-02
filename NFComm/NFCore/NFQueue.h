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
#include <boost/smart_ptr/detail/spinlock.hpp>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include <mutex>

class spinlock_mutex
{
public:
    spinlock_mutex()
    {
        _spinlock.lock();
		//std::lock_guard<boost::detail::spinlock> guard(_spinlock);
    }
    
    ~spinlock_mutex()
    {
        _spinlock.unlock();
    }

private:
    boost::detail::spinlock _spinlock;
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
		//spinlock_mutex();
        std::lock_guard<boost::detail::spinlock> guard(_spinlock);

		mList.push_back(object);

		return true;
	}

    bool Pop(T& object)
	{
		//spinlock_mutex();
        std::lock_guard<boost::detail::spinlock> guard(_spinlock);

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
    boost::detail::spinlock _spinlock;
};

#endif