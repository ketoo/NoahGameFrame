// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_CLIENTLOGINNET_MODULE_H_
#define _NFC_CLIENTLOGINNET_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFSvrMsgType.h"
#include "NFComm/NFMessageDefine/NFClientMsgType.h"
#include "NFComm/NFPluginModule/NFIClientNetModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFIEventProcessModule;

class NFCClientLoginNetModule
    : public NFIClientNetModule,
      NFCNet
{
public:
    NFCClientLoginNetModule();
    virtual ~NFCClientLoginNetModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual  bool DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress);
    virtual  bool DispatcherPacket(NFIPacket* pMsgPacket);
    virtual  bool DispatcherPacket(RakNet::Packet* pPacket, const std::string& strMsg)
    {
        return true;
    }
    virtual bool AfterInit();

    virtual bool Disconnected(const std::string& strAccount);


protected:

    // client event
    static int OnLoginEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnLogOutEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnDisconnectEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    //////////////////////////////////////////////////////////////////////////

    int OnSelectWorldResultProcess(NFIPacket* pMsgPacket);


    //int OnContainProcess( NFIPacket* pMsgPacket );
    //对象创建消息
    int OnObjectProcess(NFIPacket* pMsgPacket);
    //对象属性变化消息
    int OnObjectPropertyIntProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyFloatProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyDoubleProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyStringProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyObjectProcess(NFIPacket* pMsgPacket);


private:
    static int mnClientConnectContainer;

    static NFIKernelModule* m_pKernelModule;
    static NFIEventProcessModule* m_pEventProcessModule;
    static NFCClientLoginNetModule* m_pThis;
};

#endif