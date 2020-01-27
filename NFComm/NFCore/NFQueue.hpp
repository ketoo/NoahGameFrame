/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NF_QUEUE_H
#define NF_QUEUE_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "Dependencies/concurrentqueue/concurrentqueue.h"

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
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    bool try_lock()
    {
        if (flag.test_and_set(std::memory_order_acquire))
        {
            return false;
        }

        return true;
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }

protected:
    mutable std::atomic_flag flag = ATOMIC_FLAG_INIT;

private:
    NFLock& operator=(const NFLock& src);
};

//read prior or write prior?
//it's different for these two situations

template<typename T>
//class NFQueue : public NFLock
class NFQueue : public moodycamel::ConcurrentQueue<T>
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
		this->enqueue(object);

        return true;
    }

	bool PushBulk(const T& object)
	{
		this->enqueue(object);

		return true;
	}

    bool TryPop(T& object)
    {
		return this->try_dequeue(object);
    }
	
	bool TryPopBulk(T& object)
	{
		return this->try_dequeue(object);
	}
};

#endif