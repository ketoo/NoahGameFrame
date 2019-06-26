/*
    Copyright 2009 - 2019 NoahGameFrame/NoahFrame

   This source file is part of NoahGameFrame/NoahFrame.
   NoahGameFrame/NoahFrame is open-source software and you can redistribute it and/or modify
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

#ifndef NFI_STATE_H
#define NFI_STATE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

//所有的状态
enum NFAI_STATE
{
    IdleState,                  // 休闲状态
    PatrolState,                // 巡逻状态
    FightState,                 // 战斗状态
    ChaseState,                 // 追击状态
	DeadState,                  // 死亡状态
	MAXState,                  // MAX
};

class NFIStateMachine;

class NFIState
	: public NFIModule
{
public:
    NFIState(NFAI_STATE eState, const float fHeartBeatTime, NFIPluginManager* p)
    {
        meState = eState;
        mfHeartBeatTime = fHeartBeatTime;
        pPluginManager = p;
    }

    virtual ~NFIState()
    {
    }

    virtual bool DoRule(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        //返回true表示基类处理过，上面就别处理了
        return false;
    }

    virtual bool Enter(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    virtual bool Execute(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    virtual bool Exit(const NFGUID& self, NFIStateMachine* pStateMachine)
    {
        return false;
    }

    const NFAI_STATE GetCurrState()
    {
        return meState;
    }

    float GetHeartBeatTime()
    {
        return mfHeartBeatTime;
    }

protected:
    NFAI_STATE  meState;
    float		mfHeartBeatTime;
};

#endif