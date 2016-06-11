// -------------------------------------------------------------------------
//    @FileName      :    NFCMoveModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-06-11
//    @Module           :    NFCMoveModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ITEM_MODULE_H
#define NFC_ITEM_MODULE_H

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

class NFCItemModule
    : public NFIItemModule
{
public:

    NFCItemModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCItemModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

protected:
	int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

	int OnRequireUseItemPosEvent( const NFGUID& self, const int nEventID, const NFIDataList& var );

	virtual bool CheckConfig();

    int AddItemEffectDataProperty( const NFGUID& self, const NFGUID& xTarget, const std::string& strItemID);
    bool ConsumeDataItemProperty( const NFGUID& self, const std::string& strID);


	bool DoAwardPack( const NFGUID& self, const std::string& strAwardPack);

protected:
	void OnClienUseItem(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

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
};


#endif
