// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_WORLDNET_CLIENT_MODULE_H
#define NFC_WORLDNET_CLIENT_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFCWorldToMasterModule
    : public NFIWorldToMasterModule
{
public:
    NFCWorldToMasterModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void LogRecive(const char* str) {}
    virtual void LogSend(const char* str) {}

protected:

    void OnReciveMSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSocketMSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);

    virtual void LogServerInfo(const std::string& strServerInfo);


    void Register(NFINet* pNet);
    void RefreshWorldInfo();

    int OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    int OnKickClientProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


private:

    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIWorldLogicModule* m_pWorldLogicModule;
    NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;

};

#endif