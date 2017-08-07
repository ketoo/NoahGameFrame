// -------------------------------------------------------------------------
//    @FileName			:    NFCStateMachine.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-08
//    @Module           :    NFCStateMachine
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCStateMachine.h"

NFCStateMachine::NFCStateMachine(const NFGUID& self, NFIPluginManager* p)
    : mOwnerID(self),
      meCurrentState(PatrolState),
      meLastState(IdleState)
{
	pPluginManager = p;

    NFIState* pState = GetState(CurrentState());
    mfHeartBeatTime = pState->GetHeartBeatTime();

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pAIModule = pPluginManager->FindModule<NFIAIModule>();
	m_pMoveModule = pPluginManager->FindModule<NFIMoveModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pHateModule = pPluginManager->FindModule<NFIHateModule>();
}

NFCStateMachine::~NFCStateMachine()
{

}

bool NFCStateMachine::Execute()
{
    //same for the current state
    if (State_Error != meCurrentState)
    {
        if (mfHeartBeatTime > 0)
        {
            mfHeartBeatTime -= 0.001f;
        }
        else
        {
            NFIState* pState = GetState(meCurrentState);
            pState->Execute(mOwnerID, this);

            //设置心跳时间
            NFDataList xDataList;
            m_pKernelModule->Random(0, 10, 1, xDataList);

            mfHeartBeatTime = pState->GetHeartBeatTime() + xDataList.Int(0);
        }
    }

	return true;
}

NFIState * NFCStateMachine::GetState(const NFAI_STATE eState) const
{
	return nullptr;
}

void NFCStateMachine::ChangeState(const NFAI_STATE eNewState)
{
    meLastState = meCurrentState;

    NFIState* pState = GetState(meCurrentState);
    pState->Exit(mOwnerID, this);

    meCurrentState = eNewState;

    pState = GetState(meCurrentState);
    pState->Enter(mOwnerID, this);

    //心跳
    mfHeartBeatTime = pState->GetHeartBeatTime();
}

void NFCStateMachine::RevertToLastState()
{
    ChangeState(meLastState);
}