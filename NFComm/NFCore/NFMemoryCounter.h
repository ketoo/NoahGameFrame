// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.h
//    @Author           :    Yu.Tang
//    @Date             :    2016-11-26
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#ifndef NF_COUNTER_MEMORY_BASE_HPP
#define NF_COUNTER_MEMORY_BASE_HPP

#include <iostream>
#include <string>
#include <map>
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFMemoryCounter
{
public:
	NFMemoryCounter();
	void MCB_SetClassName(std::string className);
	int MCB_GetCount();
	int MCB_GetCount(std::string className);
	virtual ~NFMemoryCounter();
private:
	std::string mClassName = "Default";
	static std::map<std::string, int> mnCounter;
};
#endif