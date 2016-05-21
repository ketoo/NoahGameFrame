// -------------------------------------------------------------------------
//    @FileName         :   NFCTeamModule.h
//    @Author           :   flyicegood
//    @Date             :   2016/03/07
//    @Module           :   NFCTeamModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEAM_MODULE_H
#define NFC_TEAM_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFITileModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCTeamModule
    : public NFITeamModule
{
public:
    NFCTeamModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCTeamModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
	virtual bool AfterInit();
	virtual bool CreateTeam(const NFGUID& self, const std::string& strName, NFGUID& xTeam);
	virtual bool JoinTeam(const NFGUID& self, const std::string& strName, NFGUID& xTeam);
	
protected:
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eEvent, const NFIDataList& args);

protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;

	NFMap<NFGUID, NFCDataList> mxTeamList;

};

#endif // !NFC_TILE_MODULE_H
