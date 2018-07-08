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

#ifndef NFC_HELLO_WORLD4_H
#define NFC_HELLO_WORLD4_H

#include <thread>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"
#include "NFComm/NFPluginModule/NFIComponent.h"

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
		AddMsgObserver(2, this, &NFHttpComponent::OnMsgEvent);

		return true;
	}


	virtual int OnMsgEvent(const NFGUID& self, const int from, const int event, std::string& arg)
	{

		std::cout << "Thread: " << std::this_thread::get_id() << " " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;



		return 0;
	}

	virtual int OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg)
	{
		std::cout << "------Actor thread: " << std::this_thread::get_id() << " " << self.ToString() << " MsgID: " << event << " Data:" << arg << std::endl;

		return 0;
	}
};

class NFIHelloWorld4Module
	: public NFIModule
{
};

class NFCHelloWorld4Module
    : public NFIHelloWorld4Module
{
public:
    NFCHelloWorld4Module(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
	int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nSubMsgID, const std::string& strData);
	
protected:
    NFIActorModule* m_pActorModule;
};

#endif
