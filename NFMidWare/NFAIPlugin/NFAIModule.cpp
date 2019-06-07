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
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();

    //////////////////////////////////////////////////////////////////////////
	mtStateMap[IdleState] = new NFIdleState(3.0f, pPluginManager);
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

bool NFAIModule::CreateAIObject(const NFGUID& self)
{
    //这里只是为了以后方便维护状态机时间，节约CPU
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

void NFAIModule::OnBeKilled(const NFGUID& self, const NFGUID& other)
{
	std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>>::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        it->second->ChangeState(DeadState);
    }
}

void NFAIModule::OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue)
{
	m_pHateModule->AddHate(self, other, nDamageValue);
	m_pHateModule->CompSortList(self);
}

void NFAIModule::OnSpring(const NFGUID& self, const NFGUID& other)
{
	/*
    //根据职业,等级,血量,防御
    //战斗状态只打仇恨列表内的人，巡逻,休闲状态才重新找对象打
    NF_AI_SUB_TYPE subType = (NF_AI_SUB_TYPE)m_pKernelModule->GetPropertyInt(self, "SubType");
    if (NF_AI_SUB_TYPE::NFAST_INITATIVE == subType)
    {
        //玩家或者PET进入
        const std::string& strClassName = m_pKernelModule->GetPropertyString(other, "ClassName");
        if ("Player" == strClassName
            || "Pet" == strClassName)
        {
            GetHateModule()->AddHate(self, other, 100);
            GetHateModule()->CompSortList(self);
            NFIStateMachine* pStateMachine = GetStateMachine(self);
            if (pStateMachine)
            {
                if (NFAI_STATE::FightState != pStateMachine->CurrentState())
                {
                    NFIState* pState = GetState(pStateMachine->CurrentState());
                    pState->Execute(self);
                }
            }
        }
    }
	*/
}

void NFAIModule::OnEndSpring(const NFGUID& self, const NFGUID& other)
{
    m_pHateModule->SetHateValue(self, other, 0);
}

void NFAIModule::OnMotion(const NFGUID& self, int nResults)
{
}

void NFAIModule::OnSelect(const NFGUID& self, const NFGUID& other)
{
}

int NFAIModule::CanUseAnySkill(const NFGUID& self, const NFGUID& other)
{
    return 0;
}

float NFAIModule::UseAnySkill(const NFGUID& self, const NFGUID& other)
{
    return 0.5f;
}

int NFAIModule::OnAIObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        DelAIObject(self);
    }
    else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
    {
        //m_pEventProcessModule->AddEventCallBack(self, NFED_ON_SERVER_MOVEING, OnMoveRquireEvent);
		const int nNPCType = m_pKernelModule->GetPropertyInt(self, NFrame::NPC::NPCType());
		if (nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
		{
			CreateAIObject(self);
		}
		else if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL)
		{
		}
    }

    return 0;
}

bool NFAIModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFAIModule::OnAIObjectEvent);
    return true;
}

bool NFAIModule::BeforeShut()
{
    return true;
}
