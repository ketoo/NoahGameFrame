// -------------------------------------------------------------------------
//    文件名      ：    NFCFightState.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-08 23:26
//    功能描述    ：
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

	NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));
    NFIDENTID ident = m_pHateModule->QueryMaxHateObject(self);
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

                    //添加心跳，还原状态StateType
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
		//目标挂了什么的,或者没目标
		pStateMachine->ChangeState(IdleState);
	}

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

bool NFCFightState::RunInFightArea(const NFIDENTID& self)
{
    //需要回调知道已经走到了,moving事件
    return true;
}

bool NFCFightState::RunCloseTarget(const NFIDENTID& self, const NFIDENTID& target)
{
    return true;
}

int NFCFightState::OnSkillConsumeTime( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
	m_pKernelModule->SetPropertyInt(self, "StateType", (int)NFObjectStateType::NOST_IDLE);
	
	return 0;
}
