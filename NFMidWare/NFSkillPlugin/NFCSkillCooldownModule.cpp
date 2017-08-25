//
// Created by LUSHENG HUANG on 23/08/17.
//
#include "NFCSkillCooldownModule.h"
#include <NFComm/NFPluginModule/NFGUID.h>

void NFCSkillCooldownModule::AddSkillCD(const NFGUID& self, const std::string& strSkillName )
{
    /*
    //skillCnfID, usetTime
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::Player::SkillCoolDown.ThisName());
    const int nRow = xRecord->FindString(NFrame::Player::SkillCoolDown.SkillID(), strSkillName);
    if (nRow >= 0)
    {
        //reset the time
        xRecord->SetInt(nRow, NFrame::Player::SkillCoolDown.SkillID(), NFGetTime());
    }
    else
    {
        NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
        xDataList->SetString(0, strSkillName);
        xDataList->SetInt(1, NFGetTime());

        xRecord->AddRow(nRow, *xDataList);
    }
     */
}

bool NFCSkillCooldownModule::ExistSkillCD( const NFGUID& self, const std::string& strSkillName )
{
    /*
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::Player::SkillCoolDown.ThisName());
    const int nRow = xRecord->FindString("", strSkillName);
    if (nRow >= 0)
    {
        //compare the time with the cooldown time
        float fCDTime = m_pElementModule->GetPropertyFloat(strSkillName, NFrame::Skill::CoolDownTime());
        int64_t nLastTime = xRecord->GetInt(nRow, NFrame::Player::SkillCoolDown.SkillTime());
        if ((NFGetTime() - nLastTime) > fCDTime * 1000)
        {
            return true;
        }
    }
     */

    return false;
}
