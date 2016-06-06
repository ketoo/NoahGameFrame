// -------------------------------------------------------------------------
//    @FileName         £º    NFIGameServerNet_ServerModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIGameServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GAMESERVERNET_SERVERMODULE_H
#define NFI_GAMESERVERNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIGameServerNet_ServerModule
    : public NFILogicModule
{
public:
	virtual NFINetModule* GetNetModule() = 0;
	virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self) = 0;
	virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self) = 0;
};

#endif