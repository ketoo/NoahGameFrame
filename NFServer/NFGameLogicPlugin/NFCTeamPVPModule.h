// -------------------------------------------------------------------------
//    @FileName         :    NFCTeamPVPModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-06
//    @Module           :    NFCTeamPVPModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEAM_PVP_MODULE_H
#define NFC_TEAM_PVP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIPVPRedisModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFITeamModule.h"
#include "NFComm/NFPluginModule/NFITeamPVPModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGSSwichServerModule.h"

class NFCTeamPVPModule
    : public NFITeamPVPModule
{
public:
	NFCTeamPVPModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFCTeamPVPModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	virtual bool AfterInit();

protected:

private:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetClientModule* m_pNetClientModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIGSSwichServerModule* m_pGSSwitchServerModule;
};

#endif
