#pragma once
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
#include "NFPlatform.h"

const int QUEUE_SIZE = 10000;

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
		queueMutex.lock();
		if (!Empty())
		{
			queueMutex.unlock();

			return false;
		}

		mList.push_back(object);

		queueMutex.unlock();


		return true;
	}

    bool Pop(T& object)
	{
		queueMutex.lock();
		if (Empty())
		{
			queueMutex.unlock();

			return false;
		}

		object = mList.pop_front();

		queueMutex.unlock();

		return true;
	}

protected:

    bool Empty()
	{
		return mList.empty();
	}

private:
    std::list<T> mList;
    std::mutex queueMutex;
};

#endif