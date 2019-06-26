/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

template<typename T>
class NFQueue : public NFLock
{
public:
    NFQueue(unsigned int size = 1000)
            :m_capacity(size),
             m_front(0),
             m_rear(0)
    {
        m_data = new T[m_capacity];
    }

    virtual ~NFQueue()
    {
        delete [] m_data;
    }


    bool Empty()
    {
        return m_front == m_rear;
    }

    unsigned int Size()
    {
        if (m_rear > m_front)
        {
            return m_rear - m_front;
        }
        else if (m_rear == m_front)
        {
            return 0;
        }

        return (m_rear + Capacity() - m_front);
    }

    unsigned int Capacity()
    {
        return m_capacity;
    }

    bool Full()
    {
        return m_front == (m_rear + 1) % Capacity();
    }

    bool Push(const T& object)
    {
        lock();

        if(Full())
        {
            resize();
        }

        m_data[m_rear] = object;
        m_rear = (m_rear + 1) % Capacity();

        unlock();

        Print();

        return true;
    }

    bool TryPop(T& object)
    {
        Print();

        if (!try_lock())
        {
            return false;
        }

        if(Empty())
        {
            unlock();

            return false;
        }

        object = m_data[m_front];
        m_front = (m_front + 1) % Capacity();

        unlock();

        return true;
    }

    bool Pop(T& object)
    {
        Print();

        lock();

        if(Empty())
        {
            unlock();

            return false;
        }

        object = m_data[m_front];
        m_front = (m_front + 1) % Capacity();

        unlock();

        return true;
    }

    void Print()
    {
        return;
/*
        std::cout << "-----------size:" << Size() << " capacity:" << Capacity() << std::endl;

        if (m_rear > m_front)
        {
            for (int i = m_front; i < m_rear; ++i)
            {
                std::cout << " " << m_data[i] << " " << std::endl;
            }
        }
        else if (m_rear < m_front)
        {
            for (int i = m_front; i < m_capacity; ++i)
            {
                std::cout << " " << m_data[i] << " " << std::endl;
            }

            for (int i = 0; i < m_rear; ++i)
            {
                std::cout << " " << m_data[i] << " " << std::endl;
            }
        }
        */
    }

private:
    void resize()
    {
        T* tmp = new T [m_capacity * 2];
        memset(tmp,0 , sizeof(T) * m_capacity *2);

        int count = 0;

        if (m_rear > m_front)
        {
            for (int i = m_front; i < m_rear; ++i)
            {
                tmp[count++] = m_data[i];
            }
        }
        else if (m_rear < m_front)
        {
            for (int i = m_front; i < m_capacity; ++i)
            {
                tmp[count++] = m_data[i];
            }

            for (int i = 0; i < m_rear; ++i)
            {
                tmp[count++] = m_data[i];
            }
        }

        delete[]m_data;

        m_front = 0;
        m_rear = count;
        m_capacity *= 2;
        m_data = tmp;
    }

private:
    int m_capacity;
    int m_front;
    int m_rear;
    T* m_data;
};

#endif