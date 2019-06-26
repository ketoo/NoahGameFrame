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


#include "NFPropertyModule.h"

bool NFPropertyModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>();
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();

    return true;
}

bool NFPropertyModule::Shut()
{
    return true;
}

bool NFPropertyModule::Execute()
{
    return true;
}

bool NFPropertyModule::AfterInit()
{
    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFPropertyModule::OnObjectClassEvent);

    return true;
}

int64_t NFPropertyModule::GetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        return m_pKernelModule->GetRecordInt(self, NFrame::Player::CommValue::ThisName(), eGroupType, strPropertyName);
    }

    return m_pKernelModule->GetPropertyInt(self, strPropertyName);
}

int NFPropertyModule::SetPropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue)
{
    if (NFPropertyGroup::NPG_ALL != eGroupType)
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt(eGroupType, strPropertyName, nValue);
            }
        }
    }
    
    m_pKernelModule->SetPropertyInt(self, strPropertyName, nValue);

    return 0;
}


bool NFPropertyModule::CalculatePropertyValue(const NFGUID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int64_t nValue, const bool bPositive)
{
	if (NFPropertyGroup::NPG_ALL == eGroupType)
	{
		int64_t nPropertyValue = m_pKernelModule->GetPropertyInt(self, strPropertyName);
		nPropertyValue = nPropertyValue + nValue;
		if (bPositive)
		{
			nPropertyValue = (nPropertyValue >= 0) ? nPropertyValue : 0;
		}

		m_pKernelModule->SetPropertyInt(self, strPropertyName, nPropertyValue);
	}
	else
    {
        NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
				int64_t nPropertyValue = pRecord->GetInt(eGroupType, strPropertyName);
				nPropertyValue = nPropertyValue + nValue;
				if (bPositive)
				{
					nPropertyValue = (nPropertyValue >= 0) ? nPropertyValue : 0;
				}

                return pRecord->SetInt(eGroupType, strPropertyName, nPropertyValue);
            }
        }
    }

    return false;
}

int NFPropertyModule::OnObjectLevelEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	//if you dont play hero mode, please active the code below
    //RefreshBaseProperty(self);

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int NFPropertyModule::OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    
    const std::string& strRecordName = xEventData.strRecordName;
    const int nOpType = xEventData.nOpType;
    const int nRow = xEventData.nRow;
    const int nCol = xEventData.nCol;

    int nAllValue = 0;
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
    for (int i = 0; i < (int)(NFPropertyGroup::NPG_ALL); i++)
    {
        if (i < pRecord->GetRows())
        {
            int nValue = pRecord->GetInt32(i, nCol);
            nAllValue += nValue;
        }
    }

    m_pKernelModule->SetPropertyInt(self, pRecord->GetColTag(nCol), nAllValue);

    return 0;
}

int NFPropertyModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (strClassName == NFrame::Player::ThisName())
    {
		if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
		{
			NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
			if (pRecord)
			{
				for (int i = 0; i < NFPropertyGroup::NPG_ALL; i++)
				{
					pRecord->AddRow(-1);
				}
			}

			m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFPropertyModule::OnObjectLevelEvent);
			m_pKernelModule->AddRecordCallBack(self, NFrame::Player::CommValue::ThisName(), this, &NFPropertyModule::OnRecordEvent);
		}
		else if (CLASS_OBJECT_EVENT::COE_CREATE_AFTER_ATTACHDATA == eClassEvent)
        {
           

        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::OnlineCount());
            if (nOnlineCount <= 0 && m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID()) > 0)
            {
                m_pKernelModule->SetPropertyInt(self, NFrame::Player::Level(), 1);
            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::OnlineCount());
            m_pKernelModule->SetPropertyInt(self, NFrame::Player::OnlineCount(), (nOnlineCount + 1));

        }
    }

    return 0;
}

int NFPropertyModule::RefreshBaseProperty(const NFGUID& self)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
    if (!pRecord)
    {
        return 1;
    }

    
    int eJobType = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Job());
    int nLevel = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Level());

    for (int i = 0; i < pRecord->GetCols(); ++i)
    {
        const std::string& strColTag = pRecord->GetColTag(i);
        int nValue = (int)m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, strColTag);
        SetPropertyValue(self, strColTag, NFPropertyGroup::NPG_JOBLEVEL, nValue);
    }

    return 1;
}

