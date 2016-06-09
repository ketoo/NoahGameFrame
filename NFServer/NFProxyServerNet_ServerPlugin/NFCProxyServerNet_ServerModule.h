// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVER_SERVER_MODULE_H
#define NFC_PROXYSERVER_SERVER_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFCore/NFCConsistentHash.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIProxyServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIProxyServerToGameModule.h"
#include "NFComm/NFPluginModule/NFIClusterClientModule.hpp"

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

    //进入游戏成功
    virtual int EnterGameSuccessEvent(const NFGUID xClientID, const NFGUID xPlayerID);

protected:

    void OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

    void OnConnectKeyProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqServerListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqRoleListProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqCreateRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqDelRoleProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnReqEnterGameServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


    //客户端的连接60秒删掉
    int HB_OnConnectCheckTime(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var);
    //////////////////////////////////////////////////////////////////////////

	void OnOtherMessage(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
protected:

    NFMapEx<NFGUID, int> mxClientIdent;
    NFCConsistentHash mxConsistentHash;
protected:
    NFIProxyServerToWorldModule* m_pProxyToWorldModule;
    NFIProxyServerToGameModule* m_pProxyServerToGameModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIUUIDModule* m_pUUIDModule;
	NFINetModule* m_pNetModule;

};

#endif