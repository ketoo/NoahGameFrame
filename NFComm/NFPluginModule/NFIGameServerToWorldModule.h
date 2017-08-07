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
	virtual void TransmitToWorld(const int nHashKey, const int nMsgID, const google::protobuf::Message& xData) = 0;

};

#endif