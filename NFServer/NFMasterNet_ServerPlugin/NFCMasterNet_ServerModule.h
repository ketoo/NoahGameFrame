// -------------------------------------------------------------------------
//    @FileName      :    NFCMasterNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_MASTERNET_SERVER_MODULE_H
#define NFC_MASTERNET_SERVER_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCMasterNet_ServerModule
    : public NFIMasterNet_ServerModule
{
public:
    NFCMasterNet_ServerModule(NFIPluginManager* p)
    {
		pPluginManager = p;
        mnLastLogTime = pPluginManager->GetNowTime();
    }

    virtual bool Init();
    virtual bool Shut();

    virtual bool AfterInit();
    virtual bool Execute();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

protected:

    void OnReceivePack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

protected:
    //世界服务器注册，刷新信息
    int OnWorldRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnWorldUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnRefreshWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////
    //登录服务器注册，刷新信息
    int OnLoginRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnLoginUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnRefreshLoginInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //选择世界服务器消息
    int OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////

    void SynWorldToLogin();
    void LogGameServer();

private:

    NFINT64 mnLastLogTime;

    //serverid,data
    NFMapEx<int, ServerData> mWorldMap;
    NFMapEx<int, ServerData> mLoginMap;


    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
};

#endif