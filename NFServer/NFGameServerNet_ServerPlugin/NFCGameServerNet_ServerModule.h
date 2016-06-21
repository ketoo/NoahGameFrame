// -------------------------------------------------------------------------
//    @FileName			:    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_GAMESERVER_SERVER_MODULE_H
#define NFC_GAMESERVER_SERVER_MODULE_H

//#include "GameServerPCH.h"
//#include "NW_Helper.h"
//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
////////////////////////////////////////////////////////////////////////////



class NFCGameServerNet_ServerModule
    : public NFIGameServerNet_ServerModule
{
public:
    NFCGameServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}
    virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self);
    virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self);
	virtual NFINetModule* GetNetModule();

    virtual bool AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID);
    virtual bool RemovePlayerGateInfo(const NFGUID& nRoleID);
    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& nRoleID);

    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int nGateID);
    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfoBySockIndex(const int nSockIndex);

    virtual int OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self);
    virtual int OnRecordEnter(const NFIDataList& argVar, const NFGUID& self);

    virtual int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar);
    virtual int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar);

protected:
    void OnSocketPSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
    void OnClientDisconnect(const int nSockIndex);
    void OnClientConnected(const int nSockIndex);

protected:
    void OnProxyServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnProxyServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshProxyServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    void OnReqiureRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnCreateRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnDeleteRoleGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienEnterGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienLeaveGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienSwapSceneProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


    ///////////WORLD_START///////////////////////////////////////////////////////////////
    void OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnTransWorld(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, const int nWorldKey);
	
protected:
    //网络同步
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int OnGroupEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnContainerEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
    int OnSwapSceneResultEvent(const NFGUID& self, const int nEventID, const NFIDataList& var);

    int GetBroadCastObject(const NFGUID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject);
    int GetBroadCastObject(const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject);

private:
    //<角色id,角色网关基础信息>//其实可以在object系统中被代替
    NFMapEx<NFGUID, GateBaseInfo> mRoleBaseData;
    //gateid,data
    NFMapEx<int, GateServerInfo> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
    NFIUUIDModule* m_pUUIDModule;
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
    //////////////////////////////////////////////////////////////////////////
    NFIGameServerToWorldModule* m_pGameServerToWorldModule;
};
#endif
