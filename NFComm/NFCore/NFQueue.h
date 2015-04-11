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
#include <mutex>
#include "NFComm/NFPluginModule/NFPlatform.h"

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
		mxQueueMutex.lock();

		mList.push_back(object);

		mxQueueMutex.unlock();


		return true;
	}

    bool Pop(T& object)
	{
		mxQueueMutex.lock();
		if (mList.empty())
		{
			mxQueueMutex.unlock();

			return false;
		}

		object = mList.front();
		mList.pop_front();

		mxQueueMutex.unlock();

		return true;
	}

private:
    std::list<T> mList;
    std::mutex mxQueueMutex;
};

#endif