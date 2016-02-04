// -------------------------------------------------------------------------
//    @FileName      :    NFCGuildEctypeModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCGuildEctypeModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILDECTYPE_MODULE_H
#define NFC_GUILDECTYPE_MODULE_H

#include "NFComm/NFPluginModule/NFIGuildEctypeModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"

class NFCGuildEctypeModule
    : public NFIGuildEctypeModule
{
public:
	struct GuildEctypeInfo
	{
		GuildEctypeInfo()
		{
			nEctypeID = 0;
			nGroupID = 0;
		}

		int nEctypeID;
		int nGroupID;
	};
public:
    NFCGuildEctypeModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGuildEctypeModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool CreateGuilEctype(const NFGUID& self, const NFGUID& guild);
    virtual bool ApplyEnterGuilEctype(const NFGUID& self, const NFGUID& guild);

private:
	NFISceneProcessModule* m_pSceneProcessModule;
	NFIGameServerToWorldModule* m_pGameServerToWorldModule;
	NFICommonConfigModule* m_pCommonConfigModule;

	NFMapEx<NFGUID, GuildEctypeInfo> mmGuilEctype;
	NFIKernelModule* m_pKernelModule;
};


#endif
