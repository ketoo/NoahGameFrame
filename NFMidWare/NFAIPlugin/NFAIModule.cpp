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

#include "NFAIModule.h"
#include "NFIStateMachine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFAIModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
	m_pFriendModule = pPluginManager->FindModule<NFIFriendModule>();
	
    //////////////////////////////////////////////////////////////////////////
	mtStateMap[IdleState] = new NFIdleState(1.0f, pPluginManager);
	mtStateMap[FightState] = new NFFightState(1.0f, pPluginManager);
	mtStateMap[PatrolState] = new NFPatrolState(5.0f, pPluginManager);
	mtStateMap[ChaseState] = new NFChaseState(1.0f, pPluginManager);
	mtStateMap[DeadState] = new NFDeadState(5.0f, pPluginManager);

    return true;
}

bool NFAIModule::Shut()
{
	mtObjectStateMachine.clear();

    return true;
}

NFIState* NFAIModule::GetState(const NFAI_STATE eState)
{
    return mtStateMap[eState];
}

const std::string & NFAIModule::ChooseSkill(const NFGUID & self, const float fDis)
{
	const std::string & strCnfID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
	const std::string & strNormal = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillNormal());

	long nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
	if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
	{
		return strNormal;
	}

	const std::string & strAttack = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillAttack());
	const std::string & strThump = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillTHUMP());

	float fNormal = (float)m_pElementModule->GetPropertyFloat(strNormal, NFrame::Skill::AtkDis());
	float fAttack = (float)m_pElementModule->GetPropertyFloat(strAttack, NFrame::Skill::AtkDis());
	float fThump = (float)m_pElementModule->GetPropertyFloat(strThump, NFrame::Skill::AtkDis());

	if (fNormal < fDis
		&& fAttack < fDis
		&& fThump < fDis)
	{
		return "";
	}

	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::NPC::Cooldown::ThisName());
	const int nCommonSKillRow = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, "CommonSkillID");
	if (nCommonSKillRow >= 0)
	{
		long nTime = xRecord->GetInt(nCommonSKillRow, NFrame::NPC::Cooldown::Time);
		long milliseconds = NFGetTimeMS();
		if (milliseconds < nTime)
		{
			return "";
		}
	}

	int nRand = m_pKernelModule->Random(0, 10);
	if (nRand <= 3)
	{
		if (fThump < fDis)
		{
			//cd
			int nRow = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, strThump);
			if (nRow >= 0)
			{
				long nTime = xRecord->GetInt(nRow, NFrame::NPC::Cooldown::Time);
				long milliseconds = NFGetTimeMS();
				if (milliseconds >= nTime)
				{
					return strThump;
				}
			}
		}
	}
	else if (nRand <= 6)
	{
		if (fAttack < fDis)
		{
			//cd
			int nRow = xRecord->FindString(NFrame::NPC::Cooldown::SkillID, strAttack);
			if (nRow >= 0)
			{
				long nTime = xRecord->GetInt(nRow, NFrame::NPC::Cooldown::Time);
				long milliseconds = NFGetTimeMS();
				if (milliseconds >= nTime)
				{
					return strAttack;
				}
			}
		}
	}

	return strNormal;
}

bool NFAIModule::CreateAIObject(const NFGUID& self)
{
	std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.find(self);
    if (it == mtObjectStateMachine.end())
    {
        mtObjectStateMachine.insert(std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::value_type(self, NF_SHARE_PTR<NFIStateMachine>(new NFStateMachine(self, pPluginManager))));

        return true;
    }

    return false;
}

bool NFAIModule::DelAIObject(const NFGUID& self)
{
	std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        mtObjectStateMachine.erase(it);
        return true;
    }

    return false;
}

bool NFAIModule::Execute()
{
	std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.begin();
    for (it; it != mtObjectStateMachine.end(); it++)
    {
        it->second->Execute();
    }

    return true;
}

NF_SHARE_PTR<NFIStateMachine> NFAIModule::GetStateMachine(const NFGUID& self)
{
	std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        return it->second;
    }

    return nullptr;
}

int NFAIModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	const int sceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	const int groupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

    if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
		DelAIObject(self);
    }
    else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		CreateAIObject(self);

		m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFAIModule::OnNPCHPEvent);
	}

    return 0;
}

int NFAIModule::OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	const NFGUID& lastAttacker = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::LastAttacker());
	if (newVar.GetInt() <= 0)
	{
		std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.find(self);
		if (it != mtObjectStateMachine.end())
		{
			it->second->ChangeState(DeadState);
		}
	}

	return 0;
}

bool NFAIModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFAIModule::OnClassObjectEvent);
    return true;
}

bool NFAIModule::BeforeShut()
{
    return true;
}
