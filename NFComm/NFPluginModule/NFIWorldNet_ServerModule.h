// -------------------------------------------------------------------------
//    @FileName			:    NFIWorldNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIWorldNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_WORLDNET_SERVERMODULE_H
#define NFI_WORLDNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFIWorldNet_ServerModule
    : public NFIModule
{

public:
	virtual void OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) = 0;

	virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer = NFGUID()) = 0;
	virtual bool SendMsgToGame(const NFDataList& argObjectVar, const NFDataList& argGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData) = 0;
	virtual bool SendMsgToPlayer(const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer) = 0;

	virtual NF_SHARE_PTR<ServerData> GetSuitProxyForEnter() = 0;
};

#endif