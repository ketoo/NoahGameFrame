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
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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

	int OnRequireUseItemEvent( const NFGUID& self, const int nEventID, const NFIDataList& var );
	int OnRequireUseItemPosEvent( const NFGUID& self, const int nEventID, const NFIDataList& var );

	virtual bool CheckConfig();

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
};


#endif
