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

#ifndef NF_STRING_RING_BUFF_H
#define NF_STRING_RING_BUFF_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>
#include "NFComm/NFPluginModule/NFPlatform.h"

//template<typename T>
class NFStringRingBuffer
{
public:
    NFStringRingBuffer(unsigned int size = 1024 * 1024 * 1)
        :m_capacity(size),
        m_front(0),
        m_rear(0)
    {
        //m_data = new T[m_capacity];
        m_data = new char[m_capacity];
    }

    virtual ~NFStringRingBuffer()
    {
        //delete[] m_data;
    }


    bool Empty()
    {
        return m_front == m_rear;
    }

    unsigned int Size() const
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

    unsigned int Capacity() const
    {
        return m_capacity;
    }

    bool Full(int newSize)
    {
        return (Size() + newSize) >= Capacity();
    }

    bool Push(const char* src, int size)
    {
        while (Full(size))
        {
            resize();
        }

        if (m_rear >= m_front)
        {
            if (m_rear + size >= Capacity())
            {
                int offset = Capacity() - m_rear;
                memcpy(m_data + m_rear, src, offset);
                memcpy(m_data, src + offset, (size_t)(size - offset));
            }
            else
            {
                memcpy(m_data + m_rear, src, size);
            }
        }
        else
        {
            if (m_rear + size < m_front)
            {
                memcpy(m_data + m_rear, src, size);
            }
            else
            {
                //impassible!!!!
            }
        }

        m_rear = (m_rear + size) % Capacity();

        return true;
    }

    bool Pop(char* dst, int size, bool readOnly = false)
    {
        if (size <= 0 || Size() < size)
        {
            return false;
        }

        if (m_rear > m_front)
        {
            memcpy(dst, m_data + m_front, size);
        }
        else
        {
            int tempOffset = Capacity() - m_front;

            memcpy(dst, m_data + m_front, tempOffset);
            memcpy(dst + tempOffset, m_data, (size_t)(size - tempOffset));
        }

        if (!readOnly)
        {
            m_front = (m_front + size) % Capacity();
        }

        return true;
    }

    bool Erase(int size)
    {
        if (size <= 0 || Size() < size)
        {
            return false;
        }

        m_front = (m_front + size) % Capacity();

        return true;
    }

    //dangerous dark magic: zero copy pop
    //you can use this function when you sure that the memory is stick.
    //if you are failed in calling this function, plz use Pop function do it again.
    bool ZeroCopyPop(char** dst, int size, bool readOnly = false)
    {
        if (size <= 0 || Size() < size)
        {
            return false;
        }

        if (m_front > m_rear)
        {
            //dont have a stick memory
            //we apply a new stick memory for this copy


            return false;
        }

        *dst = m_data + m_front;

        if (!readOnly)
        {
            m_front = (m_front + size) % Capacity();
        }

        return true;
    }

    void Print()
    {
        //std::cout << "-----------size:" << Size() << " capacity:" << Capacity() << " m_front:" << m_front << " m_rear:" << m_rear << std::endl;
    }

    void Sort()
    {
        //only when rear < front and only one element in buffer
    }

    void Clear()
    {
        m_front = 0;
        m_rear = 0;
    }
private:
    void resize()
    {
        //T* tmp = new T[m_capacity * 2];
        char* tmp = new char[m_capacity * 2];
        //memset(tmp, 0, sizeof(char) * m_capacity * 2);
        if (m_rear < m_front)
        {
            int oldSize = Size();
            int offset = m_capacity - m_front;
            memcpy(tmp, m_data + m_front, offset);
            memcpy(tmp + offset, m_data, m_rear);
            m_front = 0;
            m_rear = oldSize;
        }
        else
        {
            memcpy(tmp, m_data, m_capacity);
        }

        delete[] m_data;

        m_capacity *= 2;
        m_data = tmp;
    }

private:
    int m_capacity;
    int m_front;
    int m_rear;
    //T* m_data;
    char* m_data;
};

