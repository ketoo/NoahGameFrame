// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_CHAT_GROUP_BROADCAST_MODULE_H_
#define _NFC_CHAT_GROUP_BROADCAST_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

protected:
    //¹«»á
    int OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar);
    int OnRecordCommonEvent( const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar );

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIWorldNet_ServerModule* m_pWorldNet_ServerModule;
    NFIWorldChatGroupModule* m_pWorldChatGroupModule;

private:
    std::string mstrGroupTalble;
};

#endif