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
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BRIEF_SINGLE, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBriefSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BRIEF_GROUP, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBriefSkillConsumeProcessModule(pPluginManager)));
	
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_SINGLE, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBulletSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_REBOUND, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBulletSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_BOMB, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFCBulletSkillConsumeProcessModule(pPluginManager)));

	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Init();
	}

    return true;
}

bool NFCSkillConsumeManagerModule::Shut()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Shut();
	}
    return true;
}

bool NFCSkillConsumeManagerModule::Execute()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Execute();
	}
    return true;
}

bool NFCSkillConsumeManagerModule::AfterInit()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->AfterInit();
	}
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
