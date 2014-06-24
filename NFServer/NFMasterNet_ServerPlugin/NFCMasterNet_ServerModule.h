// -------------------------------------------------------------------------
//    @FileName      :    NFCMasterNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_MASTERNET_SERVER_MODULE_H_
#define _NFC_MASTERNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCMasterNet_ServerModule
	: public NFINetModule
{
public:
	NFCMasterNet_ServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute(const float fLasFrametime, const float fStartedTime);

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

	//////////////////////////////////////////////////////////////////////////
	int GetWorldObject(const int nWorldID, NFIValueList& varObjectList);
	int GetLoginObject(const int nLoginID, NFIValueList& varObjectList);

	//世界服务器注册，刷新信息
	int OnWorldRegisteredProcess(const NFIPacket& msg);
	int OnWorldUnRegisteredProcess(const NFIPacket& msg);
	int OnRefreshWorldInfoProcess(const NFIPacket& msg);

	//////////////////////////////////////////////////////////////////////////
	//登录服务器注册，刷新信息
	int OnLoginRegisteredProcess(const NFIPacket& msg);
	int OnLoginUnRegisteredProcess(const NFIPacket& msg);
	int OnRefreshLoginInfoProcess(const NFIPacket& msg);

	//选择世界服务器消息
	int OnSelectWorldProcess(const NFIPacket& msg);
	int OnSelectServerResultProcess(const NFIPacket& msg);

	//////////////////////////////////////////////////////////////////////////

	int OnWorldServerObjectEvent(const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
	int OnWorldServerObjectPropertyEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar);
    //int OnTestEvent(const NFIDENTID& self, const int nEventID, const NFIValueList& var);
	void SynWorldToLogin();

private:
	//world server container:-2
	int mnWorldContainerID;

	//login server container:-1
	int mnLoginContainerID;


	
	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIEventProcessModule* m_pEventProcessModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
};

#endif