/*

    NFStringRingBuffer stringBuffers[10];

    //test 0, for basic feature push and pop
    {
        NFStringRingBuffer sb = stringBuffers[0];

        std::string str1 = "test12ergfdgfdgdgdsd111111";
        std::string str2 = "test1fdsfsdfdsf";
        std::string str3 = "test1sdfsdfvxcv";
        std::string str4 = "test1sdfdsfvcxvx";
        std::string str5 = "test1svdfvcvdfgvdfgfd";

        std::cout << "input1 ===> " << str1 << std::endl;
        std::cout << "input2 ===> " << str2 << std::endl;
        std::cout << "input3 ===> " << str3 << std::endl;
        std::cout << "input4 ===> " << str4 << std::endl;
        std::cout << "input5 ===> " << str5 << std::endl;

        sb.Push(str1.c_str(), str1.length());
        sb.Print();
        sb.Push(str2.c_str(), str2.length());
        sb.Print();
        sb.Push(str3.c_str(), str3.length());
        sb.Print();
        sb.Push(str4.c_str(), str4.length());
        sb.Print();
        sb.Push(str5.c_str(), str5.length());
        sb.Print();

        std::cout << " ===============> " << std::endl;
        char* dst1 = new char[str1.length()];
        char* dst2 = new char[str2.length()];
        char* dst3 = new char[str3.length()];
        char* dst4 = new char[str4.length()];
        char* dst5 = new char[str5.length()];
        memset(dst1, 0, str1.length());
        memset(dst2, 0, str2.length());
        memset(dst3, 0, str3.length());
        memset(dst4, 0, str4.length());
        memset(dst5, 0, str5.length());

        sb.Print();
        bool ret1 = sb.Pop(dst1, str1.length());
        sb.Print();
        bool ret2 = sb.Pop(dst2, str2.length());
        sb.Print();
        bool ret3 = sb.Pop(dst3, str3.length());
        sb.Print();
        bool ret4 = sb.Pop(dst4, str4.length());
        sb.Print();
        bool ret5 = sb.Pop(dst5, str5.length());
        sb.Print();

        std::cout << "output1 ===> " << dst1 << std::endl;
        std::cout << "output2 ===> " << dst2 << std::endl;
        std::cout << "output3 ===> " << dst3 << std::endl;
        std::cout << "output4 ===> " << dst4 << std::endl;
        std::cout << "output5 ===> " << dst5 << std::endl;
    }

    std::cout << "test 11111===>===>===>===>===>===>===>===>===> " << std::endl;
    //test 1
    {
        NFStringRingBuffer sb = stringBuffers[1];

        std::string str1 = "test12ergfdgfdgdgdsd111111";
        std::string str2 = "tgfddddddergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str3 = "test1sdfsdfvxcvtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str4 = "test1sdfdsfvcxvxtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str5 = "test1svdfvcvdfgvdfgfdtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";

        std::cout << "input1 ===> " << str1 << std::endl;
        std::cout << "input2 ===> " << str2 << std::endl;
        std::cout << "input3 ===> " << str3 << std::endl;
        std::cout << "input4 ===> " << str4 << std::endl;
        std::cout << "input5 ===> " << str5 << std::endl;

        sb.Push(str1.c_str(), str1.length());
        sb.Print();
        sb.Push(str2.c_str(), str2.length());
        sb.Print();
        sb.Push(str3.c_str(), str3.length());
        sb.Print();
        sb.Push(str4.c_str(), str4.length());
        sb.Print();
        sb.Push(str5.c_str(), str5.length());
        sb.Print();

        std::cout << " ===============> " << std::endl;
        char* dst1 = new char[str1.length()];
        char* dst2 = new char[str2.length()];
        char* dst3 = new char[str3.length()];
        char* dst4 = new char[str4.length()];
        char* dst5 = new char[str5.length()];
        memset(dst1, 0, str1.length());
        memset(dst2, 0, str2.length());
        memset(dst3, 0, str3.length());
        memset(dst4, 0, str4.length());
        memset(dst5, 0, str5.length());

        sb.Print();
        bool ret1 = sb.Pop(dst1, str1.length());
        sb.Print();
        bool ret2 = sb.Pop(dst2, str2.length());
        sb.Print();
        bool ret3 = sb.Pop(dst3, str3.length());
        sb.Print();
        bool ret4 = sb.Pop(dst4, str4.length());
        sb.Print();
        bool ret5 = sb.Pop(dst5, str5.length());
        sb.Print();

        std::cout << "output1 ===> " << dst1 << std::endl;
        std::cout << "output2 ===> " << dst2 << std::endl;
        std::cout << "output3 ===> " << dst3 << std::endl;
        std::cout << "output4 ===> " << dst4 << std::endl;
        std::cout << "output5 ===> " << dst5 << std::endl;
    }
    std::cout << "test 22222===>===>===>===>===>===>===>===>===> " << std::endl;
    //test 2
    {
        NFStringRingBuffer sb = stringBuffers[2];

        std::string str1 = "test12ergfdgfdgdgdsd111111";
        std::string str2 = "tgfddddddergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str3 = "test1sdfsdfvxcvtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str4 = "test1sdfdsfvcxvxtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";
        std::string str5 = "test1svdfvcvdfgvdfgfdtest12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111test12ergfdgfdgdgdsd111111";

        std::cout << "input1 ===> " << str1 << std::endl;
        std::cout << "input2 ===> " << str2 << std::endl;
        std::cout << "input3 ===> " << str3 << std::endl;
        std::cout << "input4 ===> " << str4 << std::endl;
        std::cout << "input5 ===> " << str5 << std::endl;

        char* dst1 = new char[str1.length()];
        char* dst2 = new char[str2.length()];
        char* dst3 = new char[str3.length()];
        char* dst4 = new char[str4.length()];
        char* dst5 = new char[str5.length()];
        memset(dst1, 0, str1.length());
        memset(dst2, 0, str2.length());
        memset(dst3, 0, str3.length());
        memset(dst4, 0, str4.length());
        memset(dst5, 0, str5.length());


        sb.Push(str1.c_str(), str1.length());
        sb.Print();
        bool ret1 = sb.Pop(dst1, str1.length());
        sb.Print();

        sb.Push(str2.c_str(), str2.length());
        sb.Print();
        bool ret2 = sb.Pop(dst2, str2.length());
        sb.Print();

        sb.Push(str3.c_str(), str3.length());
        sb.Print();
        sb.Push(str4.c_str(), str4.length());
        sb.Print();
        sb.Push(str5.c_str(), str5.length());
        sb.Print();

        std::cout << " ===============> " << std::endl;



        bool ret3 = sb.Pop(dst3, str3.length());
        sb.Print();
        bool ret4 = sb.Pop(dst4, str4.length());
        sb.Print();
        bool ret5 = sb.Pop(dst5, str5.length());
        sb.Print();

        std::cout << "output1 ===> " << dst1 << std::endl;
        std::cout << "output2 ===> " << dst2 << std::endl;
        std::cout << "output3 ===> " << dst3 << std::endl;
        std::cout << "output4 ===> " << dst4 << std::endl;
        std::cout << "output5 ===> " << dst5 << std::endl;
    }

*/
#endif