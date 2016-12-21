// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.cpp
//    @Author           :    Yu.Tang
//    @Date             :    2016-11-26
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#include "NFMemoryCounter.h"

std::map<std::string, int> NFMemoryCounter::mxCounter;

NFMemoryCounter::NFMemoryCounter(const std::string& strClassName)
{
	mstrClassName = strClassName;
	mxCounter[mstrClassName]++;
}

NFMemoryCounter::~NFMemoryCounter()
{
	mxCounter[mstrClassName]--;
}

const std::map<std::string, int>& NFMemoryCounter::GetCounter()
{
	return mxCounter;
}
