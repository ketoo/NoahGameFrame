//
// Created by LUSHENG HUANG on 23/08/17.
//
#include "NFCSkillCooldownModule.h"
#include <NFComm/NFPluginModule/NFGUID.h>


bool NFCSkillCooldownModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}

void NFCSkillCooldownModule::AddSkillCD(const NFGUID& self, const std::string& strSkillName )
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

bool NFCSkillCooldownModule::ExistSkillCD( const NFGUID& self, const std::string& strSkillName )
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
