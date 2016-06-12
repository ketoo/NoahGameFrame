// -------------------------------------------------------------------------
//    @FileName      :    NFCChatModule.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2013-06-11
//    @Module           :    NFCChatModule
//
// -------------------------------------------------------------------------

#ifndef NFC_CHAT_MODULE_H
#define NFC_CHAT_MODULE_H

#include "NFCPackModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIHeroModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"

class NFCChatModule
    : public NFILogicModule
{
public:

    NFCChatModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCChatModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    void OnClienChatProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIKernelModule* m_pKernelModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementModule* m_pElementModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIHeroModule* m_pHeroModule;
	NFICommonConfigModule* m_pCommonConfigModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIGameServerToWorldModule* m_pGameServerToWorldModule;
};


#endif
