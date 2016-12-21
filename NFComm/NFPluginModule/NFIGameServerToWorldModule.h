// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_GAMESERVERTOWORLDMODULE_H
#define NFI_GAMESERVERTOWORLDMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.hpp"

class NFIGameServerToWorldModule
	: public NFIModule
{

public:
	virtual void SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
	virtual NFINetClientModule* GetClusterClientModule() = 0;
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif