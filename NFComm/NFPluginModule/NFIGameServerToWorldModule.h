// -------------------------------------------------------------------------
//    @FileName         £º    NFIGameServerToWorldModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIGameServerToWorldModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GAMESERVERTOWORLDMODULE_H
#define NFI_GAMESERVERTOWORLDMODULE_H

#include <iostream>
#include "NFILogicModule.h"

class NFIGameServerToWorldModule
    : public NFILogicModule
{

public:
	virtual void SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen) = 0;
};

#endif