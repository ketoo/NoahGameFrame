// -------------------------------------------------------------------------




//
// -------------------------------------------------------------------------

#ifndef NFI_WORLDNET_CLIENTMODULE_H
#define NFI_WORLDNET_CLIENTMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"
#include "NFINetClientModule.hpp"

class NFIWorldToMasterModule
	: public  NFIModule
{
public:
	virtual NFINetClientModule* GetNetClientModule() = 0;
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif