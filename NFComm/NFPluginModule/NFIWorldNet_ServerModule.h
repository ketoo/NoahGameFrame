// -------------------------------------------------------------------------
//    @FileName         £º    NFIWorldNet_ServerModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIWorldNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef NFI_WORLDNET_SERVERMODULE_H
#define NFI_WORLDNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFIWorldNet_ServerModule
    : public NFINetModule
{

public:
    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer = NFGUID()) = 0;
    virtual int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar) = 0;
    virtual int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar) = 0;
    virtual int OnPropertyEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self) = 0;
    virtual int OnRecordEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self) = 0;
    virtual bool SendMsgToGame(const NFIDataList& argObjectVar, const NFIDataList& argGameID,  const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData) = 0;
    virtual bool OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase) = 0;

    virtual NF_SHARE_PTR<ServerData> GetSuitProxyForEnter() = 0;

};

#endif