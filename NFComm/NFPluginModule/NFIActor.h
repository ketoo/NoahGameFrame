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

#ifndef NFI_ACTOR_H
#define NFI_ACTOR_H

#include <map>
#include <string>
#include "NFIModule.h"

class NFIComponent;
class NFIActorModule;
class NFActorMessage;

typedef std::function<void(NFActorMessage&)> ACTOR_PROCESS_FUNCTOR;
typedef NF_SHARE_PTR<ACTOR_PROCESS_FUNCTOR> ACTOR_PROCESS_FUNCTOR_PTR;

class NFActorMessage
{
public:
	NFActorMessage()
	{
		msgID = 0;
	}

	int msgID;
    NFGUID id;
	std::string data;
protected:
private:
};

class NFIActor
{
public:
	virtual ~NFIActor() {}
	virtual const NFGUID ID() = 0;
    virtual bool Execute() = 0;
    
	virtual bool AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool RemoveComponent(const std::string& strComponentName) = 0;
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;
	
	virtual bool AddMessageHandler(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor) = 0;

	virtual bool SendMsg(const NFActorMessage& message) = 0;
};

#endif