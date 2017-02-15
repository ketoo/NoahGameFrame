// -------------------------------------------------------------------------
//    @FileName      	:   NFCItemConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFCItemConsumeManagerModule
// -------------------------------------------------------------------------

#ifndef NFC_ITEMCONSUME_MANAGER_MODULE_H
#define NFC_ITEMCONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCItemConsumeManagerModule
    : public NFIItemConsumeManagerModule,
      NFMapEx<int, NFIItemConsumeProcessModule>
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

    virtual bool ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFIItemConsumeProcessModule> pModule );

    virtual NF_SHARE_PTR<NFIItemConsumeProcessModule> GetConsumeModule( const int nModuleType );

};

#endif
