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

#ifndef NFI_ACTOR_H
#define NFI_ACTOR_H

#define THERON_USE_STD_THREADS 1

#include <map>
#include <string>
#include <Theron/Theron.h>
#include "NFIModule.h"

class NFIComponent;
class NFIActorModule;

typedef std::function<int(const int, const int, std::string&)> ACTOR_PROCESS_FUNCTOR;
typedef NF_SHARE_PTR<ACTOR_PROCESS_FUNCTOR> ACTOR_PROCESS_FUNCTOR_PTR;


class NFIActorMessage
{
public:
	enum MessageType
	{
		ACTOR_MSG_TYPE_COMPONENT,
		ACTOR_MSG_TYPE_END_FUNC,
		ACTOR_MSG_TYPE_NET_MSG,
	};

	NFIActorMessage()
	{
		nMsgID = 0;
		nFormActor = 0;
		msgType = ACTOR_MSG_TYPE_COMPONENT;
	}

	int nMsgID;
	int nFormActor;
	std::string data;
	MessageType msgType;
	////////////////////event/////////////////////////////////////////////////
	ACTOR_PROCESS_FUNCTOR_PTR xEndFuncptr;
protected:
private:
};

class NFIActor : public Theron::Actor
{
public:
    NFIActor(Theron::Framework& framework) : Theron::Actor(framework)
    {
        RegisterHandler(this, &NFIActor::DefaultHandler);
    }

	virtual ~NFIActor() {}

	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;

	template<typename T>
	NF_SHARE_PTR<T> FindComponent(const std::string& strName)
	{
		if (!TIsDerived<T, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return nullptr;
		}

		NF_SHARE_PTR<NFIComponent> pComponent = FindComponent(strName);
		NF_SHARE_PTR<T> pT = std::dynamic_pointer_cast<T>(pComponent);
		if (nullptr != pT)
		{
			return pT;
		}

		return nullptr;
	}
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;
	
	virtual bool AddBeginFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor) = 0;
	virtual bool AddEndFunc(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor) = 0;
	virtual bool AddDefaultEndFunc(ACTOR_PROCESS_FUNCTOR_PTR xEndFunctor) = 0;

	virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message) = 0;

protected:

    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from) {};
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from) {};

private:
    void DefaultHandler(const NFIActorMessage& message, const Theron::Address from)
    {
        if (message.msgType == NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT)
        {
			Handler(message, from);
        }
        else
        {
            HandlerEx(message, from);
        }
    }

protected:
	NFIActorModule* m_pActorModule;

};

#endif