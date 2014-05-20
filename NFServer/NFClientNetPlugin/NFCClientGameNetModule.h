// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_CLIENTGAMENET_MODULE_H_
#define _NFC_CLIENTGAMENET_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFSvrMsgType.h"
#include "NFComm/NFMessageDefine/NFClientMsgType.h"
#include "NFComm/NFPluginModule/NFIClientNetModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFIEventProcessModule;

class NFCClientGameNetModule
    : public NFILogicModule,
      NFCNet
{
public:
    NFCClientGameNetModule();
    virtual ~NFCClientGameNetModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

    virtual  bool DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress);
    virtual  bool DispatcherPacket(NFIPacket* pMsgPacket);
    virtual  bool DispatcherPacket(RakNet::Packet* pPacket, const std::string& strMsg)
    {
        return true;
    }
protected:
    void SendConnectKey();

protected:

    static int OnSelectServerResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnCreateRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnDeleteRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnToWorldEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnLeaveEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

protected:
    int OnSelectRoleResultsProcess(NFIPacket* pMsgPacket);

protected:
    int OnGeneralProcess(NFIPacket* pMsgPacket);

    int OnContainProcess(NFIPacket* pMsgPacket);
    //对象创建消息
    int OnObjectSpringProcess(NFIPacket* pMsgPacket);
    //对象创建消息
    int OnObjectEndSpringProcess(NFIPacket* pMsgPacket);

    //对象属性变化消息
    int OnObjectPropertyIntProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyFloatProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyDoubleProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyStringProcess(NFIPacket* pMsgPacket);
    int OnObjectPropertyObjectProcess(NFIPacket* pMsgPacket);

    //对象属性变化消息
    int OnObjectTableAddProcess(NFIPacket* pMsgPacket);
    int OnObjectTableRemoveProcess(NFIPacket* pMsgPacket);

    int OnObjectTableIntProcess(NFIPacket* pMsgPacket);
    int OnObjectTableFloatProcess(NFIPacket* pMsgPacket);
    int OnObjectTableDoubleProcess(NFIPacket* pMsgPacket);
    int OnObjectTableStringProcess(NFIPacket* pMsgPacket);
    int OnObjectTableObjectProcess(NFIPacket* pMsgPacket);
private:

    static int mnGameConnectContainer;
    static int mnGameRoleHallContainer;

    std::string mstrAccount;
    std::string mstrWorldAddress;
    int mnPort;
    std::string mstrWorldKey;

    static NFIKernelModule* m_pKernelModule;
    static NFIEventProcessModule* m_pEventProcessModule;
    static NFCClientGameNetModule* m_pThis;

};

#endif