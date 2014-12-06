// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_LOGINNET_SERVER_MODULE_H_
#define _NFC_LOGINNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFILoginNet_ClientModule.h"

#define NET_MSG_PROCESS(xNFMsg, msg) \
    NFIDENTID nPlayerID; \
    xNFMsg xMsg; \
    if (!RecivePB(msg, xMsg, nPlayerID)) \
    { \
    return 0; \
    } \
    \
    NFIActorMessage xActorMsg; \
    xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG; \
    xActorMsg.nSubMsgID = msg.GetMsgHead()->GetMsgID(); \
    xMsg.SerializeToString(&xActorMsg.data); \
    pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());

class NFCLoginNet_ServerModule
	: public NFILoginNet_ServerModule

{
public:
	NFCLoginNet_ServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute(const float fLasFrametime, const float fStartedTime);


	virtual bool BeforeShut();
	virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

protected:
	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

protected:
	//连接丢失,删2层(连接对象，帐号对象)
	void OnClientDisconnect(const int nAddress);
	//有连接
	void OnClientConnected(const int nAddress);

	//登入 
	int OnLoginProcess(const NFIPacket& msg);
    
	//选择大世界
	int OnSelectWorldProcess(const NFIPacket& msg);
    
	//申请查看世界列表
	int OnViewWorldProcess(const NFIPacket& msg);

    //////////////////////////////////////////////////////////////////////////

    //选择大世界结果(发下key等给客户端)
    int OnSelectWorldResultsEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);

    //登入结果
    int OnLoginResultsEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var);


protected:

protected:
	void SynWorldToClient(const int nFD);

private:

    NFILoginNet_ClientModule* m_pLoginNet_ClientModule;
    NFIEventProcessModule* m_pEventProcessModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIElementInfoModule* m_pElementInfoModule;	
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFILoginLogicModule* m_pLoginLogicModule;
};

#endif