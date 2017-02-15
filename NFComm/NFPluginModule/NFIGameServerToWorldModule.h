// -------------------------------------------------------------------------
//    @FileName			:    NFIGameServerToWorldModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFIGameServerToWorldModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFI_GAMESERVERTOWORLDMODULE_H
#define NFI_GAMESERVERTOWORLDMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.h"

class NFIGameServerToWorldModule
	: public NFIModule
{

public:
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif