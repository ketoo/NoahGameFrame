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

#include "../NFPluginLoader/NFPluginServer.h"


////////////EXTERNAL PLUGIN LISTED BELOW////////////////////////////////////////////

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "NFPluginLoader.lib" )

#include "Tutorial/Tutorial1/Tutorial1.h"
#include "Tutorial/Tutorial2/Tutorial2.h"
#include "Tutorial/Tutorial3/Tutorial3Plugin.h"
#include "Tutorial/Tutorial4/Tutorial4Plugin.h"
#include "Tutorial/Tutorial5/Tutorial5.h"
#include "Tutorial/Tutorial6/Tutorial6.h"
#include "Tutorial/Tutorial7/Tutorial7.h"

#pragma comment( lib, "Tutorial1.lib" )
#pragma comment( lib, "Tutorial2.lib" )
#pragma comment( lib, "Tutorial3.lib" )
#pragma comment( lib, "Tutorial4.lib" )
#pragma comment( lib, "Tutorial5.lib" )
#pragma comment( lib, "Tutorial6.lib" )
#pragma comment( lib, "Tutorial7.lib" )
#endif

void MidWareLoader(NFIPluginManager* pPluginManager)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
	//TUTORIAL
	CREATE_PLUGIN(pPluginManager, Tutorial1)
	CREATE_PLUGIN(pPluginManager, Tutorial2)
	CREATE_PLUGIN(pPluginManager, Tutorial3Plugin)
	CREATE_PLUGIN(pPluginManager, Tutorial4Plugin)
	CREATE_PLUGIN(pPluginManager, Tutorial5)
	CREATE_PLUGIN(pPluginManager, Tutorial6)
	CREATE_PLUGIN(pPluginManager, Tutorial7)
#endif
}
////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	std::cout << "__cplusplus:" << __cplusplus << std::endl;

	std::vector<NF_SHARE_PTR<NFPluginServer>> serverList;

	std::string strArgvList;
	for (int i = 0; i < argc; i++)
	{
		strArgvList += " ";
		strArgvList += argv[i];
	}

	if (argc == 1)
	{
		//IDE
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=MasterServer ID=3 Plugin=Plugin.xml")));
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=WorldServer ID=7 Plugin=Plugin.xml")));
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=LoginServer ID=4 Plugin=Plugin.xml")));
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=DBServer ID=8 Plugin=Plugin.xml")));
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=ProxyServer ID=5 Plugin=Plugin.xml")));
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=GameServer ID=6 Plugin=Plugin.xml")));
	}
	else
	{
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList)));
	}

	for (auto item : serverList)
	{
		item->SetMidWareLoader(MidWareLoader);
		item->Init();
	}

	////////////////
	uint64_t nIndex = 0;
	while (true)
	{
		nIndex++;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		for (auto item : serverList)
		{
			item->Execute();
		}
	}

	////////////////

	for (auto item : serverList)
	{
		item->Final();
	}

	serverList.clear();

    return 0;
}