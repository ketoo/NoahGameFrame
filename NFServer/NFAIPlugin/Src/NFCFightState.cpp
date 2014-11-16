// -------------------------------------------------------------------------
//    �ļ���      ��    NFCFightState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-08 23:26
//    ��������    ��
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "../NFCAIModule.h"

bool NFCFightState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        Execute(self);
    }

    return true;
}

bool NFCFightState::Execute(const NFIDENTID& self)
{
    NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);

    NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
    if (!ident.IsNull())
    {
        if (m_pKernelModule->QueryPropertyInt(self, "HP") > 0)
        {
            if (m_pKernelModule->QueryPropertyInt(ident, "HP") > 0)
            {
                NFSkillTestSkillResult eResult = (NFSkillTestSkillResult)m_pAIModule->CanUseAnySkill(self, ident);
                if (NFSkillTestSkillResult::NFSTSR_OK == eResult)
                {
                    float fSkillConsumeTime = m_pAIModule->UseAnySkill(self, ident);
                    m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_SKILLUSE);
                    //�����������ԭ״̬StateType
                    m_pKernelModule->AddHeartBeat(self, "OnSkillConsumeTime", OnSkillConsumeTime, NFCValueList(), fSkillConsumeTime, 1);
                }
                else if (NFSkillTestSkillResult::NFSTSR_DISTANCE == eResult)
                {
                    RunCloseTarget(self);
                }
                else if (NFSkillTestSkillResult::NFSTSR_CD == eResult)
                {
                    RunInFightArea(self);
                }
            }
            else
            {
                m_pHateModule->SetHateValue(self, ident, 0);
            }
        }
        else
        {
            pStateMachine->ChangeState(DeadState);
        }
    }

    //Ŀ�����ʲô��
    pStateMachine->ChangeState(IdleState);


    return true;
}

bool NFCFightState::Exit(const NFIDENTID& self)
{

    return true;
}

bool NFCFightState::DoRule(const NFIDENTID& self)
{
    return true;
}

int NFCFightState::OnSkillConsumeTime(const NFIDENTID& self, const NFIValueList& var)
{
    m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_IDEL);
    return 0;
}

bool NFCFightState::RunInFightArea(const NFIDENTID& self)
{
    //��Ҫ�ص�֪���Ѿ��ߵ���,moving�¼�
    return true;
}

bool NFCFightState::RunCloseTarget(const NFIDENTID& self)
{
    //��Ҫ�ص�֪���Ѿ��ߵ���,moving�¼�
    return true;
}
