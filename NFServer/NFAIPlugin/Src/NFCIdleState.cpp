// -------------------------------------------------------------------------
//    文件名      ：    NFCIdleState.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：   2011-03-08 23:26
//    功能描述    ：   cpp
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"
#include "../NFAIPlugin.h"

NFCIdleState::NFCIdleState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(IdleState, fHeartBeatTime, p)
{
    //任何对象出生后先进入Idle状态，这里初始化静态变量指针

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pAIModule = dynamic_cast<NFIAIModule*>(pPluginManager->FindModule("NFCAIModule"));
    m_pMoveModule = dynamic_cast<NFIMoveModule*>(pPluginManager->FindModule("NFCMoveModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));

    m_pHateModule = m_pAIModule->GetHateModule();
}

NFCIdleState::~NFCIdleState()
{

}

bool NFCIdleState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            switch (pStateMachine->LastState())
            {
                case NFAI_STATE::FightState:
                    //找人继续打
                    Execute(self);
                    break;

                default:
                    break;
            }
        }
    }

    return true;
}

bool NFCIdleState::Execute(const NFIDENTID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //查找是否有可以攻击的对象
            NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                pStateMachine->ChangeState(FightState);
            }
            else
            {
                if (m_pKernelModule->GetPropertyInt(self, "MoveSpeed") > 0)
                {
                    RandomIdle(pStateMachine);
                }
            }
        }
    }

    return true;
}

bool NFCIdleState::Exit(const NFIDENTID& self)
{


    return true;
}

bool NFCIdleState::DoRule(const NFIDENTID& self)
{
    return true;
}

bool NFCIdleState::RandomIdle(NFIStateMachine* pStateMachine)
{
    //随机进入休闲或者继续巡逻
    float fRand = (float)(rand() / double(RAND_MAX));
    if (fRand < 0.4f)
    {
        pStateMachine->ChangeState(PatrolState);
        return true;
    }

    return false;
}
