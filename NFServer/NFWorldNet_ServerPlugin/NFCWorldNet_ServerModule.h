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
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include <memory>

class NFCWorldNet_ServerModule
    : public NFIWorldNet_ServerModule
{
public:
    NFCWorldNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);

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
    //serverid,data
    NFMapEx<int, ServerData> mGameMap;
    NFMapEx<int, ServerData> mProxyMap;

	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
    NFIWorldLogicModule* m_pWorldLogicModule;
	NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIEventProcessModule* m_pEventProcessModule;
};

#endif
