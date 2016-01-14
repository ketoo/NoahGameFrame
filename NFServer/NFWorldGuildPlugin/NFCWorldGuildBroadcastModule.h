// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_GUILD_BROADCAST_MODULE_H
#define NFC_WORLD_GUILD_BROADCAST_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildBroadcastModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFCWorldGuildBroadcastModule
    : public NFIWorldGuildBroadcastModule
{
public:
    NFCWorldGuildBroadcastModule(NFIPluginManager* p)
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
    int OnRecordCommonEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar );

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIWorldGuildDataModule* m_pWorldGuildDataModule;
    NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
    NFIWorldGuildModule* m_pWorldGuildModule;
};

#endif