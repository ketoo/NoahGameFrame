//
// Created by LUSHENG HUANG on 23/08/17.
//
#include "NFCSkillCooldownModule.h"
#include <NFComm/NFPluginModule/NFGUID.h>

void NFCSkillCooldownModule::AddSkillCD(const NFGUID& self, const std::string& strSkillName )
{
    //skillCnfID, usetTime
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::IObject::Cooldown::ThisName());
    const int nRow = xRecord->FindString(NFrame::IObject::Cooldown::SkillID, strSkillName);
    if (nRow >= 0)
    {
        //reset the time
        xRecord->SetInt(nRow, NFrame::IObject::Cooldown::SkillID, NFGetTime());
    }
    else
    {
        NF_SHARE_PTR<NFDataList> xDataList = xRecord->GetInitData();
        xDataList->SetString(0, strSkillName);
        xDataList->SetInt(1, NFGetTime());

        xRecord->AddRow(nRow, *xDataList);
    }
}

bool NFCSkillCooldownModule::ExistSkillCD( const NFGUID& self, const std::string& strSkillName )
{
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::IObject::Cooldown::ThisName());
    const int nRow = xRecord->FindString("", strSkillName);
    if (nRow >= 0)
    {
        //compare the time with the cooldown time
        float fCDTime = m_pElementModule->GetPropertyFloat(strSkillName, NFrame::IObject::Cooldown::SkillID);
        int64_t nLastTime = xRecord->GetInt(nRow, NFrame::IObject::Cooldown::Time);
        if ((NFGetTime() - nLastTime) > fCDTime * 1000)
        {
            return true;
        }
    }

    return false;
}
