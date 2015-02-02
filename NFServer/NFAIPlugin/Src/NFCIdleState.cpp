// -------------------------------------------------------------------------
//    �ļ���      ��    NFCIdleState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��   2011-03-08 23:26
//    ��������    ��   cpp
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"
#include "../NFAIPlugin.h"

NFCIdleState::NFCIdleState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(IdleState, fHeartBeatTime, p)
{
    //�κζ���������Ƚ���Idle״̬�������ʼ����̬����ָ��

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
                    //���˼�����
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
            //�����Ƿ��п��Թ����Ķ���
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
    //����������л��߼���Ѳ��
    float fRand = (float)(rand() / double(RAND_MAX));
    if (fRand < 0.4f)
    {
        pStateMachine->ChangeState(PatrolState);
        return true;
    }

    return false;
}
