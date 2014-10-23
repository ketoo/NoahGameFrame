// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_NETCLIENT_MODULE_H_
#define _NFC_GAMESERVER_NETCLIENT_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ClientModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCGameServerNet_ClientModule : public NFINetModule
{
public:
    NFCGameServerNet_ClientModule(NFIPluginManager* p)
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

protected:

    int OnRecivePack(const NFIPacket& msg);
    int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

    //连接丢失,删2层(连接对象，帐号对象)
    void OnClientDisconnect(const int nAddress);
    //有连接
    void OnClientConnected(const int nAddress);
protected:

    void Register();
    void UnRegister();
    void RefreshWorldInfo();

    int OnLoadRoleDataBeginProcess(const NFIPacket& msg);

    int OnLoadRoleDataFinalProcess(const NFIPacket& msg);

    int OnEnquireSceneInfoProcess(const NFIPacket& msg);

    int OnSwapGSProcess(const NFIPacket& msg);

    int OnDataLoadBeginEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

    //int OnToWorldEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var );

    int OnSwapGSEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

    int OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

private:
    int mnSocketFD;

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif