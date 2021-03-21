/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#ifndef NF_DYNAMIC_PLUGIN
//for nf-sdk plugins
#include "NFComm/NFActorPlugin/NFActorPlugin.h"
#include "NFComm/NFConfigPlugin/NFConfigPlugin.h"
#include "NFComm/NFKernelPlugin/NFKernelPlugin.h"
#include "NFComm/NFLogPlugin/NFLogPlugin.h"
#include "NFComm/NFLuaScriptPlugin/NFLuaScriptPlugin.h"
#include "NFComm/NFNavigationPlugin/NFNavigationPlugin.h"
#include "NFComm/NFNetPlugin/NFNetPlugin.h"
#include "NFComm/NFNoSqlPlugin/NFNoSqlPlugin.h"
#include "NFComm/NFSecurityPlugin/NFSecurityPlugin.h"
#include "NFComm/NFTestPlugin/NFTestPlugin.h"

#include "NFExamples/NFChatPlugin/NFChatPlugin.h"
#include "NFExamples/NFConsumeManagerPlugin/NFConsumeManagerPlugin.h"
#include "NFExamples/NFInventoryPlugin/NFInventoryPlugin.h"

#if NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_WIN
#include "NFComm/NFRenderPlugin/NFRenderPlugin.h"
#include "NFComm/NFBluePrintPlugin/NFBluePrintPlugin.h"
#endif

//DB
#include "NFServer/NFDBLogicPlugin/NFDBLogicPlugin.h"
#include "NFServer/NFDBNet_ClientPlugin/NFDBNet_ClientPlugin.h"
#include "NFServer/NFDBNet_ServerPlugin/NFDBNet_ServerPlugin.h"
//GAME
#include "NFServer/NFGameServerNet_ClientPlugin/NFGameServerNet_ClientPlugin.h"
#include "NFServer/NFGameServerNet_ServerPlugin/NFGameServerNet_ServerPlugin.h"
#include "NFServer/NFGameServerPlugin/NFGameServerPlugin.h"
//LOGIN
#include "NFServer/NFLoginLogicPlugin/NFLoginLogicPlugin.h"
#include "NFServer/NFLoginNet_ClientPlugin/NFLoginNet_ClientPlugin.h"
#include "NFServer/NFLoginNet_ServerPlugin/NFLoginNet_ServerPlugin.h"
#include "NFServer/NFLoginNet_HttpServerPlugin/NFLoginNet_HttpServerPlugin.h"
//MASTER
#include "NFServer/NFMasterNet_HttpServerPlugin/NFMasterNet_HttpServerPlugin.h"
#include "NFServer/NFMasterNet_ServerPlugin/NFMasterNet_ServerPlugin.h"
//PROXY
#include "NFServer/NFProxyLogicPlugin/NFProxyLogicPlugin.h"
#include "NFServer/NFProxyServerNet_ClientPlugin/NFProxyServerNet_ClientPlugin.h"
#include "NFServer/NFProxyServerNet_ServerPlugin/NFProxyServerNet_ServerPlugin.h"
//WORLD
#include "NFServer/NFWorldNet_ClientPlugin/NFWorldNet_ClientPlugin.h"
#include "NFServer/NFWorldNet_ServerPlugin/NFWorldNet_ServerPlugin.h"


#endif

////////////EXTERNAL PLUGIN LISTED BELOW////////////////////////////////////////////

#include "Tutorial/Tutorial1/Tutorial1.h"
#include "Tutorial/Tutorial2/Tutorial2.h"
#include "Tutorial/Tutorial3/Tutorial3Plugin.h"
#include "Tutorial/Tutorial4/Tutorial4Plugin.h"
#include "Tutorial/Tutorial5/Tutorial5.h"
#include "Tutorial/Tutorial6/Tutorial6.h"
#include "Tutorial/Tutorial7/Tutorial7.h"

