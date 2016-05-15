// -------------------------------------------------------------------------
//    @FileName         ：    NFIGameServerNet_ServerModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIGameServerNet_ServerModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_GAMESERVERNET_SERVERMODULE_H
#define _NFI_GAMESERVERNET_SERVERMODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFINetModule.h"

class NFIGameServerNet_ServerModule
    : public NFINetModule
{

public:
    virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self) = 0;
    virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self) = 0;

    //把argVar这些人的出现或者离去广播给self这些人
    virtual int OnPropertyEnter( const NFIDataList& argVar, const NFGUID& self ) = 0;
    virtual int OnRecordEnter( const NFIDataList& argVar, const NFGUID& self ) = 0;
    virtual int OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar ) = 0;
    virtual int OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar ) = 0;
	virtual void ProcessSwitchToGame(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID, const int nSceneID) = 0;
	virtual void ProcessSwitchOffGame(const NFGUID& self) = 0;
	virtual bool GetGateInfo(const NFGUID& self, int & nGateID, NFGUID& xClientID) = 0;
};

#endif