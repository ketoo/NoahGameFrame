// -------------------------------------------------------------------------
//    @FileName         ：    NFIGameServerNet_ServerModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFIGameServerNet_ServerModule
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
    //要管理当前所有的对象所在的actor,gateid,fd等
    struct GateBaseInfo
    {
        GateBaseInfo()
        {
            nActorID = 0;
            nGateID = 0;
        }

        GateBaseInfo(const int gateID, const NFGUID xIdent)
        {
            nActorID = 0;
            nGateID = gateID;
            xClientID = xIdent;
        }

        int nActorID;
        int nGateID;
        NFGUID xClientID;
    };

    struct GateServerInfo
    {
        ServerData xServerData;
        //此网关上所有的对象<角色ID,gate_FD>
        std::map<NFGUID, int> xRoleInfo;
    };

public:
	virtual NFINetModule* GetNetModule() = 0;
	virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self) = 0;
	virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self) = 0;
    virtual bool AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID) = 0;
    virtual bool RemovePlayerGateInfo(const NFGUID& nRoleID) = 0;
    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& nRoleID) = 0;
    virtual int OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self) = 0;
    virtual int OnRecordEnter(const NFIDataList& argVar, const NFGUID& self) = 0;
    virtual int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar) = 0;
    virtual int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar) = 0;
};

#endif