void BasicPluginLoader(NFIPluginManager* pPluginManager)
{

#ifndef NF_DYNAMIC_PLUGIN

//for nf-sdk plugins
	CREATE_PLUGIN(pPluginManager, NFActorPlugin)
	CREATE_PLUGIN(pPluginManager, NFConfigPlugin)
	CREATE_PLUGIN(pPluginManager, NFKernelPlugin)
	CREATE_PLUGIN(pPluginManager, NFLogPlugin)
	CREATE_PLUGIN(pPluginManager, NFLuaScriptPlugin)
	CREATE_PLUGIN(pPluginManager, NFNavigationPlugin)
	CREATE_PLUGIN(pPluginManager, NFNetPlugin)
	CREATE_PLUGIN(pPluginManager, NFNoSqlPlugin)
	CREATE_PLUGIN(pPluginManager, NFSecurityPlugin)
	CREATE_PLUGIN(pPluginManager, NFTestPlugin)

#if NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DEBUG_MODE
	//CREATE_PLUGIN(pPluginManager, NFRenderPlugin)
	//CREATE_PLUGIN(pPluginManager, NFBluePrintPlugin)
#endif
#endif

//DB
	CREATE_PLUGIN(pPluginManager, NFDBLogicPlugin)
	CREATE_PLUGIN(pPluginManager, NFDBNet_ClientPlugin)
	CREATE_PLUGIN(pPluginManager, NFDBNet_ServerPlugin)

//GAME
	CREATE_PLUGIN(pPluginManager, NFGameServerNet_ClientPlugin)
	CREATE_PLUGIN(pPluginManager, NFGameServerNet_ServerPlugin)
	CREATE_PLUGIN(pPluginManager, NFGameServerPlugin)

//LOGIN
	CREATE_PLUGIN(pPluginManager, NFLoginLogicPlugin)
	CREATE_PLUGIN(pPluginManager, NFLoginNet_ClientPlugin)
	CREATE_PLUGIN(pPluginManager, NFLoginNet_ServerPlugin)
	CREATE_PLUGIN(pPluginManager, NFLoginNet_HttpServerPlugin)

//MASTER
	CREATE_PLUGIN(pPluginManager, NFMasterNet_HttpServerPlugin)
	CREATE_PLUGIN(pPluginManager, NFMasterNet_ServerPlugin)

//PROXY
	CREATE_PLUGIN(pPluginManager, NFProxyLogicPlugin)
	CREATE_PLUGIN(pPluginManager, NFProxyServerNet_ClientPlugin)
	CREATE_PLUGIN(pPluginManager, NFProxyServerNet_ServerPlugin)

//WORLD
	CREATE_PLUGIN(pPluginManager, NFWorldNet_ClientPlugin)
	CREATE_PLUGIN(pPluginManager, NFWorldNet_ServerPlugin)
#endif
}

void MidWareLoader(NFIPluginManager* pPluginManager)
{

	CREATE_PLUGIN(pPluginManager, NFChatPlugin)
	CREATE_PLUGIN(pPluginManager, NFConsumeManagerPlugin)
	CREATE_PLUGIN(pPluginManager, NFInventoryPlugin)


	CREATE_PLUGIN(pPluginManager, Tutorial1)
	CREATE_PLUGIN(pPluginManager, Tutorial2)
	CREATE_PLUGIN(pPluginManager, Tutorial3Plugin)
	CREATE_PLUGIN(pPluginManager, Tutorial4Plugin)
	CREATE_PLUGIN(pPluginManager, Tutorial5)
	CREATE_PLUGIN(pPluginManager, Tutorial6)
	CREATE_PLUGIN(pPluginManager, Tutorial7)
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
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=GameServer ID=16001 Plugin=Plugin.xml")));
	}
	else
	{
		serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList)));
	}

	for (auto item : serverList)
	{
		item->SetBasicWareLoader(BasicPluginLoader);
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