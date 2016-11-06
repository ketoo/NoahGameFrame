// -------------------------------------------------------------------------
//    @FileName      :    NFIGuildBroadcastModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFIGuildBroadcastModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_BROADCAST_MODULE_H
#define NFC_GUILD_BROADCAST_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildBroadcastModule.h"
#include "NFComm/NFPluginModule/NFIGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIGuildModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCGuildBroadcastModule
    : public NFIGuildBroadcastModule
{
public:
    NFCGuildBroadcastModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
    //¹«»á
    int OnPropertyCommonEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );

protected:
    NFIKernelModule* m_pKernelModule;
    NFIGuildModule* m_pGuildModule;
    NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif