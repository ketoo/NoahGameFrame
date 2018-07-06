/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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
#include "NFCChaseState.h"

NFCChaseState::NFCChaseState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(ChaseState, fHeartBeatTime, p)
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

bool NFCChaseState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
        // TODO
    }

    return true;
}

bool NFCChaseState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
	/*
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //查找是否有可以攻击的对象
			NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));
            NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                // 增加技能是否可以用判断
                if (NFSkillTestSkillResult::NFSTSR_OK == m_pAIModule->CanUseAnySkill(self, ident))
                {
                    pStateMachine->ChangeState(FightState);
                }
                else
                {
                    // 走向目标
					if (NFAI_MOVE_TYPE::NO_MOVE_TYPE == eMoveType)
					{
						m_pKernelModule->SetPropertyFloat(self, "X", m_pKernelModule->GetPropertyFloat(ident, "X"));
						m_pKernelModule->SetPropertyFloat(self, "Y", m_pKernelModule->GetPropertyFloat(ident, "Y"));
						m_pKernelModule->SetPropertyFloat(self, "Z", m_pKernelModule->GetPropertyFloat(ident, "Z"));
					}
                }
            }
            else
            {
				if (NFAI_MOVE_TYPE::NO_MOVE_TYPE == eMoveType)
				{
					pStateMachine->ChangeState(IdleState);
				}
				else
				{
					pStateMachine->ChangeState(PatrolState);
				}
            }
        }
    }
	*/
    return true;
}

bool NFCChaseState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    return true;
}

bool NFCChaseState::DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    return true;
}