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


#include "NFBulletSkillConsumeProcessModule.h"

bool NFBulletSkillConsumeProcessModule::Init()
{
    return true;
}

bool NFBulletSkillConsumeProcessModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}

bool NFBulletSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFBulletSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFBulletSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other )
{
        return 1;
}

int NFBulletSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& strItemName, const NFDataList& other, NFDataList& damageListValue , NFDataList& damageResultList )
{
    return 0;
}