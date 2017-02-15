// -------------------------------------------------------------------------
//    @FileName			:    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVER_SERVER_MODULE_H
#define NFC_PROXYSERVER_SERVER_MODULE_H

#include "NFComm/NFCore/NFCConsistentHash.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

class NFCProxyServerNet_ServerModule : public NFIProxyServerNet_ServerModule
{
public:
    NFCProxyServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual int Transpond(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID);

protected:

    void OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void OnClientDisconnect(const int nAddress);
    void OnClientConnected(const int nAddress);

    void OnConnectKeyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqServerListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqCreateRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqDelRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqEnterGameServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////

	void OnOtherMessage(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
protected:

    NFMapEx<NFGUID, int> mxClientIdent;
protected:
    NFINetClientModule* m_pNetClientModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
	NFINetModule* m_pNetModule;
	NFIProxyServerToWorldModule* m_pProxyToWorldModule;
};

#endif