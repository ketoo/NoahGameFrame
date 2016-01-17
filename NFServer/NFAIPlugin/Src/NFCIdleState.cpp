// -------------------------------------------------------------------------
//    �ļ���      ��    NFCIdleState.cpp
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��   2011-03-08 23:26
//    ��������    ��   cpp
//
// -------------------------------------------------------------------------

#include "../NFCAIModule.h"
#include "../NFAIPlugin.h"

NFCIdleState::NFCIdleState(float fHeartBeatTime, NFIPluginManager* p)
    : NFIState(IdleState, fHeartBeatTime, p)
{
    //�κζ���������Ƚ���Idle״̬�������ʼ����̬����ָ��

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
			//���Ƿ���ս������
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

bool NFCIdleState::Execute(const NFGUID& self)
{
    if (!NFIState::Execute(self))
    {
        NFIStateMachine* pStateMachine = m_pAIModule->GetStateMachine(self);
        if (pStateMachine)
        {
            //�����Ƿ��п��Թ����Ķ���
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
	//����Ƕ���ģ����ߣ�����idle
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
