// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_WORLDNET_CLIENT_MODULE_H
#define NFC_WORLDNET_CLIENT_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"

class NFCWorldToMasterModule
	: public NFIWorldToMasterModule
{
public:
	NFCWorldToMasterModule(NFIPluginManager* p)
	{
		pPluginManager = p;
		mLastReportTime = 0;
	}

	virtual bool Init();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();
	virtual void AddServerInfoExt(const std::string& key, const std::string& value);
protected:

	void OnSocketMSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	
	void OnClientDisconnect(const int nAddress);
	
	void OnClientConnected(const int nAddress);

	virtual void LogServerInfo(const std::string& strServerInfo);


	void Register(NFINet* pNet);
	void ServerReport();
	void RefreshWorldInfo();

	void OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnKickClientProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void InvalidMessage(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
private:
	NFINT64 mLastReportTime;

	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIClassModule* m_pClassModule;
	NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
	NFINetClientModule* m_pNetClientModule;
	NFINetModule* m_pNetModule;
	std::map<std::string, std::string> m_mServerInfoExt;
};

#endif