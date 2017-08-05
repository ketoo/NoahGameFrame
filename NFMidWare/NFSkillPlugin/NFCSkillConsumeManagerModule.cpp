// -------------------------------------------------------------------------
//    @FileName      :   NFCSkillConsumeManagerModule.cpp
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-10-02
//    @Module           :   NFCSkillConsumeManagerModule

// -------------------------------------------------------------------------

#include "NFCSkillConsumeManagerModule.h"
#include "NFCBulletSkillConsumeProcessModule.h"
#include "NFCBriefSkillConsumeProcessModule.h"

bool NFCSkillConsumeManagerModule::Init()
{
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BRIEF, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBriefSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBulletSkillConsumeProcessModule(pPluginManager)));

    return true;
}

bool NFCSkillConsumeManagerModule::Shut()
{
    return true;
}

bool NFCSkillConsumeManagerModule::Execute()
{
    return true;
}

bool NFCSkillConsumeManagerModule::AfterInit()
{
    return true;
}

bool NFCSkillConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFISkillConsumeProcessModule> pModule )
{
    return AddElement( nModuleType, pModule );
}

NF_SHARE_PTR<NFISkillConsumeProcessModule> NFCSkillConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
