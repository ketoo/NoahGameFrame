// -------------------------------------------------------------------------
//    @FileName			:    NFCDeadState.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCDeadState
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFIStateMachine.h"
#include "NFCDeadState.h"

NFCDeadState::NFCDeadState(float fHeartBeatTime, NFIPluginManager* p)
	: NFIState(DeadState, fHeartBeatTime, p)
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

bool NFCDeadState::Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Enter(self, pStateMachine))
    {
        m_pHateModule->ClearHate(self);
    }

    return true;
}

bool NFCDeadState::Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    if (!NFIState::Execute(self, pStateMachine))
    {
        //¼¸ÃëºóÉ¾³ýÊ¬Ìå
        m_pKernelModule->DestroyObject(self);
    }

    return true;
}

bool NFCDeadState::Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
{


    return true;
}

bool NFCDeadState::DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
{
    return true;
}