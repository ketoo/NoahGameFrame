// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.cpp
//    @Author           :    Yu.Tang
//    @Date             :    2016-11-26
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#include "NFMemoryCounter.h"

NFMemoryCounter::NFMemoryCounter()
{

}

void NFMemoryCounter::MCB_SetClassName(std::string className)
{
	mClassName = className;
	mnCounter[className]++;
}

int NFMemoryCounter::MCB_GetCount()
{
	return mnCounter[mClassName];
}

int NFMemoryCounter::MCB_GetCount(std::string className)
{
	return mnCounter[className];
}

NFMemoryCounter::~NFMemoryCounter()
{
	mnCounter[mClassName]--;
}

std::map<std::string, int> NFMemoryCounter::mnCounter;