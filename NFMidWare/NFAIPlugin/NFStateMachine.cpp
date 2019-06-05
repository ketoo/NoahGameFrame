/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFStateMachine.h"

NFStateMachine::NFStateMachine(const NFGUID& self, NFIPluginManager* p)
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

NFStateMachine::~NFStateMachine()
{

}

bool NFStateMachine::Execute()
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

NFIState * NFStateMachine::GetState(const NFAI_STATE eState) const
{
	return nullptr;
}

void NFStateMachine::ChangeState(const NFAI_STATE eNewState)
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

void NFStateMachine::RevertToLastState()
{
    ChangeState(meLastState);
}