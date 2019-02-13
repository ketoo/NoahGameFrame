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

#ifndef NFI_ACTOR_MODULE_H
#define NFI_ACTOR_MODULE_H

#include "NFIModule.h"
#include "NFIActor.h"
#include "NFIComponent.h"

///////////////////////////////////////////////////

class NFIComponent;

class NFIActorModule : public NFIModule
{
public:
    template<typename TypeComponent>
    int RequireActor()
    {
        if (!TIsDerived<TypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return 0;
        }

        int nActorID = RequireActor();
        if (nActorID >= 0)
        {
            AddComponent<TypeComponent>(nActorID);

            return nActorID;
        }

        return -1;
    }

	template<typename TypeComponent, typename BaseType>
	int RequireActor(BaseType* pBase, int (BaseType::*handler_end)(const int, const int, const std::string&))
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return 0;
		}

		int nActorID = RequireActor();
		if (nActorID >= 0)
		{
			AddComponent<TypeComponent>(nActorID);
			AddDefaultEndFunc<BaseType>(nActorID, pBase, handler_end);

			return nActorID;
		}

		return -1;
	}
	template<typename TypeComponent>
	bool AddComponent(const int nActorIndex)
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return false;
		}

		NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
		if (pActor)
		{
			//use CreateNewInstance to replace this line to create a new component script
			NF_SHARE_PTR<TypeComponent> pComponent = NF_SHARE_PTR<TypeComponent>(NF_NEW TypeComponent());

			//GET_CLASS_NAME(TypeComponent);

			return AddComponent(nActorIndex, pComponent);
		}

		return false;
	}

	template<typename BaseType>
	int AddEndFunc(const int nActorIndex, const int nSubMessageID,
		BaseType* pBase, int (BaseType::*handler_end)(const int, const int, const std::string&))
	{
		ACTOR_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end(new ACTOR_PROCESS_FUNCTOR(functor_end));

		return AddEndFunc(nActorIndex, nSubMessageID, functorPtr_end);
	}
	template<typename BaseType>
	int AddDefaultEndFunc(const int nActorIndex,
		BaseType* pBase, int (BaseType::*handler_end)(const int, const int, const std::string&))
	{
		ACTOR_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end(new ACTOR_PROCESS_FUNCTOR(functor_end));

		return AddDefaultEndFunc(nActorIndex, functorPtr_end);
	}
	virtual int RequireActor() = 0;
    virtual bool SendMsgToActor(const int nActorIndex, const int nEventID, const std::string& strArg) = 0;
	virtual bool HandlerEx(const NFIActorMessage& message, const int from) = 0;
	virtual bool ReleaseActor(const int nActorIndex) = 0;
	virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex) = 0;

protected:
    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent) = 0;

	virtual bool AddDefaultEndFunc(const int nActorIndex, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end) = 0;
	virtual bool AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end) = 0;
};

#endif
