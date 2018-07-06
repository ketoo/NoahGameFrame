/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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
#ifndef NFC_STATE_MACHINE_H
#define NFC_STATE_MACHINE_H

#include "NFCDeadState.h"
#include "NFCFightState.h"
#include "NFCIdleState.h"
#include "NFCPatrolState.h"
#include "NFCChaseState.h"
#include "NFIStateMachine.h"
#include "NFCAIModule.h"

class NFCStateMachine
    : public NFIStateMachine
{
public:

    NFCStateMachine(const NFGUID& self, NFIPluginManager* p);

    virtual ~NFCStateMachine();

    virtual bool Execute();


	virtual NFIState* GetState(const NFAI_STATE eState) const;
	
    //change to a new state
    virtual void ChangeState(const NFAI_STATE eState);

    virtual void RevertToLastState();

    virtual const NFAI_STATE CurrentState() const
    {
        return meCurrentState;
    }

    virtual const NFAI_STATE LastState() const
    {
        return meLastState;
    }


private:
    NFGUID mOwnerID;
    float mfHeartBeatTime;
    NFAI_STATE meCurrentState;
    NFAI_STATE meLastState;

protected:
	NFIAIModule* m_pAIModule;
	NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIMoveModule* m_pMoveModule;
	NFIElementModule* m_pElementModule;
};

#endif