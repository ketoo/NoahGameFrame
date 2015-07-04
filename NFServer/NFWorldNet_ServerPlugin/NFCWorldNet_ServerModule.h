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
    virtual bool GetGameID(const NFIDENTID& self, int& nGameID);

    virtual bool SendMsgToPlayer( const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer);
protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);

    int OnRecordCommonEvent( const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar );
    int OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar);

protected:

    int OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);
    int OnShowStringEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);
    int OnShowRecordEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);
    int OnShowPropertyEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

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

    void ShowString(const NFIDENTID& self, const int nResultID);

    void SendPropertyToPlayer(const NFIDENTID& self, const NFIDENTID& xPlayer);
    void SendRecordToPlayer(const NFIDENTID& self, const NFIDENTID& xPlayer, const std::string& strRecordName, const int nRow = -1);

protected:

	void OnCrateGuildProcess(const NFIPacket& msg);
	void OnJoinGuildProcess(const NFIPacket& msg);
	void OnLeaveGuildProcess(const NFIPacket& msg);
	void OnOprGuildMemberProcess(const NFIPacket& msg);
    void OnSearchGuildProcess(const NFIPacket& msg);
    void OnOnline(const NFIPacket& msg);
    void OnOffline(const NFIPacket& msg);
    int OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar );
    int OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar );
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
};

#endif
