// -------------------------------------------------------------------------
//    文件名      ：    NFIStateMachine.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2011-03-05 22:11
//    功能描述    ：    状态机基类
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "NFCStateMachine.h"
#include "NFCAIModule.h"

NFCStateMachine::NFCStateMachine(const NFIDENTID& self, NFIAIModule* pControl)
    : mOwnerID(self),
      meCurrentState(IdleState),
      meLastState(IdleState),
      m_pAIControlInterface(pControl)
{
    NFIState* pState = m_pAIControlInterface->GetState(CurrentState());
    mfHeartBeatTime = pState->GetHeartBeatTime();
}

NFCStateMachine::~NFCStateMachine()
{

}

void NFCStateMachine::UpData(float fFrameTime, float fAllTime)
{
    //same for the current state
    if (State_Error != meCurrentState)
    {
        if (mfHeartBeatTime > 0)
        {
            mfHeartBeatTime -= fFrameTime;
        }
        else
        {
            NFIState* pState = m_pAIControlInterface->GetState(meCurrentState);
            pState->Execute(mOwnerID);

            //设置心跳时间
            float fRandTime = (float)(rand() / double(RAND_MAX));

            mfHeartBeatTime = pState->GetHeartBeatTime() + fRandTime;
        }
    }
}

void NFCStateMachine::ChangeState(const NFAI_STATE eNewState)
{
    meLastState = meCurrentState;

    NFIState* pState = m_pAIControlInterface->GetState(meCurrentState);
    pState->Exit(mOwnerID);

    meCurrentState = eNewState;

    pState = m_pAIControlInterface->GetState(meCurrentState);
    pState->Enter(mOwnerID);

    //心跳
    mfHeartBeatTime = pState->GetHeartBeatTime();
}

void NFCStateMachine::RevertToLastState()
{
    ChangeState(meLastState);
}