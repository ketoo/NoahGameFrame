// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.h
//    @Author           :    Yu.Tang
//    @Date             :    2016-11-26
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#ifndef NF_MEMORY_COUNTER_H
#define NF_MEMORY_COUNTER_H

#include <iostream>
#include <string>
#include <map>
#include "NFComm/NFPluginModule/NFPlatform.h"

template <class T>
class NFMemoryCounter
{
private:
	NFMemoryCounter() {}
public:
	NFMemoryCounter(const std::string& strClassName)
	{
		mstrClassName = strClassName;
		mxCounter[mstrClassName]++;
	}

	~NFMemoryCounter()
	{
		mxCounter[mstrClassName]--;
	}

	std::string mstrClassName;
	static std::map<std::string, int> mxCounter;
};

template <class T>
std::map<std::string, int> NFMemoryCounter<T>::mxCounter;

#endif