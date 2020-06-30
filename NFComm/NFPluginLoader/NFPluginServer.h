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
#ifndef NF_SERVER_H
#define NF_SERVER_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>
#include "NFPluginManager.h"
#include "NFComm/NFCore/NFException.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM != NF_PLATFORM_WIN
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <execinfo.h>
#include <setjmp.h>

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <sys/prctl.h>
#endif

#endif




class NFPluginServer
{
public:
	NFPluginServer(const std::string& strArgv);

	virtual ~NFPluginServer()
	{
		Final();
	}

	NF_SHARE_PTR<NFIPluginManager> pPluginManager;
	std::string strArgvList;
	std::function<void(NFIPluginManager * p)> externalPluginLoader;

	void Init();

	void Execute();

	void Final();
	
	void PrintfLogo();

	void SetMidWareLoader(std::function<void(NFIPluginManager * p)> fun);

private:

	void ProcessParameter();

	std::string FindParameterValue(const NFDataList& argList, const std::string& header);

	void InitDaemon();

	static bool GetFileContent(NFIPluginManager* p, const std::string& strFilePath, std::string& strContent);
};

#endif