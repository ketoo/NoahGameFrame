/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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


#include "NFCSurvivalModule.h"

bool NFCSurvivalModule::Init()
{

	mnMinPeopleForSurvivalMode = 50;

	return true;
}

bool NFCSurvivalModule::Shut()
{
	return true;
}

bool NFCSurvivalModule::Execute()
{
	return true;
}

bool NFCSurvivalModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pRankModule = pPluginManager->FindModule<NFIRankModule>();


	m_pScheduleModule->AddSchedule("CheckSurvivalList", this, &NFCSurvivalModule::CheckSurvivalList, 10, -1);
	
	return true;
}

int NFCSurvivalModule::CheckSurvivalList(const std::string & strScheduleName, const float fIntervalTime, const int nCount)
{
	m_pLogModule->LogDebug(strScheduleName);

	if (m_pRankModule->GetRankListCount(NFIRankModule::RANK_TYPE::RT_SURVIVAL_MODE) >= mnMinPeopleForSurvivalMode)
	{

	}

	return 0;
}
