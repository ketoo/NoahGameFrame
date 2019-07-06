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


#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFBriefSkillConsumeProcessModule.h"

bool NFBriefSkillConsumeProcessModule::Init()
{
 
    return true;
}

bool NFBriefSkillConsumeProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pSkillConsumeManagerModule = pPluginManager->FindModule<NFISkillConsumeManagerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSkillModule = pPluginManager->FindModule<NFISkillModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();

    return true;
}

bool NFBriefSkillConsumeProcessModule::Shut()
{
    return true;
}

bool NFBriefSkillConsumeProcessModule::Execute()
{
    return true;
}


int NFBriefSkillConsumeProcessModule::ConsumeLegal( const NFGUID& self, const std::string& skillID,  const NFDataList& other )
{
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject( self );
	if ( pObject == NULL )
	{
	    return 1;
	}

    return 0;
}

int NFBriefSkillConsumeProcessModule::ConsumeProcess( const NFGUID& self, const std::string& skillID, const NFDataList& other, NFDataList& damageListValue, NFDataList& damageResultList )
{
    if (!m_pElementModule->ExistElement(skillID))
    {
        return 1;
    }

	const NFGUID xTeamID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::TeamID());
	const std::string& strConsumeProperty = m_pElementModule->GetPropertyString(skillID, NFrame::Skill::ConsumeProperty());
	const NFINT64 nConsumeValue = m_pElementModule->GetPropertyInt(skillID, NFrame::Skill::ConsumeValue());
	const NFINT64 nConsumeTYpe = m_pElementModule->GetPropertyInt(skillID, NFrame::Skill::ConsumeType());

	const std::string& strDamageProperty = m_pElementModule->GetPropertyString(skillID, NFrame::Skill::DamageProperty());
	const NFINT64 nDamageCnfValue = m_pElementModule->GetPropertyInt(skillID, NFrame::Skill::DamageValue());
	const NFINT64 nDamageTYpe = m_pElementModule->GetPropertyInt(skillID, NFrame::Skill::DamageType());

	const std::string& strGetBuffList = m_pElementModule->GetPropertyString(skillID, NFrame::Skill::GetBuffList());
	const std::string& strSendBuffList = m_pElementModule->GetPropertyString(skillID, NFrame::Skill::SendBuffList());

	const double fRequireDistance = m_pElementModule->GetPropertyFloat(skillID, NFrame::Skill::AtkDis());
	const double fDamageDistance = m_pElementModule->GetPropertyFloat(skillID, NFrame::Skill::DamageDistance());
	const NFINT64 nTargetType = m_pElementModule->GetPropertyInt(skillID, NFrame::Skill::EffectObjType());

	int64_t nOldConsumeVaue = m_pKernelModule->GetPropertyInt(self, strConsumeProperty);
	nOldConsumeVaue -= nConsumeValue;
	if (nOldConsumeVaue < 0)
	{
		return 2;
	}

	m_pKernelModule->SetPropertyInt(self, strConsumeProperty, nOldConsumeVaue);

	NFINT64 nSkillDamageValue = m_pKernelModule->Random(nDamageCnfValue, nDamageCnfValue * 1.2f);
	NFINT64 nAtkValue = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_VALUE());
	NFINT64 nAtkPVP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_PVP());

	NFINT64 nAtkFire = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_FIRE());
	NFINT64 nAtkDark = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_DARK());
	NFINT64 nAtkIce = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_ICE());
	NFINT64 nAtkLight = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_LIGHT());
	NFINT64 nAtkPoison = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_POISON());
	NFINT64 nAtkWind = m_pKernelModule->GetPropertyInt(self, NFrame::Player::ATK_WIND());

    for ( int j = 0; j < other.GetCount(); j++ )
    {
        NFGUID identOther = other.Object( j );
        if ( identOther.IsNull() )
        {
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
            continue;
        }

		int nCurHP = m_pKernelModule->GetPropertyInt(identOther, NFrame::NPC::HP());
		if (nCurHP <= 0)
		{
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
			continue;
		}

        NF_SHARE_PTR<NFIObject> pOtherObject = m_pKernelModule->GetObject( identOther );
        if ( pOtherObject == NULL )
        {
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
            continue;
        }

		// dont attack your team members
		const NFGUID xOtherTeamID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::TeamID());
		if (!xOtherTeamID.IsNull() && xOtherTeamID == xTeamID)
		{
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
			continue;
		}

		// dont attack yourself's building
		const NFGUID xMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
		const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
		if (xMasterID == self
			&& nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
		{
			damageListValue.AddInt(0);
			damageResultList.AddInt(0);
			continue;
		}

		pOtherObject->SetPropertyObject(NFrame::NPC::LastAttacker(), self);


		NFINT64 nDefFire = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_FIRE());
		NFINT64 nDefDark = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_DARK());
		NFINT64 nDefIce = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_ICE());
		NFINT64 nDefLight = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_LIGHT());
		NFINT64 nDefPoison = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_POISON());
		NFINT64 nDefWind = m_pKernelModule->GetPropertyInt(identOther, NFrame::Player::DEF_WIND());

		NFINT64 nDamageFire = nAtkFire - nDefFire;
		NFINT64 nDamageDark = nAtkDark - nDefDark;
		NFINT64 nDamageIce = nAtkIce - nDefIce;
		NFINT64 nDamageLight = nAtkLight - nDefLight;
		NFINT64 nDamagePoison = nAtkPoison - nDefPoison;
		NFINT64 nDamageWind = nAtkWind - nDefWind;
		if (nDamageFire < 0)
		{
			nDamageFire = 0;
		}

		if (nDamageDark < 0)
		{
			nDamageDark = 0;
		}

		if (nDamageIce < 0)
		{
			nDamageIce = 0;
		}

		if (nDamageLight < 0)
		{
			nDamageLight = 0;
		}

		if (nDamagePoison < 0)
		{
			nDamagePoison = 0;
		}

		if (nDamageWind < 0)
		{
			nDamageWind = 0;
		}

		NFINT64 TotalDamage = nDamageFire + nDamageDark + nDamageIce + nDamageLight + nDamagePoison + nDamageWind + nSkillDamageValue + nAtkPVP;

		m_pPropertyModule->CalculatePropertyValue(identOther, strDamageProperty, NFIPropertyModule::NPG_ALL, -TotalDamage, true);

		damageListValue.AddInt(TotalDamage);

		int n = m_pKernelModule->Random(0, 100);
		if (n < 20)
		{
			damageResultList.AddInt(NFMsg::EffectData_EResultType::EffectData_EResultType_EET_CRIT);
		}
		else
		{
			damageResultList.AddInt(NFMsg::EffectData_EResultType::EffectData_EResultType_EET_SUCCESS);
		}
    }

    return 0;
}
