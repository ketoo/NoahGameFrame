// -------------------------------------------------------------------------
//    @FileName      :   NFCBriefSkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBriefSkillConsumeProcessModule
//    @Desc             :   技能消费机制类,短刃类，当场扣除当场生效
// -------------------------------------------------------------------------

#ifndef _NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H_
#define _NFC_BRIEFSKILLCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"

class NFCBriefSkillConsumeProcessModule
    : public NFISkillConsumeProcessModule
{
public:
    NFCBriefSkillConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //物品使用是否合法
    virtual int ConsumeLegal( const NFIDENTID& self, const std::string& skillID,  const NFIValueList& other );

    //使用物品的消耗
    virtual int ConsumeSelf( const NFIDENTID& self, const std::string& skillID );

    //合法,消耗,那么处理过程[消耗后,nItemRowID已经找不到了，因为可能被清空了]
    virtual int ConsumeProcess( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList );
    virtual int ConsumeProcessEx( const NFIDENTID& self, const std::string& strSkillName, const NFIValueList& other, NFIValueList& damageListValue, NFIValueList& damageResultList );

private:
    char* mstrSkillTableName;

    NFIKernelModule* m_pKernelModule;
    NFISkillModule* m_pSkillModule;
    NFIPropertyModule* m_pPropertyModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementInfoModule* m_pElementInfoModule;
};

#endif
