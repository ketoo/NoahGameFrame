/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

bool NFSkillConsumeManagerModule::Init()
{

    return true;
}

bool NFSkillConsumeManagerModule::Shut()
{

    return true;
}

bool NFSkillConsumeManagerModule::Execute()
{

    return true;
}

bool NFSkillConsumeManagerModule::AfterInit()
{

    return true;
}

bool NFSkillConsumeManagerModule::SetConsumeModule(const int skillType, NFISkillConsumeProcessModule* pModule)
{
	mSkillConsumeProcess[NFGUID(skillType, 0)] = pModule;
    return true;
}

NFISkillConsumeProcessModule* NFSkillConsumeManagerModule::GetConsumeModule(const int skillType)
{
	auto it = mSkillConsumeProcess.find(NFGUID(skillType, 0));
	if (it != mSkillConsumeProcess.end())
	{
		return it->second;
	}

	return nullptr;
}

bool NFSkillConsumeManagerModule::SetConsumeModule(const int skillType, const int skillSubType, NFISkillConsumeProcessModule *pModule)
{
	mSkillConsumeProcess[NFGUID(skillType, skillSubType)] = pModule;
	return false;
}

NFISkillConsumeProcessModule *NFSkillConsumeManagerModule::GetConsumeModule(const int skillType, const int skillSubType)
{
	auto it = mSkillConsumeProcess.find(NFGUID(skillType, skillSubType));
	if (it != mSkillConsumeProcess.end())
	{
		return it->second;
	}
	else
	{
		it = mSkillConsumeProcess.find(NFGUID(skillType, 0));
		if (it != mSkillConsumeProcess.end())
		{
			return it->second;
		}
	}

	return nullptr;
}
