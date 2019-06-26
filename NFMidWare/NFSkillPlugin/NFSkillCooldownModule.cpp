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

#include "NFSkillCooldownModule.h"
#include <NFComm/NFPluginModule/NFGUID.h>


bool NFSkillCooldownModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}

void NFSkillCooldownModule::AddSkillCD(const NFGUID& self, const std::string& strSkillName )
{
    //skillCnfID, usetTime
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::NPC::Cooldown::ThisName());
    const int nRow = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, strSkillName);
    if (nRow >= 0)
    {
        //reset the time
        xRecord->SetInt(nRow, NFrame::NPC::Cooldown::SkillID, NFGetTimeMS());
    }
    else
    {
        NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
        xDataList->SetString(NFrame::NPC::Cooldown::SkillID, strSkillName);
        xDataList->SetInt(NFrame::NPC::Cooldown::Time, NFGetTimeMS());

        xRecord->AddRow(nRow, *xDataList);
    }

	//for common skillm if you dont have a common skill CD, the monster will use multiple skills in one sec when meet the players
	const int nRowCommon = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, mstrCommonSkillID);
	if (nRowCommon >= 0)
	{
		//reset the time
		xRecord->SetInt(nRowCommon, NFrame::NPC::Cooldown::SkillID, NFGetTimeMS());
	}
	else
	{
		NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
		xDataList->SetString(NFrame::NPC::Cooldown::SkillID, mstrCommonSkillID);
		xDataList->SetInt(NFrame::NPC::Cooldown::Time, NFGetTimeMS());

		xRecord->AddRow(nRowCommon, *xDataList);
	}
}

bool NFSkillCooldownModule::ExistSkillCD( const NFGUID& self, const std::string& strSkillName )
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::NPC::Cooldown::ThisName());

	//for common skillm if you dont have a common skill CD, the monster will use multiple skills in one sec when meet the players
	const int nRowCommon = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, mstrCommonSkillID);
	if (nRowCommon >= 0)
	{
		float fCDTime = 1.5f;
		int64_t nLastTime = xRecord->GetInt(nRowCommon, NFrame::NPC::Cooldown::Time);
		int64_t nNowTime = NFGetTimeMS();
		if ((nNowTime - nLastTime) < fCDTime * 1000)
		{
			return true;
		}
	}

    const int nRow = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, strSkillName);
    if (nRow >= 0)
    {
        //compare the time with the cooldown time
        float fCDTime = m_pElementModule->GetPropertyFloat(strSkillName, NFrame::Skill::CoolDownTime());
        int64_t nLastTime = xRecord->GetInt(nRow, NFrame::NPC::Cooldown::Time);
        if ((NFGetTimeMS() - nLastTime) < fCDTime * 1000)
        {
            return true;
        }
    }

    return false;
}
