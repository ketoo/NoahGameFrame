// -------------------------------------------------------------------------
//    �ļ���      ��    NFCFightState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2011-03-08 23:26
//    ��������    ��
//
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"

bool NFCFightState::Enter(const NFGUID& self)
{
    if (!NFIState::Enter(self))
    {
        Execute(self);
    }

    return true;
}

bool NFCFightState::Execute(const NFGUID& self)
{
    NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);

	NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));
    NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
    if (!ident.IsNull())
    {
        if (m_pKernelModule->GetPropertyInt(self, "HP") > 0)
        {
            if (m_pKernelModule->GetPropertyInt(ident, "HP") > 0)
            {
                NFSkillTestSkillResult eResult = (NFSkillTestSkillResult)m_pAIModule->CanUseAnySkill(self, ident);
                if (NFSkillTestSkillResult::NFSTSR_OK == eResult)
                {
                    float fSkillConsumeTime = m_pAIModule->UseAnySkill(self, ident);
                    m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_SKILLUSE);

                    //�����������ԭ״̬StateType
                    m_pKernelModule->AddHeartBeat(self, "OnSkillConsumeTime", this, &NFCFightState::OnSkillConsumeTime, fSkillConsumeTime, 1);
                }
                else if (NFSkillTestSkillResult::NFSTSR_DISTANCE == eResult)
                {
					if (NFAI_MOVE_TYPE::NO_MOVE_TYPE != eMoveType)
					{
						RunCloseTarget(self, ident);
					}
					else
					{
						pStateMachine->ChangeState(IdleState);
					}
                }
                else if (NFSkillTestSkillResult::NFSTSR_CD == eResult)
                {
					if (NFAI_MOVE_TYPE::NO_MOVE_TYPE != eMoveType)
					{
						RunInFightArea(self);
					}
					else
					{
						pStateMachine->ChangeState(IdleState);
					}
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
	else
	{
		//Ŀ�����ʲô��,����ûĿ��
		pStateMachine->ChangeState(IdleState);
	}

    return true;
}

bool NFCFightState::Exit(const NFGUID& self)
{

    return true;
}

bool NFCFightState::DoRule(const NFGUID& self)
{
    return true;
}

bool NFCFightState::RunInFightArea(const NFGUID& self)
{
    //��Ҫ�ص�֪���Ѿ��ߵ���,moving�¼�
    return true;
}

bool NFCFightState::RunCloseTarget(const NFGUID& self, const NFGUID& target)
{
    return true;
}

int NFCFightState::OnSkillConsumeTime( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
	m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_IDLE);
	
	return 0;
}
