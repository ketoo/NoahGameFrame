// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-08
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#ifndef NF_COUNTER_MEMORY_HPP
#define NF_COUNTER_MEMORY_HPP

#include <iostream>
#include <string>
#include <map>
#include "NFComm/NFPluginModule/NFPlatform.h"


template <class T>
class NFMemoryCounter
{
public:

	NFMemoryCounter()
	{
		++nCount;
	}

	virtual ~NFMemoryCounter()
	{
		--nCount;
	}


public:
	static int nCount;
};

template <class T>
int NFMemoryCounter<T>::nCount = 0;

/*
class NFMemoryCounter
{
public:
	NFMemoryCounter()
	{
		if (!mxCounter)
		{
			mxCounter = new std::map<std::string, int>();
		}
	}

	virtual ~NFMemoryCounter()
	{
	}

    void AddInstance(const std::string& strClassName)
    {
        std::map<std::string, int>::iterator it = mxCounter->find(strClassName);
        if(it != mxCounter->end())
        {
            it->second++;
        }
        else
        {
            mxCounter->insert(std::map<std::string, int>::value_type(strClassName, 1));
        }
    }

	void RemInstance(const std::string& strClassName)
	{
		std::map<std::string, int>::iterator it = mxCounter->find(strClassName);
		if (it != mxCounter->end())
		{
			it->second--;
		}
	}

	const std::map<std::string, int>& GetCounter()
	{
		return *mxCounter;
	}

protected:
    static std::map<std::string, int>* mxCounter;
};

std::map<std::string, int>* NFMemoryCounter::mxCounter = NULL;

*/
#endif