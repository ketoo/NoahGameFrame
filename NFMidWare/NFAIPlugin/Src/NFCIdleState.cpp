// -------------------------------------------------------------------------
//    @FileName			:    NFCIdleState.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCIdleState
//    @Desc             :
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"
#include "../NFAIPlugin.h"

NFCIdleState::NFCIdleState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(IdleState, fHeartBeatTime, p)
{
    //任何对象出生后先进入Idle状态，这里初始化静态变量指针

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pAIModule = dynamic_cast<NFIAIModule*>(pPluginManager->FindModule("NFCAIModule"));
    m_pMoveModule = dynamic_cast<NFIMoveModule*>(pPluginManager->FindModule("NFCMoveModule"));
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");

    m_pHateModule = m_pAIModule->GetHateModule();
}

NFCIdleState::~NFCIdleState()
{

}

bool NFCIdleState::Enter(const NFGUID& self)
{
    if (!NFIState::Enter(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
			//看是否有战斗能力
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

bool NFCIdleState::Execute(const NFGUID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //查找是否有可以攻击的对象
            NFGUID ident = m_pHateModule->QueryMaxHateObject(self);
            if (!ident.IsNull())
            {
                pStateMachine->ChangeState(FightState);
            }
            else
            {
                if (m_pKernelModule->GetPropertyInt(self, "MoveSpeed") > 0)
                {
                    RandomIdle(self, pStateMachine);
                }
            }
        }
    }

    return true;
}

bool NFCIdleState::Exit(const NFGUID& self)
{


    return true;
}

bool NFCIdleState::DoRule(const NFGUID& self)
{
    return true;
}

bool NFCIdleState::RandomIdle(const NFGUID& self, NFIStateMachine* pStateMachine)
{
	//如果是定点的，则不走，继续idle
	NFAI_MOVE_TYPE eMoveType = (NFAI_MOVE_TYPE)(m_pKernelModule->GetPropertyInt(self, "MoveType"));

	switch (eMoveType)
	{
	case NFAI_MOVE_TYPE::MOVE_BY_POINT_LIST:
	case NFAI_MOVE_TYPE::MOVE_BY_RANDOM:
		{
			float fRand = (float)(rand() / double(RAND_MAX));
			if (fRand < 0.4f)
			{
				pStateMachine->ChangeState(PatrolState);
			}
		}
		break;

	default:
		break;
	}

    return false;
}
