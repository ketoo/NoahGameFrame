// -------------------------------------------------------------------------
//    @FileName      :   NFCItemConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCItemConsumeManagerModule
//    @Desc             :   道具消费机制管理类,所有类型的道具消费类型均需注册才能消费
// -------------------------------------------------------------------------

#ifndef NFC_ITEMCONSUME_MANAGER_MODULE_H
#define NFC_ITEMCONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCItemConsumeManagerModule
    : public NFIItemConsumeManagerModule,
      NFMap<int, NFIItemConsumeProcessModule>
{
public:

    NFCItemConsumeManagerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool ResgisterConsumeModule( const int nModuleType, NFIItemConsumeProcessModule* pModule );

    virtual NFIItemConsumeProcessModule* GetConsumeModule( const int nModuleType );

};

#endif
