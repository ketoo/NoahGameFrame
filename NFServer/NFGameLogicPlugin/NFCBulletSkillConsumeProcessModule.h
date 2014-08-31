// -------------------------------------------------------------------------
//    @FileName      :   NFCBulletSkillConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBulletSkillConsumeProcessModule
//    @Desc             :   技能消费机制类,子弹类，当场扣除，但是到达才生效
// -------------------------------------------------------------------------

#ifndef _NFC_BULLETSKILLCONSUME_PROCESS_MODULE_H_
#define _NFC_BULLETSKILLCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFISkillConsumeManagerModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeProcessModule.h"
#include "NFComm/NFPluginModule/NFIItemConsumeManagerModule.h"

class NFCBulletSkillConsumeProcessModule
    : public NFISkillConsumeProcessModule
{
public:
    NFCBulletSkillConsumeProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //物品使用是否合法
    virtual int ConsumeLegal( const NFIDENTID& self, const std::string& skillID,  const NFIDataList& other );

    //使用物品的消耗
    virtual int ConsumeSelf( const NFIDENTID& self, const std::string& skillID );

    virtual int ConsumeProcess( const NFIDENTID& self, const std::string& strItemName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList );
    virtual int ConsumeProcessEx( const NFIDENTID& self, const std::string& strSkillName, const NFIDataList& other, NFIDataList& damageListValue, NFIDataList& damageResultList );

private:
    NFIKernelModule* m_pKernelModule;
    NFISkillConsumeManagerModule* m_pSkillConsumeManagerModule;
    NFIElementInfoModule* m_pElementInfoModule;


};

#endif
