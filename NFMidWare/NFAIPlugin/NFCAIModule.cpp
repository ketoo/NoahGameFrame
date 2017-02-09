// -------------------------------------------------------------------------
//    @FileName			:    NFCAIModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCAIModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCAIModule.h"
#include "NFCStateMachine.h"
#include "NFCHateModule.h"
#include "NFAIPlugin.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


bool NFCAIModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");

    assert(NULL != m_pKernelModule);

    //////////////////////////////////////////////////////////////////////////

    m_pHateModule = new NFCHateModule(pPluginManager);

    //获取所有的状态，保存
    mtStateMap.insert(TMAPSTATE::value_type(DeadState, new NFCDeadState(5.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(FightState, new NFCFightState(1.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(IdleState, new NFCIdleState(3.0f, pPluginManager)));
    mtStateMap.insert(TMAPSTATE::value_type(PatrolState, new NFCPatrolState(5.0f, pPluginManager)));


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

NFIState* NFCAIModule::GetState(const int eState)
{
    TMAPSTATE::iterator it = mtStateMap.find((NFAI_STATE)eState);
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
        mtObjectStateMachine.insert(TOBJECTSTATEMACHINE::value_type(self, new NFCStateMachine(self, this)));

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

NFIHateModule* NFCAIModule::GetHateModule()
{
    return m_pHateModule;
}

NFIKernelModule* NFCAIModule::GetKernelModule()
{
    return m_pKernelModule;
}

void NFCAIModule::OnBeKilled(const NFGUID& self, const NFGUID& other)
{
    /*
    这里需要展示一个状态转变规则的应用
    除掉基础状态外，美一个状态都有非常多的规则[或者说所有的状态都是规则]
    当在某个规则[状态]中受到某个事件的时候，进入另外一个规则[状态]
    而且这样是可配置的，在NFIAIRuleConfigModule.h中应该可以查询到规则的转换情况
    那么这样的话，所有的NPC可控规则均是策划配置的规则

    应该来说：状态总共分为几种状态
    静止状态
    [
    出生规则
    休闲规则
    ]
    活跃状态
    [
    巡逻规则
    战斗规则
    追击规则
    开门规则[DoorNpc使用]
    ]
    死亡状态
    [
    自爆规则
    自爆规则
    死亡规则
    ]

    状态内部可以自由切换到其他状态，可以自由切换规则
    而规则则是同一类状态的总称，规则只能一次执行完毕后切换到其他规则最后切换出状态

    规则可以写成到脚本，每个规则都是简单的逻辑
    然后NPC配置中可以配置他拥有几种规则

    */

    TOBJECTSTATEMACHINE::iterator it = mtObjectStateMachine.find(self);
    if (it != mtObjectStateMachine.end())
    {
        it->second->ChangeState(DeadState);
    }
}

void NFCAIModule::OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue)
{
    GetHateModule()->AddHate(self, other, nDamageValue);
    GetHateModule()->CompSortList(self);

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
}

void NFCAIModule::OnEndSpring(const NFGUID& self, const NFGUID& other)
{
    GetHateModule()->SetHateValue(self, other, 0);
}

void NFCAIModule::OnMotion(const NFGUID& self, int nResults)
{
    //运动成功，或失败，都在这里进行一些处理，比如传送等
}

void NFCAIModule::OnSelect(const NFGUID& self, const NFGUID& other)
{
    //选择后可能是接任务啊，或者是对话，或者是搞你
}

void NFCAIModule::OnReload(const char* pstrModuleName, NFIModule* pModule)
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
