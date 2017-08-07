// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyServerToAIModule.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFCProxyServerToAIModule
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVER_TO_AI_MODULE_H
#define NFC_PROXYSERVER_TO_AI_MODULE_H

#include <string>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerToAIModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIProxyLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"


class NFCProxyServerToAIModule : public NFIProxyServerToAIModule
{
public:

	NFCProxyServerToAIModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    void OnSocketGSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void Register(NFINet* pNet);

    void OnAckEnterGame(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void LogServerInfo(const std::string& strServerInfo);

	void Transpond(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFIProxyServerNet_ServerModule* m_pProxyServerNet_ServerModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
	NFINetClientModule* m_pNetClientModule;

};

#endif