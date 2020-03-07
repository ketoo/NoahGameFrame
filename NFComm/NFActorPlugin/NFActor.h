/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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



#ifndef NF_ACTOR_H
#define NF_ACTOR_H

#include <map>
#include <string>
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFActor
    : public NFIActor
{
public:
	NFActor(const NFGUID id, NFIActorModule* pModule);
	virtual ~NFActor();

	const NFGUID ID();
	
    virtual bool Execute();

    virtual bool AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool RemoveComponent(const std::string& strComponentName);
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName);

    virtual bool SendMsg(const NFActorMessage& message);
    virtual bool SendMsg(const int eventID, const std::string& data, const std::string& arg);
    virtual bool BackMsgToMainThread(const NFActorMessage& message);

    virtual bool AddMessageHandler(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor);

    virtual void ToMemoryCounterString(std::string& info);
protected:
	NFGUID id;

	NFIActorModule* m_pActorModule;

    NFQueue<NFActorMessage> mMessageQueue;

	NFMapEx<std::string, NFIComponent> mComponent;

	NFMapEx<int, ACTOR_PROCESS_FUNCTOR> mxProcessFunctor;
};
#endif