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

#ifndef NFI_STATE_MACHINE_H
#define NFI_STATE_MACHINE_H

#include "NFIState.h"
#include "NFIdleState.h"
#include "NFFightState.h"
#include "NFPatrolState.h"
#include "NFDeadState.h"
#include "NFChaseState.h"

class NFIStateMachine
	: public NFIModule
{
public:

    virtual const NFAI_STATE  CurrentState() const = 0;
	virtual const NFAI_STATE  LastState() const = 0;
	virtual NFIState* GetState(const NFAI_STATE eState) = 0;

    //change to a new state
    virtual void ChangeState(const NFAI_STATE eState) = 0;

    //return last state
    virtual void RevertToLastState() = 0;

protected:

private:
};

#endif