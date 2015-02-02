// -------------------------------------------------------------------------
//    @FileName      :   NFCSkillConsumeManagerModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCSkillConsumeManagerModule
//    @Desc             :   技能消费机制管理类,所有类型的技能消费类型均需注册才能消费
// -------------------------------------------------------------------------

#include "NFCSkillConsumeManagerModule.h"

//

bool NFCSkillConsumeManagerModule::Init()
{
    return true;
}

bool NFCSkillConsumeManagerModule::Shut()
{
    return true;
}

bool NFCSkillConsumeManagerModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCSkillConsumeManagerModule::AfterInit()
{
    return true;
}

bool NFCSkillConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NFISkillConsumeProcessModule* pModule )
{
    return AddElement( nModuleType, pModule );
}

NFISkillConsumeProcessModule* NFCSkillConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
