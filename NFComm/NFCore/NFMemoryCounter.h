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

class _NFExport NFMemoryCounter
{
private:
	NFMemoryCounter() {}
public:
	NFMemoryCounter(const std::string& strClassName);
	virtual ~NFMemoryCounter();

	static const std::map<std::string, int>& GetCounter();
private:
	std::string mstrClassName;
	static std::map<std::string, int> mxCounter;
};
#endif