// -------------------------------------------------------------------------
//    @FileName      :    NFITeamBroadcastModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFITeamBroadcastModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TEAM_BROADCAST_MODULE_H
#define NFC_TEAM_BROADCAST_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFITeamBroadcastModule.h"
#include "NFComm/NFPluginModule/NFITeamModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"

class NFCTeamBroadcastModule
    : public NFITeamBroadcastModule
{
public:
    NFCTeamBroadcastModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:    
    int OnPropertyCommonEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );

protected:
    NFIKernelModule* m_pKernelModule;
    NFITeamModule* m_pTeamModule;
	NFIWorldNet_ServerModule* m_pWorlNet_ServerModule;
};

#endif