// -------------------------------------------------------------------------
//    @FileName      :    NFCGSSwichServerModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2013-07-10
//    @Module           :    NFCGSSwichServerModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SWITCHSERVER_MODULE_H
#define NFC_SWITCHSERVER_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGSSwichServerModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILevelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"

class NFCGSSwichServerModule
    : public NFIGSSwichServerModule
{
public:
    NFCGSSwichServerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGSSwichServerModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	virtual bool AfterInit();
	virtual bool ChangeServer(const NFGUID& self, const int nServer, const int nSceneID, const int nGroup);

	void OnClientReqSwichServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqSwichServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnAckSwichServer( const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen );

private:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
	NFINetClientModule* m_pNetClientModule;
	NFILevelModule* m_pLevelModule;
    NFIPackModule* m_pPackModule;
    NFIHeroModule* m_pHeroModule;
	NFIEventModule* m_pEventModule;
	NFISceneAOIModule* m_pScenemodule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
