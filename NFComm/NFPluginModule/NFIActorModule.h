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

#ifndef NFI_ACTOR_MODULE_H
#define NFI_ACTOR_MODULE_H

#include "NFIModule.h"
#include "NFIComponent.h"

///////////////////////////////////////////////////

class NFIActorModule : public NFIModule
{
public:
	template<typename TypeComponent>
	bool AddComponent(const NFGUID nActorIndex)
	{
		if (!TIsDerived<TypeComponent, NFIComponent>::Result)
		{
			//BaseTypeComponent must inherit from NFIComponent;
			return false;
		}

		NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
		if (pActor)
		{
			auto component = pActor->AddComponent<TypeComponent>();
			if (component)
			{
				return true;
			}
		}

		return false;
	}

	template<typename BaseType>
	int AddEndFunc(const int subMessageID, BaseType* pBase, void (BaseType::*handler_end)(NFActorMessage&))
	{
		ACTOR_PROCESS_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1);
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end(new ACTOR_PROCESS_FUNCTOR(functor_end));

		return AddEndFunc(subMessageID, functorPtr_end);
	}

    int AddEndFunc(const int subMessageID, ACTOR_PROCESS_FUNCTOR functor_end)
	{
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end(new ACTOR_PROCESS_FUNCTOR(functor_end));

		return AddEndFunc(subMessageID, functorPtr_end);
	}

	virtual NF_SHARE_PTR<NFIActor> RequireActor() = 0;
	virtual NF_SHARE_PTR<NFIActor> GetActor(const NFGUID nActorIndex) = 0;
	virtual bool ReleaseActor(const NFGUID nActorIndex) = 0;

	virtual bool SendMsgToActor(const NFGUID actorIndex, const NFGUID who, const int eventID, const std::string& data, const std::string& arg = "") = 0;

	//only be called by actor's processor
    virtual bool AddResult(const NFActorMessage& message) = 0;

protected:

	virtual bool AddEndFunc(const int subMessageID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end) = 0;
};

#endif
