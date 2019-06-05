/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "NFSkillConsumeManagerModule.h"
#include "NFBulletSkillConsumeProcessModule.h"
#include "NFBriefSkillConsumeProcessModule.h"

bool NFSkillConsumeManagerModule::Init()
{
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BRIEF_SINGLE, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBriefSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BRIEF_GROUP, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBriefSkillConsumeProcessModule(pPluginManager)));
	
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_SINGLE, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBulletSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_REBOUND, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBulletSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_POS_BOMB, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBulletSkillConsumeProcessModule(pPluginManager)));
	ResgisterConsumeModule(NFMsg::ESkillType::EST_BULLET_TARGET_BOMB, NF_SHARE_PTR<NFISkillConsumeProcessModule>(NF_NEW NFBulletSkillConsumeProcessModule(pPluginManager)));

	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Init();
	}

    return true;
}

bool NFSkillConsumeManagerModule::Shut()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Shut();
	}
    return true;
}

bool NFSkillConsumeManagerModule::Execute()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->Execute();
	}
    return true;
}

bool NFSkillConsumeManagerModule::AfterInit()
{
	for (NF_SHARE_PTR<NFISkillConsumeProcessModule> xModule = First(); xModule != nullptr; xModule = Next())
	{
		xModule->AfterInit();
	}
    return true;
}

bool NFSkillConsumeManagerModule::ResgisterConsumeModule( const int nModuleType, NF_SHARE_PTR<NFISkillConsumeProcessModule> pModule )
{
    return AddElement( nModuleType, pModule );
}

NF_SHARE_PTR<NFISkillConsumeProcessModule> NFSkillConsumeManagerModule::GetConsumeModule( const int nModuleType )
{
    return GetElement( nModuleType );
}
