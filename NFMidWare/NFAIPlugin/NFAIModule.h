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

#ifndef NF_AI_MODULE_H
#define NF_AI_MODULE_H

#include "NFStateMachine.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMoveModule.h"
#include "NFComm/NFPluginModule/NFIAIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHateModule.h"

class NFAIModule
    : public NFIAIModule
{
public:
    NFAIModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();

    virtual bool Shut();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Execute();

protected:
	int CanUseAnySkill(const NFGUID& self, const NFGUID& other);

	float UseAnySkill(const NFGUID& self, const NFGUID& other);
    
	bool CreateAIObject(const NFGUID& self);

    bool DelAIObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    int OnAIObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    //////////////////////////////////////////////////////////////////////////

    void OnBeKilled(const NFGUID& self, const NFGUID& other);

    void OnBeAttack(const NFGUID& self, const NFGUID& other, const int nDamageValue);

    void OnSpring(const NFGUID& self, const NFGUID& other);

    void OnEndSpring(const NFGUID& self, const NFGUID& other);

    void OnMotion(const NFGUID& self, int nResults);

    void OnSelect(const NFGUID& self, const NFGUID& other);

	NF_SHARE_PTR<NFIStateMachine> GetStateMachine(const NFGUID& self);

	NFIState* GetState(const NFAI_STATE eState);

private:
    //状态机
	NFIState* mtStateMap[MAXState];
    //AI对象，状态机
    std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>> mtObjectStateMachine;

    NFIHateModule* m_pHateModule;
    NFIKernelModule* m_pKernelModule;
};

#endif