bool NFPropertyModule::FullHPMP(const NFGUID& self)
{
    NFINT64 nMaxHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());
    if (nMaxHP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nMaxHP);
    }

    NFINT64 nMaxMP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXMP());
    if (nMaxMP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nMaxMP);
    }

    return true;
}

bool NFPropertyModule::AddHP(const NFGUID& self, const int nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HP());
	int nMaxValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MAXHP());

    if (nCurValue > 0)
    {
        nCurValue += nValue;
        if (nCurValue > nMaxValue)
        {
            nCurValue = nMaxValue;
        }

        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);
    }

    return true;
}

bool NFPropertyModule::EnoughHP(const NFGUID& self, const int nValue)
{
    NFINT64 nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFPropertyModule::DamageHP(const NFGUID & self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HP());
	if (nCurValue > 0)
	{
		nCurValue -= nValue;
		nCurValue = (nCurValue >= 0) ? nCurValue : 0;

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);

		return true;
	}

	return false;
}

bool NFPropertyModule::ConsumeHP(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::HP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nCurValue);

        return true;
    }

    return false;
}

bool NFPropertyModule::AddMP(const NFGUID& self, const int nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MP());
	int nMaxValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MAXMP());

    nCurValue += nValue;
    if (nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

    return true;
}

bool NFPropertyModule::ConsumeMP(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

        return true;
    }

    return false;
}

bool NFPropertyModule::EnoughMP(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFPropertyModule::DamageMP(const NFGUID & self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MP());
	if (nCurValue > 0)
	{
		nCurValue -= nValue;
		nCurValue = (nCurValue >= 0) ? nCurValue : 0;

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nCurValue);

		return true;
	}

	return false;
}

bool NFPropertyModule::FullSP(const NFGUID& self)
{
	int nMAXCSP = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MAXSP());
    if (nMAXCSP > 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nMAXCSP);

        return true;
    }

    return false;
}

bool NFPropertyModule::AddSP(const NFGUID& self, const int nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SP());
	int nMaxValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::MAXSP());

    nCurValue += nValue;
    if (nCurValue > nMaxValue)
    {
        nCurValue = nMaxValue;
    }

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCurValue);

    return true;
}

bool NFPropertyModule::ConsumeSP(const NFGUID& self, const int nValue)
{
	int nCSP = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SP());
    if ((nCSP > 0) && (nCSP - nValue >= 0))
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::SP(), nCSP);

        return true;
    }

    return false;
}

bool NFPropertyModule::EnoughSP(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SP());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFPropertyModule::AddGold(const NFGUID& self, const int64_t nValue)
{
	int64_t nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue += nValue;
	if (nCurValue < 0)
	{
		nCurValue = 0;
	}

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

    return false;
}

bool NFPropertyModule::ConsumeGold(const NFGUID& self, const int64_t nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

	int64_t nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Gold(), nCurValue);

        return true;
    }

    return false;
}

bool NFPropertyModule::EnoughGold(const NFGUID& self, const int64_t nValue)
{
	int64_t nCurValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Gold());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}

bool NFPropertyModule::AddDiamond(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Diamond());
    nCurValue += nValue;
	if (nCurValue < 0)
	{
		nCurValue = 0;
	}

    m_pKernelModule->SetPropertyInt(self, NFrame::Player::Diamond(), nCurValue);

    return false;
}

bool NFPropertyModule::ConsumeDiamond(const NFGUID& self, const int nValue)
{
    if (nValue <= 0)
    {
        return false;
    }

	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Diamond());
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt(self, NFrame::Player::Diamond(), nCurValue);

        return true;
    }

    return false;
}

bool NFPropertyModule::EnoughDiamond(const NFGUID& self, const int nValue)
{
	int nCurValue = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Diamond());
    if ((nCurValue > 0) && (nCurValue - nValue >= 0))
    {
        return true;
    }

    return false;
}