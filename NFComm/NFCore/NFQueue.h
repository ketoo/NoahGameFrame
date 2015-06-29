// -------------------------------------------------------------------------
//    @FileName      :    NFQueue.h
//    @Author      :    LvSheng.Huang
//    @Date    :    2011-01-21 21:49
//    @Module    :
//
// -------------------------------------------------------------------------

#ifndef _NF_QUEUE_H_
#define _NF_QUEUE_H_

#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <thread>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "Theron/Detail/Threading/SpinLock.h"

template<typename T>
class NFQueue
{
public:
    NFQueue()
	{
	}
    virtual ~NFQueue()
	{
		mList.clear();
	}

    bool Push(const T& object)
	{
		mxQueueMutex.Lock();

		mList.push_back(object);

		mxQueueMutex.Unlock();


		return true;
	}

    bool Pop(T& object)
	{
		mxQueueMutex.Lock();
		if (mList.empty())
		{
			mxQueueMutex.Unlock();

			return false;
		}

		object = mList.front();
		mList.pop_front();

		mxQueueMutex.Unlock();

		return true;
	}

private:
    std::list<T> mList;
    mutable Theron::Detail::SpinLock mxQueueMutex;                 ///< Thread synchronization object protecting the mailbox.
};

#endif