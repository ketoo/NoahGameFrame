// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
//     @Changed         : NFCProxyConnectObject Add HeartBeat by hahaya 2013-11-1
// -------------------------------------------------------------------------

#ifndef _NFC_PROXYSERVER_NETCLIENT_MODULE_H_
#define _NFC_PROXYSERVER_NETCLIENT_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFILogModule.h"


//这个连接是连的gameserver
class NFCProxyConnectObject :   public NFINetModule
{
public:
    NFCProxyConnectObject(int nGameServerID, NFIPluginManager* p);

    virtual bool Execute(float fFrameTime, float fTotalTime);
    virtual void LogRecive(const char* str){}
    virtual void LogSend(const char* str){}

protected:

    int OnRecivePack(const NFIPacket& msg);
    int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

private:

    int mnGameServerID;

    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
};

//策略是，开始先连到world，然后拿到gameserverlist，再连gameserver，因此这个连接是连到worldserver
class NFCProxyServerNet_ClientModule : public NFIProxyServerNet_ClientModule
{
public:

    NFCProxyServerNet_ClientModule(NFIPluginManager* p)
    {
        mnSocketFD = 0;
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

    virtual int Transpond(int nGameServerID, const NFIPacket& msg);
    virtual ConnectData* GetConnectData(const std::string& strAccount);
    virtual GameData* GetGameData(int nGameID);
    virtual GameDataMap& GetGameDataMap() { return mGameDataMap; }
protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);


    int DoHeartBeatEvent(const NFIDENTID& self, const NFIValueList& var);
protected:

    void Register();
    void UnRegister();

    int OnSelectServerResultProcess(const NFIPacket& msg);
    int OnGameInfoProcess(const NFIPacket& msg);



private:
    int mnSocketFD;
    //int mnWantToConnectContainer;
    //int mnGameContainerID;
    
    ConnectDataMap mWantToConnectMap;
    GameDataMap mGameDataMap;
private:

    NFILogModule* m_pLogModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
	NFIElementInfoModule* m_pElementInfoModule;
};

#endif