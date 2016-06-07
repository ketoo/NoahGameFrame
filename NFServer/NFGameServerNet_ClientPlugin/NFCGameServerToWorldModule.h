// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_GAMESERVER_NETCLIENT_MODULE_H
#define NFC_GAMESERVER_NETCLIENT_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClusterClientModule.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"

class NFCGameServerToWorldModule : public NFIGameServerToWorldModule
{
public:
    NFCGameServerToWorldModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();


    virtual void LogReceive(const char* str) {}
    virtual void LogSend(const char* str) {}

protected:

    void OnSocketWSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

protected:
    void Register(NFINet* pNet);
    void RefreshWorldInfo();

    void OnLoadRoleDataBeginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnLoadRoleDataFinalProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnEnquireSceneInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnSwapGSProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnAckCreateChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckJoinChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnAckQuitChatGroupProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


    int OnDataLoadBeginEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    //int OnToWorldEvent( const NFGUID& object, const int nEventID, const NFIDataList& var );

    int OnSwapGSEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

    int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);


    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    //     template<class PBClass>
    //     int TransPBToProxy(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void TransPBToProxy(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	virtual void SendBySuit(const int& nHashKey, const int nMsgID, const char* msg, const uint32_t nLen);

    virtual void LogServerInfo(const std::string& strServerInfo);

private:
    void SendOnline(const NFGUID& self);
    void SendOffline(const NFGUID& self);

    void CreateChatGroup(const NFGUID& self, const int nChatType, const std::string& strName);
    void JoinChatGroup(const NFGUID& self, const NFGUID& xGroup, const int nChatType);
    void QuitChatGroup(const NFGUID& self, const NFGUID& xGroup);
    void SubscriptionChatGroup(const NFGUID& self, const NFGUID& xGroup);
    void CancelSubscriptionChatGroup(const NFGUID& self, const NFGUID& xGroup);


private:

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIClusterClientModule* m_pClusterClientModule;
    NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif