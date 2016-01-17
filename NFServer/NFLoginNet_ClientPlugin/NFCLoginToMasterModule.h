// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LOGINNET_CLIENT_MODULE_H
#define NFC_LOGINNET_CLIENT_MODULE_H

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILoginToMasterModule.h"

class NFCLoginToMasterModule
    : public NFILoginToMasterModule
{
public:
    NFCLoginToMasterModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
	virtual bool BeforeShut();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

    virtual NFMapEx<int, NFMsg::ServerInfoReport>& GetWorldMap();

protected:
	void OnReciveMSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnSocketMSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	
protected:

    //////////////////////////////////////////////////////////////////////////

    int OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);


	//////////////////////////////////////////////////////////////////////////

	int OnWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    //////////////////////////////////////////////////////////////////////////
    void Register(NFINet* pNet);

private:
    NFMapEx<int, NFMsg::ServerInfoReport> mWorldMap;

	NFILoginLogicModule* m_pLoginLogicModule;
	NFILoginNet_ServerModule* m_pLoginNet_ServerModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
};

#endif