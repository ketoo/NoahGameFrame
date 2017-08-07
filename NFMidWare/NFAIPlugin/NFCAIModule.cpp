// -------------------------------------------------------------------------
//    @FileName			:    NFCAIModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCAIModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCAIModule.h"
#include "NFIStateMachine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCAIModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();

    //////////////////////////////////////////////////////////////////////////

    //mtStateMap.insert(TMAPSTATE::value_type(DeadState, new NFCDeadState(5.0f, pPluginManager)));
    //mtStateMap.insert(TMAPSTATE::value_type(FightState, new NFCFightState(1.0f, pPluginManager)));
    //mtStateMap.insert(TMAPSTATE::value_type(IdleState, new NFCIdleState(3.0f, pPluginManager)));
    //mtStateMap.insert(TMAPSTATE::value_type(PatrolState, new NFCPatrolState(5.0f, pPluginManager)));

    return true;
}

bool NFCAIModule::Shut()
{
    TOBJECTSTATEMACHINE::iterator itObject = mtObjectStateMachine.begin();
    for (itObject; itObject != mtObjectStateMachine.end(); itObject++)
    {
        delete itObject->second;
    }

    TMAPSTATE::iterator itState = mtStateMap.begin();
    for (itState; itState != mtStateMap.end(); itState++)
    {
        delete itState->second;
    }

    delete m_pHateModule;
    m_pHateModule = NULL;

    return true;
}

NFIState* NFCAIModule::GetState(const NFAI_STATE eState)
{
    TMAPSTATE::iterator it = mtStateMap.find(eState);
    if (it != mtStateMap.end())
    {
        return it->second;
    }

    return NULL;
}

bool NFCAIModule::CreateAIObject(const NFGUID& self)
{
    //这里只是为了以后方便维护状态机时间，节约CPU
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it == mtObjectStateMachine.end())
    {
        //mtObjectStateMachine.insert(TOBJECTSTATEMACHINE::value_type(self, new NFCStateMachine(self, this)));

        return true;
    }

    return false;
}

bool NFCAIModule::DelAIObject(const NFGUID& self)
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        delete it->second;
        mtObjectStateMachine.erase(it);
        return true;
    }

    return false;
}

bool NFCAIModule::Execute()
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.begin();
    for (it; it != mtObjectStateMachine.end(); it++)
    {
        it->second->Execute();
    }

    return true;
}

NFIStateMachine* NFCAIModule::GetStateMachine(const NFGUID& self)
{
    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        return it->second;
    }

    return NULL;
}

void NFCAIModule::OnBeKilled(const NFGUID& self, const NFGUID& other)
{

    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        it->second->ChangeState(DeadState);
    }
}

void NFCAIModule::OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue)
{
	m_pHateModule->AddHate(self, other, nDamageValue);
	m_pHateModule->CompSortList(self);

    NFIStateMachine* pStateMachine = GetStateMachine(self);
    if (pStateMachine)
    {
        if (FightState != pStateMachine->CurrentState())
        {
            pStateMachine->ChangeState(FightState);
        }
    }
}

void NFCAIModule::OnSpring(const NFGUID& self, const NFGUID& other)
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

void NFCAIModule::OnEndSpring(const NFGUID& self, const NFGUID& other)
{
    m_pHateModule->SetHateValue(self, other, 0);
}

void NFCAIModule::OnMotion(const NFGUID& self, int nResults)
{
}

void NFCAIModule::OnSelect(const NFGUID& self, const NFGUID& other)
{
}

int NFCAIModule::CanUseAnySkill(const NFGUID& self, const NFGUID& other)
{
    return 0;
}

float NFCAIModule::UseAnySkill(const NFGUID& self, const NFGUID& other)
{
    return 0.5f;
}

int NFCAIModule::OnAIObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
    {
        DelAIObject(self);
    }
    else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
    {
        //m_pEventProcessModule->AddEventCallBack(self, NFED_ON_SERVER_MOVEING, OnMoveRquireEvent);
        CreateAIObject(self);
    }

    return 0;
}

bool NFCAIModule::AfterInit()
{
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCAIModule::OnAIObjectEvent);
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCAIModule::OnAIObjectEvent);
    return true;
}

bool NFCAIModule::BeforeShut()
{
    return true;
}
