// -------------------------------------------------------------------------
//    @FileName      :   NFCSkillConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBuSkillConsumeManagerModule
//    @Desc             :   技能消费机制管理类,所有类型的技能消费类型均需注册才能消费
// -------------------------------------------------------------------------

#ifndef NFC_SKILLCONSUME_MANAGER_MODULE_H
#define NFC_SKILLCONSUME_MANAGER_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"

class NFCSkillConsumeManagerModule
    : public NFISkillConsumeManagerModule,
      NFMap<int, NFISkillConsumeProcessModule>
{
public:
    NFCSkillConsumeManagerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool ResgisterConsumeModule( const int nModuleType, NFISkillConsumeProcessModule* pModule );

    virtual NFISkillConsumeProcessModule* GetConsumeModule( const int nModuleType );

};

#endif
