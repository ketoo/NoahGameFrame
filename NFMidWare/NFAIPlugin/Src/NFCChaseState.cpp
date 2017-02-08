// -------------------------------------------------------------------------
//    @FileName			:    NFCChaseState.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCChaseState
//    @Desc             :
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"

bool NFCChaseState::Enter(const NFGUID& self)
{
    if (!NFIState::Enter(self))
    {
        // TODO
    }

    return true;
}

bool NFCChaseState::Execute(const NFGUID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //查找是否有可以攻击的对象
			NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));
            NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                // 增加技能是否可以用判断
                if (NFSkillTestSkillResult::NFSTSR_OK == m_pAIModule->CanUseAnySkill(self, ident))
                {
                    pStateMachine->ChangeState(FightState);
                }
                else
                {
                    // 走向目标
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

bool NFCChaseState::Exit(const NFGUID& self)
{
    return true;
}

bool NFCChaseState::DoRule(const NFGUID& self)
{
    return true;
}