#include "../NFCAIModule.h"

bool NFCChaseState::Enter(const NFIDENTID& self)
{
    if (!NFIState::Enter(self))
    {
        // TODO
    }

    return true;
}

bool NFCChaseState::Execute(const NFIDENTID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //�����Ƿ��п��Թ����Ķ���
			NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));
            NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                // ���Ӽ����Ƿ�������ж�
                if (NFSkillTestSkillResult::NFSTSR_OK == m_pAIModule->CanUseAnySkill(self, ident))
                {
                    pStateMachine->ChangeState(FightState);
                }
                else
                {
                    // ����Ŀ��
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

    return true;
}

bool NFCChaseState::Exit(const NFIDENTID& self)
{
    return true;
}

bool NFCChaseState::DoRule(const NFIDENTID& self)
{
    return true;
}