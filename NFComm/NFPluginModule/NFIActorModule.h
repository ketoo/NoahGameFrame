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
    NFGUID RequireActor()
    {
        if (!TIsDerived<TypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return NFGUID();
        }

        NFGUID nActorID = RequireActor();
        if (!nActorID.IsNull())
        {
            AddComponent<TypeComponent>(nActorID);

            return nActorID;
        }

        return NFGUID();
    }

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
			//use CreateNewInstance to replace this line to create a new component script
			NF_SHARE_PTR<TypeComponent> pComponent = NF_SHARE_PTR<TypeComponent>(NF_NEW TypeComponent());

			//GET_CLASS_NAME(TypeComponent);

			return AddComponent(nActorIndex, pComponent);
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

	virtual NFGUID RequireActor() = 0;
	virtual NF_SHARE_PTR<NFIActor> GetActor(const NFGUID nActorIndex) = 0;
	virtual bool ReleaseActor(const NFGUID nActorIndex) = 0;

    virtual bool SendMsgToActor(const NFGUID nActorIndex, const int messageID, const std::string& strArg) = 0;

	//only be called by actor's processor
    virtual bool AddResult(const NFActorMessage& message) = 0;

protected:

	virtual bool AddEndFunc(const int subMessageID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end) = 0;
	
    virtual bool AddComponent(const NFGUID nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool RemoveComponent(const NFGUID nActorIndex, const std::string& strComponentName) = 0;
	virtual NF_SHARE_PTR<NFIComponent> FindComponent(const NFGUID nActorIndex, const std::string& strComponentName) = 0;
};

#endif
