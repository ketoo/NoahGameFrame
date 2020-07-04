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

#ifndef NF_HELLO_WORLD4_H
#define NF_HELLO_WORLD4_H

#include <thread>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFIComponent.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"

/*
IN THIS PLUGIN:
YOU WILL KNOW HOW TO USE THE "NFIActorModule" TO PROCESS IN DIFFERENT CPU
*/


class NFHttpComponent : public NFIComponent
{
public:
	NFHttpComponent() : NFIComponent(GET_CLASS_NAME(NFHttpComponent))
	{
	}

	virtual ~NFHttpComponent()
	{

	}

	virtual bool Init()
	{
		AddMsgHandler(0, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(1, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(2, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(3, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(4, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(5, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(6, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(7, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(8, this, &NFHttpComponent::OnMsgEvent);
		AddMsgHandler(9, this, &NFHttpComponent::OnMsgEvent);

		return true;
	}

	//it's very important to note here:
	//sometimes, the function below not finished but other thread call it at the same time
	//the reason is the main thread post a new message to this actor and the schedulel assigned another thread to take the execution right
	//so, you wouldnot use the data which not thread-safe in this function
	virtual int OnMsgEvent(NFActorMessage& arg)
	{

		std::cout << "Thread: " << std::this_thread::get_id() << " MsgID: " << arg.msgID << " Data:" << arg.data << std::endl;


		return 0;
	}
};

class NFIHelloWorld4Module
	: public NFIModule
{
};

class NFHelloWorld4Module
    : public NFIHelloWorld4Module
{
public:
    NFHelloWorld4Module(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	void RequestAsyEnd(NFActorMessage& actorMessage);
	
protected:
    NFIActorModule* m_pActorModule;
	NFIThreadPoolModule* m_pThreadPoolModule;
};

#endif
