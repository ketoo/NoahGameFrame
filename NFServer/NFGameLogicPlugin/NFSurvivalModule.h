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


#ifndef NF_SURVIVAL_MODULE_H
#define NF_SURVIVAL_MODULE_H

#include "NFomm/NFore/NFonsistentHash.hpp"
#include "NFomm/NFPluginModule/NFIKernelModule.h"
#include "NFomm/NFPluginModule/NFINoSqlModule.h"
#include "NFomm/NFPluginModule/NFIElementModule.h"
#include "NFomm/NFPluginModule/NFICreateRoleModule.h"
#include "NFomm/NFPluginModule/NFIPlayerRedisModule.h"
#include "NFomm/NFPluginModule/NFIScheduleModule.h"
#include "NFomm/NFPluginModule/NFISceneAOIModule.h"
#include "NFomm/NFPluginModule/NFISceneProcessModule.h"
#include "NFomm/NFPluginModule/NFISurvivalModule.h"
#include "NFomm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFomm/NFPluginModule/NFILogModule.h"
#include "NFomm/NFPluginModule/NFIRankModule.h"

class NFSurvivalModule
    : public NFISurvivalModule
{
public:
    NFSurvivalModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFSurvivalModule() {};

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

protected:
	int CheckSurvivalList(const std::string& strScheduleName, const float fIntervalTime, const int nCount);


private:
    int mnMinPeopleForSurvivalMode;
private:
	NFIScheduleModule* m_pScheduleModule;
	NFINetModule* m_pNetModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
    NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNoSqlModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
	NFIPlayerRedisModule* m_pPlayerRedisModule;
	NFISceneAOIModule* m_pSceneAOIModule;
	NFISceneProcessModule* m_pSceneProcessModule;
	NFILogModule* m_pLogModule;
	NFIRankModule* m_pRankModule;
};

#endif