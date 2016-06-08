// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
//     @Changed         : NFCProxyConnectObject Add HeartBeat by hahaya 2013-11-1
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVER_NETCLIENT_MODULE_H
#define NFC_PROXYSERVER_NETCLIENT_MODULE_H

#include <string>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIClusterClientModule.hpp"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"

class NFCProxyServerToWorldModule : public NFIProxyServerToWorldModule
{
public:

    NFCProxyServerToWorldModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

	virtual NFIClusterClientModule* GetClusterModule();
    virtual bool VerifyConnectData(const std::string& strAccount, const std::string& strKey);

protected:

    void OnSocketWSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void Register(NFINet* pNet);

    void OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void LogServerInfo(const std::string& strServerInfo);

	void OnOtherMessage(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
private:
    struct ClientConnectData
    {
        ClientConnectData()
        {
            strAccount = "";
            strConnectKey = "";
        }

        std::string strAccount;
        std::string strConnectKey;
    };


    NFMapEx<std::string, ClientConnectData> mWantToConnectMap;

private:

    NFILogModule* m_pLogModule;
    NFIProxyLogicModule* m_pProxyLogicModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIProxyServerToGameModule* m_pProxyServerToGameModule;
	NFIClusterClientModule* m_pClusterClientModule;

};

#endif