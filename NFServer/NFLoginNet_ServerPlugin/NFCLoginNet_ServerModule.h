// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LOGINNET_SERVER_MODULE_H
#define NFC_LOGINNET_SERVER_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFILoginToMasterModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"

#define NET_MSG_PROCESS(xNFMsg, msg) \
    NFGUID nPlayerID; \
    xNFMsg xMsg; \
    if (!RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID)) \
    { \
    return 0; \
    } \
    \
    NFIActorMessage xActorMsg; \
    xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG; \
    xActorMsg.nSubMsgID = nMsgID; \
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
	virtual bool Execute();


	virtual bool BeforeShut();
	virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

	virtual int OnSelectWorldResultsProcess(const int nWorldID, const NFGUID xSenderID, const int nLoginID, const std::string& strAccount, const std::string& strWorldIP, const int nWorldPort, const std::string& strKey);

protected:
	void OnReciveClientPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

protected:
	void OnClientDisconnect(const int nAddress);
	void OnClientConnected(const int nAddress);

	//登入 
	int OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    
	//选择大世界
	int OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    
	//申请查看世界列表
	int OnViewWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);



protected:

protected:
	void SynWorldToClient(const int nFD);

	NFMapEx<NFGUID, int> mxClientIdent;

private:

    NFILoginToMasterModule* m_pLoginToMasterModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIElementInfoModule* m_pElementInfoModule;	
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFILoginLogicModule* m_pLoginLogicModule;
	NFIUUIDModule* m_pUUIDModule;
};

#endif