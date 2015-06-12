// -------------------------------------------------------------------------
//    @FileName         £º    NFIWorldNet_ServerModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIWorldNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLDNET_SERVERMODULE_H_
#define _NFI_WORLDNET_SERVERMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFNet/NFIPacket.h"

class NFIWorldNet_ServerModule
    : public NFINetModule
{

public:
    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer = NFIDENTID()) = 0;
    virtual bool SendMsgToProxy(const int nGateID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer = NFIDENTID()) = 0;
    virtual bool GetGameID(const NFIDENTID& self, int& nGameID) = 0;
    virtual bool GetGateID(const NFIDENTID& self, int& nGateID) = 0;
};

#endif