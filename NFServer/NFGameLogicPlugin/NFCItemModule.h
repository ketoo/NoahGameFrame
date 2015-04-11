// -------------------------------------------------------------------------
//    @FileName      :    NFCMoveModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-06-11
//    @Module           :    NFCMoveModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_ITEM_MODULE_H_
#define _NFC_ITEM_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFCPackModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

protected:
    /**/int OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    /**/int OnRequireUseItemEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );
    /**/int OnRequireUseItemPosEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var );

	virtual bool CheckConfig();
private:
    char* mstrPackTableName;

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIItemConsumeManagerModule* m_pItemConsumeManagerModule;
    NFIPackModule* m_pPackModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
};


#endif
