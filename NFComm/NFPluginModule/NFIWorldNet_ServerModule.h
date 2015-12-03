// -------------------------------------------------------------------------
//    @FileName         ��    NFIWorldNet_ServerModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2012-12-15
//    @Module           ��    NFIWorldNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLDNET_SERVERMODULE_H_
#define _NFI_WORLDNET_SERVERMODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFIWorldNet_ServerModule
    : public NFINetModule
{

public:
    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer = NFIDENTID()) = 0;
    virtual int OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar ) = 0;
    virtual int OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar ) = 0;
    virtual int OnPropertyEnter( const NFIDataList& argVar, const NFIDataList& argGameID, const NFIDENTID& self ) = 0;
    virtual int OnRecordEnter( const NFIDataList& argVar,const NFIDataList& argGameID, const NFIDENTID& self ) = 0;
    virtual bool SendMsgToGame( const NFIDataList& argObjectVar, const NFIDataList& argGameID,  const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData) = 0;
};

#endif