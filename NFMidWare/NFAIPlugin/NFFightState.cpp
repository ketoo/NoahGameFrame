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

#include "NFIStateMachine.h"
#include "NFFightState.h"

NFFightState::NFFightState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(FightState, fHeartBeatTime, p)
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
	m_pSkillModule = pPluginManager->FindModule<NFISkillModule>();
}

bool NFFightState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
        Execute(self, pStateMachine);
    }

    return true;
}

bool NFFightState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
	const NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
	if (ident.IsNull())
	{
		pStateMachine->ChangeState(IdleState);
		return false;
	}

	const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
	if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL)
	{

	}
	else if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
	{
		if (m_pKernelModule->GetPropertyInt(self, NFrame::NPC::HP()) > 0)
		{
			if (m_pKernelModule->GetPropertyInt(ident, NFrame::NPC::HP()) > 0)
			{
				const NFVector3& selfPos = m_pKernelModule->GetPropertyVector3(self, NFrame::Player::Position());
				const NFVector3& identPos = m_pKernelModule->GetPropertyVector3(ident, NFrame::Player::Position());
				const float fDis = selfPos.Distance(identPos);

				const std::string& strSkillCnfID = m_pAIModule->ChooseSkill(self, fDis);
				if (strSkillCnfID.empty())
				{
					pStateMachine->ChangeState(IdleState);
				}
				else
				{
					m_pSkillModule->UseSkill(self, strSkillCnfID, ident);

					pStateMachine->ChangeState(IdleState);
				}
			}
		}
		else
		{
			pStateMachine->ChangeState(DeadState);
		}
	}

    return true;
}

bool NFFightState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{

    return true;
}
