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

#ifndef NFI_COMPONENT_H
#define NFI_COMPONENT_H

#include "NFPlatform.h"
#include "NFGUID.h"
#include "NFIModule.h"
#include "NFComm/NFCore/NFMemoryCounter.h"

class NFActorMessage;
class NFIComponent;

typedef std::function<void(NFActorMessage&)> ACTOR_PROCESS_FUNCTOR;
typedef NF_SHARE_PTR<ACTOR_PROCESS_FUNCTOR> ACTOR_PROCESS_FUNCTOR_PTR;

class NFActorMessage
{
public:
	NFActorMessage()
	{
		msgID = 0;
		index = 0;
	}

	int msgID;
	uint64_t index;
    NFGUID id;
	std::string data;
	std::string arg;
protected:
private:
};

class NFIActor : NFMemoryCounter
{
public:
	NFIActor()
		: NFMemoryCounter(GET_CLASS_NAME(NFIActor), 1)
	{

	}

	virtual ~NFIActor() {}
	virtual const NFGUID ID() = 0;
    virtual bool Execute() = 0;

	template <typename T>
	NF_SHARE_PTR<T> AddComponent()
	{
		NF_SHARE_PTR<NFIComponent> component = FindComponent(typeid(T).name());
		if (component)
		{
			return NULL;
		}

		{
			if (!TIsDerived<T, NFIComponent>::Result)
			{
				return NULL;
			}

			NF_SHARE_PTR<T> pComponent = NF_SHARE_PTR<T>(NF_NEW T());

			assert(NULL != pComponent);

			AddComponent(pComponent);

			return pComponent;
		}

		return nullptr;
	}



	template <typename T>
	NF_SHARE_PTR<T> FindComponent()
	{
		NF_SHARE_PTR<NFIComponent> component = FindComponent(typeid(T).name());
		if (component)
		{
			NF_SHARE_PTR<T> pT = std::dynamic_pointer_cast<T>(component);

			assert(NULL != pT);

			return pT;
		}

		return nullptr;
	}
	
	template <typename T>
	bool RemoveComponent()
	{
		return RemoveComponent(typeid(T).name());
	}

	virtual bool SendMsg(const NFActorMessage& message) = 0;
	virtual bool SendMsg(const int eventID, const std::string& data, const std::string& arg = "") = 0;
	virtual bool BackMsgToMainThread(const NFActorMessage& message) = 0;

	virtual bool AddMessageHandler(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR xBeginFunctor) = 0;

protected:
	virtual bool AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool RemoveComponent(const std::string& strComponentName) = 0;
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;

};

class NFIComponent : NFMemoryCounter
{
private:
    NFIComponent()
		: NFMemoryCounter(GET_CLASS_NAME(NFIComponent), 1)
    {
    }

public:
    NFIComponent(const std::string& strName)
		: NFMemoryCounter(strName, 1)
    {
        mbEnable = true;
        mstrName = strName;
    }

    virtual ~NFIComponent() {}

	virtual void SetActor(NF_SHARE_PTR<NFIActor> self)
	{
		mSelf = self;
	}

	virtual NF_SHARE_PTR<NFIActor> GetActor()
	{
		return mSelf;
	}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Init()
	{

		return true;
	}

	virtual bool AfterInit()
	{
		return true;
	}

	virtual bool CheckConfig()
	{
		return true;
	}

	virtual bool ReadyExecute()
	{
		return true;
	}

	virtual bool Execute()
	{
		return true;
	}

	virtual bool BeforeShut()
	{
		return true;
	}

	virtual bool Shut()
	{
		return true;
	}

	virtual bool Finalize()
	{
		return true;
	}

    virtual bool SetEnable(const bool bEnable)
    {
        mbEnable = bEnable;

        return mbEnable;
    }

    virtual bool Enable()
    {
        return mbEnable;
    }

    virtual const std::string& GetComponentName() const
    {
        return mstrName;
    };

	virtual void ToMemoryCounterString(std::string& info)
	{
		info.append(mSelf->ID().ToString());
		info.append(":");
		info.append(mstrName);
	}

	template<typename BaseType>
	bool AddMsgHandler(const int nSubMessage, BaseType* pBase, int (BaseType::*handler)(NFActorMessage&))
	{
		ACTOR_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr(new ACTOR_PROCESS_FUNCTOR(functor));
		
		return mSelf->AddMessageHandler(nSubMessage, functorPtr);
	}

private:
    bool mbEnable;
	NF_SHARE_PTR<NFIActor> mSelf;
    std::string mstrName;
};

#endif