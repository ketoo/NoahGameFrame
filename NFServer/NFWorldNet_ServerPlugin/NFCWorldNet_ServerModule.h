// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_WORLDNET_SERVER_MODULE_H
#define NFC_WORLDNET_SERVER_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"

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

    virtual void LogRecive(const char* str) {}
    virtual void LogSend(const char* str) {}

    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer = NFGUID());
    virtual bool SendMsgToGame(const NFIDataList& argObjectVar, const NFIDataList& argGameID,  const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData);
    virtual bool SendMsgToPlayer(const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFGUID nPlayer);

    virtual int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar);
    virtual int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar);
    virtual int OnPropertyEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self);
    virtual int OnRecordEnter(const NFIDataList& argVar, const NFIDataList& argGameID, const NFGUID& self);
    virtual bool OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase);

    virtual NF_SHARE_PTR<ServerData> GetSuitProxyForEnter();

    virtual int GetPlayerGameID(const NFGUID self);

protected:

    void OnRecivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
    void OnClientDisconnect(const int nAddress);
    //������
    void OnClientConnected(const int nAddress);



protected:

    int OnGameServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnGameServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnRefreshGameServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    int OnProxyServerRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnProxyServerUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnRefreshProxyServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    int OnLeaveGameProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    //////////////////////////////////////////////////////////////////////////

    void SynGameToProxy();
    void SynGameToProxy(const int nFD);

    //////////////////////////////////////////////////////////////////////////
    void LogGameServer();

protected:

    void OnOnlineProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnOfflineProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


private:

    NFINT64 mnLastCheckTime;

    //serverid,data
    NFMapEx<int, ServerData> mGameMap;
    NFMapEx<int, ServerData> mProxyMap;

    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIWorldLogicModule* m_pWorldLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIClusterModule* m_pClusterSQLModule;
    NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;

};

#endif
