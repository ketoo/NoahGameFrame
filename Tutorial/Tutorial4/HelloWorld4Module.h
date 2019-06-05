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
		AddMsgObserver(0, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(1, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(2, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(3, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(4, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(5, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(6, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(7, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(8, this, &NFHttpComponent::OnMsgEvent);
		AddMsgObserver(9, this, &NFHttpComponent::OnMsgEvent);

		return true;
	}


	virtual int OnMsgEvent(const int from, const int event, std::string& arg)
	{

		std::cout << "Thread: " << std::this_thread::get_id() << " MsgID: " << event << " Data:" << arg << std::endl;


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
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	int RequestAsyEnd(const int nFormActor, const int nSubMsgID, const std::string& strData);
	
protected:
    NFIActorModule* m_pActorModule;
	NFIThreadPoolModule* m_pThreadPoolModule;
};

#endif
