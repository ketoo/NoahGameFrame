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

#include "NFTester.h"

int main(int argc, char* argv[])
{
	std::cout << "__cplusplus:" << __cplusplus << std::endl;

	std::string strArgvList;
	for (int i = 0; i < argc; i++)
	{
		strArgvList += " ";
		strArgvList += argv[i];
	}

	std::vector<NF_SHARE_PTR<NFPluginServer>> serverList;

	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=MasterServer ID=3 CPU=1 Plugin=Plugin.xml")));
	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=WorldServer ID=7 CPU=1 Plugin=Plugin.xml")));
	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=LoginServer ID=4 CPU=1 Plugin=Plugin.xml")));
	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=DBServer ID=8 CPU=1 Plugin=Plugin.xml")));
	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=ProxyServer ID=5 CPU=1 Plugin=Plugin.xml")));
	serverList.push_back(NF_SHARE_PTR<NFPluginServer>(NF_NEW NFPluginServer(strArgvList + " Server=GameServer ID=16001 CPU=1 Plugin=Plugin.xml", true)));

	for (auto item : serverList)
	{
		item->SetBasicWareLoader(BasicPluginLoader);
		item->SetMidWareLoader(MidWareLoader);
		item->Init();
		item->Execute();
		item->Final();
	}

    return 0;
}