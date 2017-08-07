// -------------------------------------------------------------------------
//    @FileName      :   NFCBulletSkillConsumeProcessModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCBulletSkillConsumeProcessModule
// -------------------------------------------------------------------------

#include "NFCBulletSkillConsumeProcessModule.h"

bool NFCBulletSkillConsumeProcessModule::Init()
{
    return true;
}

bool NFCBulletSkillConsumeProcessModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}

bool NFCBulletSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFCBulletSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFCBulletSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other )
{
        return 1;
}

int NFCBulletSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFDataList& other, NFDataList& damageListValue , NFDataList& damageResultList )
{
    return 0;
}