// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_WORLDNET_SERVER_MODULE_H_
#define _NFC_WORLDNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include <memory>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIWorldChatGroupModule.h"

class NFCWorldNet_ServerModule
    : public NFIWorldNet_ServerModule
{
public:
    NFCWorldNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
		mfLastLogTime = 0;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

    virtual bool SendMsgToGame(const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer = NFIDENTID());
    virtual bool SendMsgToGame( const NFIDataList& argObjectVar, const NFIDataList& argGameID,  const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData);
    virtual bool SendMsgToPlayer( const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer);

    virtual int OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar );
    virtual int OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar );
    virtual int OnPropertyEnter( const NFIDataList& argVar, const NFIDataList& argGameID, const NFIDENTID& self );
    virtual int OnRecordEnter( const NFIDataList& argVar,const NFIDataList& argGameID, const NFIDENTID& self );

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const int nAddress);
	//������
	void OnClientConnected(const int nAddress);

    bool OnRecordEnterPack(NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase* pObjectRecordBase);


protected:
    int OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

protected:

	bool InThisWorld(const std::string& strAccount);

    int OnGameServerRegisteredProcess(const NFIPacket& msg);
    int OnGameServerUnRegisteredProcess(const NFIPacket& msg);
    int OnRefreshGameServerInfoProcess(const NFIPacket& msg);

    int OnProxyServerRegisteredProcess(const NFIPacket& msg);
    int OnProxyServerUnRegisteredProcess(const NFIPacket& msg);
    int OnRefreshProxyServerInfoProcess(const NFIPacket& msg);

    int OnLeaveGameProcess(const NFIPacket& msg);
    //////////////////////////////////////////////////////////////////////////

	void SynGameToProxy();
	void SynGameToProxy( const int nFD );

	//////////////////////////////////////////////////////////////////////////
	void LogGameServer(const float fLastTime);

protected:
	void OnCreateGuildProcess(const NFIPacket& msg);
	void OnJoinGuildProcess(const NFIPacket& msg);
	void OnLeaveGuildProcess(const NFIPacket& msg);
	void OnOprGuildMemberProcess(const NFIPacket& msg);
    void OnSearchGuildProcess(const NFIPacket& msg);
    void OnOnlineProcess(const NFIPacket& msg);
    void OnOfflineProcess(const NFIPacket& msg);
    void OnChatProcess(const NFIPacket& msg);
    
    void OnReqCreateChatGroupProcess(const NFIPacket& msg);
    void OnReqJoineChatGroupProcess(const NFIPacket& msg);
    void OnReqLeaveChatGroupProcess(const NFIPacket& msg);
    void OnReqSubscriptionChatGroupProcess(const NFIPacket& msg);
    void OnReqCancelSubscriptionChatGroupProcess(const NFIPacket& msg);

private:

    struct ServerData
    {
        ServerData()
        {
            pData = NF_SHARE_PTR<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
            nFD = 0;
        }
        ~ServerData()
        {
            nFD = 0;
            pData = NULL;
        }

        int nFD;
        NF_SHARE_PTR<NFMsg::ServerInfoReport> pData;
    };

private:

	float mfLastLogTime;

    //serverid,data
    NFMapEx<int, ServerData> mGameMap;
    NFMapEx<int, ServerData> mProxyMap;

	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
    NFIWorldLogicModule* m_pWorldLogicModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIEventProcessModule* m_pEventProcessModule;
	NFIWorldGuildModule* m_pWorldGuildModule;
    NFIClusterModule* m_pClusterSQLModule;
    NFIWorldGuildDataModule* m_pWorldGuildDataModule;
    NFIWorldChatGroupModule* m_pWordChatGroupModule;
};

#endif
