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

#include "NFCooldownModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCooldownModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}

void NFCooldownModule::AddCooldown(const NFGUID& self, const std::string& configID )
{
    //skillCnfID, usedTime
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::NPC::Cooldown::ThisName());
    const int nRow = xRecord->FindString(NFrame::NPC::Cooldown::ConfigID, configID);
    if (nRow >= 0)
    {
        //reset the time
        xRecord->SetInt(nRow, NFrame::NPC::Cooldown::ConfigID, NFGetTimeMS());
    }
    else
    {
        NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
        xDataList->SetString(NFrame::NPC::Cooldown::ConfigID, configID);
        xDataList->SetInt(NFrame::NPC::Cooldown::Time, NFGetTimeMS());

        xRecord->AddRow(nRow, *xDataList);
    }

	//for common skill cd, if you dont have a common skill CD, the monster will use multiple skills in one sec when meet players
	const int nRowCommon = xRecord->FindString(NFrame::NPC::Cooldown::ConfigID, NFrame::NPC::Cooldown::ThisName());
	if (nRowCommon >= 0)
	{
		//reset the time
		xRecord->SetInt(nRowCommon, NFrame::NPC::Cooldown::ConfigID, NFGetTimeMS());
	}
	else
	{
		NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
		xDataList->SetString(NFrame::NPC::Cooldown::ConfigID, NFrame::NPC::Cooldown::ThisName());
		xDataList->SetInt(NFrame::NPC::Cooldown::Time, NFGetTimeMS());

		xRecord->AddRow(nRowCommon, *xDataList);
	}
}

bool NFCooldownModule::ExistCooldown(const NFGUID& self, const std::string& configID )
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::NPC::Cooldown::ThisName());

	//for common skill if you dont have a common skill CD, the monster will use multiple skills in one sec when meet the players
	const int nRowCommon = xRecord->FindString(NFrame::NPC::Cooldown::ConfigID, NFrame::NPC::Cooldown::ThisName());
	if (nRowCommon >= 0)
	{
		float fCDTime = 1.0f;
		int64_t nLastTime = xRecord->GetInt(nRowCommon, NFrame::NPC::Cooldown::Time);
		int64_t nNowTime = NFGetTimeMS();
		if ((nNowTime - nLastTime) < fCDTime * 1000)
		{
			return true;
		}
	}

    const int nRow = xRecord->FindString(NFrame::NPC::Cooldown::ConfigID, configID);
    if (nRow >= 0)
    {
        //compare the time with the cooldown time
        float fCDTime = m_pElementModule->GetPropertyFloat(configID, NFrame::Skill::CoolDownTime());
        int64_t nLastTime = xRecord->GetInt(nRow, NFrame::NPC::Cooldown::Time);
        if ((NFGetTimeMS() - nLastTime) < fCDTime * 1000)
        {
            return true;
        }
    }

    return false;
}
