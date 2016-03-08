// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFC_CHAT_GROUP_BROADCAST_MODULE_H
#define NFC_CHAT_GROUP_BROADCAST_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIChatGroupBroadcastModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldChatGroupModule.h"

class NFCChatGroupBroadcastModule
    : public NFIChatGroupBroadcastModule
{
public:
    NFCChatGroupBroadcastModule(NFIPluginManager* p)
    {
        pPluginManager = p;
        mstrGroupTalble = "ChatGroup";
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
    //¹«»á
    int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
    int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

protected:
    NFIKernelModule* m_pKernelModule;
    NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
    NFIWorldChatGroupModule* m_pWorldChatGroupModule;

private:
    std::string mstrGroupTalble;
};

#endif