// -------------------------------------------------------------------------
//    @FileName			:    NFCAIServerNet_ClientModule.h
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFCAIServerNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_AISERVER_NETCLIENT_MODULE_H
#define NFC_AISERVER_NETCLIENT_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIAIServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIAIServerToWorldModule.h"

class NFCAIServerToWorldModule : public NFIAIServerToWorldModule
{
public:
	NFCAIServerToWorldModule(NFIPluginManager* p)
    {
        pPluginManager = p;
		mLastReportTime = 0;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:

    void OnSocketWSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

protected:
    void Register(NFINet* pNet);
	void ServerReport();
    void RefreshWorldInfo();
    void TransPBToProxy(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

private:
    void SendOnline(const NFGUID& self);
    void SendOffline(const NFGUID& self);

private:
	NFINT64 mLastReportTime;

    NFILogModule* m_pLogModule;
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFIElementModule* m_pElementModule;
	NFINetClientModule* m_pNetClientModule;
	NFIAIServerNet_ServerModule* m_pAIServerNet_ServerModule;

};

#endif