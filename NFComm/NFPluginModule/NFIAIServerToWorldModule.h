// -------------------------------------------------------------------------
//    @FileName			:    NFIAIServerToWorldModule.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFIAIServerToWorldModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFI_AISERVERTOWORLDMODULE_H
#define NFI_AISERVERTOWORLDMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetClientModule.h"

class NFIAIServerToWorldModule
	: public NFIModule
{

public:
	virtual void AddServerInfoExt(const std::string& key, const std::string& value) = 0;
};

#endif