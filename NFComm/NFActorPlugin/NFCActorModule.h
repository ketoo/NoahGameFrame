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


#ifndef NFC_ACTOR_MANAGER_H
#define NFC_ACTOR_MANAGER_H

#include <map>
#include <string>
#include "NFCActor.h"
#include "Theron/Theron.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFCore/NFQueue.hpp"

class NFCActorModule
    : public NFIActorModule
{
public:
	NFCActorModule(NFIPluginManager* p);
    virtual ~NFCActorModule();

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute();

	virtual int RequireActor();

    virtual bool SendMsgToActor(const int nActorIndex, const int nEventID, const std::string& strArg);

	virtual bool HandlerEx(const NFIActorMessage& message, const int from);

	virtual bool ReleaseActor(const int nActorIndex);

	virtual NF_SHARE_PTR<NFIActor> GetActor(const int nActorIndex);

protected:

	virtual bool AddDefaultEndFunc(const int nActorIndex, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end);
	virtual bool AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr);

    virtual bool AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent);

	virtual bool ExecuteEvent();
private:
	NF_SHARE_PTR<NFIActor> m_pMainActor;

    Theron::Framework mFramework;
	NFMapEx<int, NFIActor> mxActorMap;
	std::map<int, int> mxActorPool;
	//NFQueue<NF_SHARE_PTR<NFIActor>> mxActorPool;

	NFQueue<NFIActorMessage> mxQueue;
};

#endif
