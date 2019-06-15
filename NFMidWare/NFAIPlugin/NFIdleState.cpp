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
#include "NFIdleState.h"

NFIdleState::NFIdleState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(IdleState, fHeartBeatTime, p)
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
    m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

NFIdleState::~NFIdleState()
{

}

bool NFIdleState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
			//看是否有战斗能力
            switch (pStateMachine->LastState())
            {
                case NFAI_STATE::FightState:
                    //找人继续打
                    Execute(self, pStateMachine);
                    break;

                default:
                    break;
            }
    }

    return true;
}

bool NFIdleState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Execute(self, pStateMachine))
    {
            //查找是否有可以攻击的对象
            NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                pStateMachine->ChangeState(FightState);
            }
            else
            {
				const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
				if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL)
				{
					if (m_pKernelModule->GetPropertyInt(self, NFrame::NPC::MOVE_SPEED()) > 0)
					{
						RandomIdle(self, pStateMachine);
					}
				}
				else if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
				{
					//do nothing
				}
            }
    }

    return true;
}

bool NFIdleState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{


    return true;
}

bool NFIdleState::DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    return true;
}

bool NFIdleState::RandomIdle(const NFGUID& self, NFIStateMachine* pStateMachine)
{
	//如果是定点的，则不走，继续idle
	const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
	if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
	{

	}
	else if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL)
	{
		//change the status as PatrolState if the NPC has the move ability
		/*
		float fRand = (float)(rand() / double(RAND_MAX));
		if (fRand < 0.4f)
		{
			pStateMachine->ChangeState(PatrolState);
		}
		*/
		//else do nothing
	}
	
	

    return false;
}
