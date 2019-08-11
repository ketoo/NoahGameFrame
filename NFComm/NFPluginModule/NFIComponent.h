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

#ifndef NFI_COMPONENT_H
#define NFI_COMPONENT_H

#include "NFPlatform.h"
#include "NFGUID.h"
#include "NFIActor.h"

class NFIComponent
{
private:
    NFIComponent()
    {
    }

public:
    NFIComponent(const std::string& strName)
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


	template<typename BaseType>
	bool AddMsgHandler(const int nSubMessage, BaseType* pBase, int (BaseType::*handler)(NFActorMessage&))
	{
		ACTOR_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr(new ACTOR_PROCESS_FUNCTOR(functor));
		
		return mSelf->AddMessageHandler(nSubMessage, functorPtr);
	}

	template <typename T>
	NF_SHARE_PTR<T> FindComponent(const std::string& strName)
	{
		if (!TIsDerived<T, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return NF_SHARE_PTR<T>();
		}

		return mSelf->FindComponent(strName);
	}

private:
    bool mbEnable;
	NF_SHARE_PTR<NFIActor> mSelf;
    std::string mstrName;
};

#endif