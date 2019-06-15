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
#include "NFComm/NFPluginModule/NFIFriendModule.h"

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

	virtual NFIState* GetState(const NFAI_STATE eState);

	virtual const std::string& ChooseSkill(const NFGUID& self, const float fDis);

protected:
	bool CreateAIObject(const NFGUID& self);

    bool DelAIObject(const NFGUID& self);

    //////////////////////////////////////////////////////////////////////////
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);
	int OnNPCHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar);

	NF_SHARE_PTR<NFIStateMachine> GetStateMachine(const NFGUID& self);

private:
    //状态机
	NFIState* mtStateMap[MAXState];
    //AI对象，状态机
    std::map<NFGUID, NF_SHARE_PTR<NFIStateMachine>> mtObjectStateMachine;

    NFIHateModule* m_pHateModule;
	NFIKernelModule* m_pKernelModule;
	NFIElementModule* m_pElementModule;
	NFIFriendModule* m_pFriendModule;
};

#endif