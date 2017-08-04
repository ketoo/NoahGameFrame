// -------------------------------------------------------------------------
//    @FileName			:    NFCWorldNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_WORLDNET_SERVER_MODULE_H
#define NFC_WORLDNET_SERVER_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"

class NFCWorldNet_ServerModule
    : public NFIWorldNet_ServerModule
{
public:
    NFCWorldNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
        mnLastCheckTime = pPluginManager->GetNowTime();
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer = NFGUID());
    virtual bool SendMsgToGame(const NFDataList& argObjectVar, const NFDataList& argGameID,  const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData);
    virtual bool SendMsgToPlayer(const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer);

    virtual int OnObjectListEnter(const NFDataList& self, const NFDataList& argVar);
    virtual int OnObjectListLeave(const NFDataList& self, const NFDataList& argVar);
    virtual int OnPropertyEnter(const NFDataList& argVar, const NFDataList& argGameID, const NFGUID& self);
    virtual int OnRecordEnter(const NFDataList& argVar, const NFDataList& argGameID, const NFGUID& self);
    virtual bool OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase);

    virtual NF_SHARE_PTR<ServerData> GetSuitProxyForEnter();

    virtual int GetPlayerGameID(const NFGUID self);

protected:

    void OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    
    void OnClientDisconnect(const NFSOCK nAddress);
    
    void OnClientConnected(const NFSOCK nAddress);



protected:

    void OnGameServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnGameServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshGameServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnAIServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAIServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRefreshAIServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    int OnLeaveGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    //////////////////////////////////////////////////////////////////////////

    void SynGameToProxy();
    void SynGameToProxy(const NFSOCK nFD);

    //////////////////////////////////////////////////////////////////////////
    void LogGameServer();

protected:

    void OnOnlineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnOfflineProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnTranspondServerReport(const NFSOCK nFd, const int msgId, const char* buffer, const uint32_t nLen);
	void ServerReport(int reportServerId, NFMsg::EServerState serverStatus);

private:

    NFINT64 mnLastCheckTime;

    //serverid,data
    NFMapEx<int, ServerData> mGameMap;
    NFMapEx<int, ServerData> mProxyMap;

    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFINetClientModule* m_pNetClientModule;

};

#endif
