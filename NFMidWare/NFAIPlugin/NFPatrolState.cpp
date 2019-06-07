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
#include "NFPatrolState.h"

NFPatrolState::NFPatrolState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(PatrolState, fHeartBeatTime, p)
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

bool NFPatrolState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
        RandomPatrol(self, pStateMachine);
    }

    return true;
}

bool NFPatrolState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Execute(self, pStateMachine))
    {
		/*
			NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
			NFAI_NPC_TYPE eMoveType = (NFAI_NPC_TYPE)(m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType()));

			//如果是定点的，则不走，继续idle
			switch (eMoveType)
			{
			case NFAI_NPC_TYPE::MASTER_TYPE:
				{
					//查找是否有可以攻击的对象
					if (!ident.IsNull())
					{
						pStateMachine->ChangeState(FightState);
					}
					else
					{
						//下一个节点
                        //object记录当前的路径ID和index，以记录寻路的位置
					}
				}
				break;

			case NFAI_NPC_TYPE::HERO_TYPE:
				{
					//查找是否有可以攻击的对象
					if (!ident.IsNull())
					{
						pStateMachine->ChangeState(FightState);
					}
					else
					{
						RandomPatrol(self, pStateMachine);
					}
				}
				break;
			default:
				break;
			}
			*/
    }

    return true;
}

bool NFPatrolState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{

    return true;
}

bool NFPatrolState::DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
{

    return true;
}

bool NFPatrolState::RandomPatrol(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    //首先，得看有没路径

    //没有的话，随机找个地址走吧(以出生点为中心开始找,种子地址)NPCConfigID
    const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, "NPCConfigID");
    const std::string& strNPCID = m_pKernelModule->GetPropertyString(self, "ConfigID");
    if (!strConfigID.empty())
    {
		NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementModule->GetPropertyManager(strConfigID);
		if (xPropertyManager)
		{
			NF_SHARE_PTR<NFIProperty> xPropertyX =  xPropertyManager->GetElement("SeedX");
			NF_SHARE_PTR<NFIProperty> xPropertyY =  xPropertyManager->GetElement("SeedY");
			NF_SHARE_PTR<NFIProperty> xPropertyZ =  xPropertyManager->GetElement("SeedZ");

			double fCurX = xPropertyX->GetFloat();
			double fCurY = xPropertyY->GetFloat();
			double fCurZ = xPropertyZ->GetFloat();

			double fPosOffestX = (double)(rand() / double(RAND_MAX) - 0.5);
			double fPosOffestZ = (double)(rand() / double(RAND_MAX) - 0.5);
			fPosOffestX *= 10;
			fPosOffestZ *= 10;

			fCurX += fPosOffestX;
			fCurZ += fPosOffestZ;

			//if (fCurX > 0.0f && fCurZ > 0.0f)
			{
				//看能否寻路，不能寻路则重来
				NFAI_STATE eStateType = NFAI_STATE::ChaseState;
				double fRand = (double)(rand() / double(RAND_MAX));
				if (fRand < 0.5f)
				{
					eStateType = NFAI_STATE::ChaseState;
				}

				NFDataList valueList;
				valueList.AddFloat(fCurX);
				valueList.AddFloat(fCurY);
				valueList.AddFloat(fCurZ);
				//m_pKernelModule->DoEvent(self, NFED_ON_CLIENT_REQUIRE_MOVE, valueList);

				m_pKernelModule->SetPropertyFloat(self, "X", fCurX);
				m_pKernelModule->SetPropertyFloat(self, "Y", fCurY);
				m_pKernelModule->SetPropertyFloat(self, "Z", fCurZ);

				return true;
			}
		}
	}

    return false;
}
