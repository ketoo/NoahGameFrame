/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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



#ifndef NFC_ACTOR_H
#define NFC_ACTOR_H

#include <map>
#include <string>
#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

class NFCActor
    : public NFIActor
{
public:
	NFCActor(Theron::Framework& framework, NFIActorModule* pModule);
	virtual ~NFCActor();

	
    virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName);

	virtual bool AddBeginFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor);
	virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor);
	virtual bool AddDefaultEndFunc(ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor);

    virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message);

protected:
	//handler in component
	virtual void Handler(const NFIActorMessage& message, const Theron::Address from);

	//handler in main thread, the purpose is to push message to main thread
	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);

protected:
	NFMapEx<std::string, NFIComponent> mxComponent;

	NFMapEx<int, ACTOR_PROCESS_FUNCTOR> mxProcessFuntor;
	NFMapEx<int, ACTOR_PROCESS_FUNCTOR> mxEndProcessFuntor;

	ACTOR_PROCESS_FUNCTOR_PTR mxDefaultEndProcessFuntor;
};
#endif