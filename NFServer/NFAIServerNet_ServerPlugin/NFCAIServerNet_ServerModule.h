// -------------------------------------------------------------------------
//    @FileName			:    NFCAIServerNet_ServerModule.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFCAIServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_AISERVER_SERVER_MODULE_H
#define NFC_AISERVER_SERVER_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIAIServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIAIServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIPlayerRedisModule.h"
////////////////////////////////////////////////////////////////////////////



class NFCAIServerNet_ServerModule
    : public NFIAIServerNet_ServerModule
{
public:
	NFCAIServerNet_ServerModule(NFIPluginManager* p)
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
	virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const int nSceneID, const int nGroupID);
	virtual void SendMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const int nSceneID, const int nGroupID);

    virtual bool AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID);
    virtual bool RemovePlayerGateInfo(const NFGUID& nRoleID);
    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& nRoleID);

    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int nGateID);
    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfoBySockIndex(const SockIndex nSockIndex);



protected:
    void OnSocketPSEvent(const SockIndex nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
    void OnClientDisconnect(const SockIndex nSockIndex);
    void OnClientConnected(const SockIndex nSockIndex);

protected:
    void OnProxyServerRegisteredProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnProxyServerUnRegisteredProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshProxyServerInfoProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    void OnReqiureRoleListProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnCreateRoleGameProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnDeleteRoleGameProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienEnterGameProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnClienLeaveGameProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClienSwapSceneProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClienReqMoveProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClienReqMoveImmuneProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClienEnterGameFinishProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    ///////////WORLD_START///////////////////////////////////////////////////////////////
    void OnTransWorld(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnTransWorld(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen, const int nWorldKey);

protected:
	void OnClientPropertyIntProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyFloatProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyStringProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyObjectProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyVector2Process(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyVector3Process(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnClientAddRowProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRemoveRowProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientSwapRowProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordIntProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordFloatProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordStringProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordObjectProcess(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordVector2Process(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordVector3Process(const SockIndex nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	//////////////////////////////////////////
	int OnSceneEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	//broad the data of argvar to self
	int OnObjectListEnter(const NFDataList& self, const NFDataList& argVar);
	int OnObjectDataFinished(const NFDataList& self, const NFDataList& argVar);
	int OnObjectListLeave(const NFDataList& self, const NFDataList& argVar);

	//broad the data of self to argvar 
	int OnPropertyEnter(const NFDataList& argVar, const NFGUID& self);
	int OnRecordEnter(const NFDataList& argVar, const NFGUID& self);

	int OnPropertyEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);
	int OnRecordEvent(const NFGUID& self, const std::string& strRecord, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);

private:
    
    NFMapEx<NFGUID, GateBaseInfo> mRoleBaseData;
    //gateid,data
    NFMapEx<int, GateServerInfo> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneAOIModule* m_pSceneAOIModule;
    //////////////////////////////////////////////////////////////////////////
    NFINetClientModule* m_pNetClientModule;
};
#endif
