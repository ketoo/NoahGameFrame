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

#include "NFPluginServer.h"
#include "NFComm/NFCore/NFException.hpp"

NFPluginServer::NFPluginServer(const std::string& strArgv)
{
    this->strArgvList = strArgv;

#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_TRY_ROOT
#endif
}

void NFPluginServer::Execute()
{
	pPluginManager->Execute();
}

void NFPluginServer::PrintfLogo()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

    std::cout << "\n" << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "**                 NoahFrame                  **" << std::endl;
    std::cout << "**   Copyright (c) 2011-2019, LvSheng.Huang   **" << std::endl;
    std::cout << "**             All rights reserved.           **" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << "-d Run itas daemon mode, only on linux" << std::endl;
    std::cout << "-x Close the 'X' button, only on windows" << std::endl;
    std::cout << "Instance: name.xml File's name to instead of \"Plugin.xml\" when programs be launched, all platform" << std::endl;
    std::cout << "Instance: \"ID=number\", \"Server=GameServer\"  when programs be launched, all platform" << std::endl;
    std::cout << "\n" << std::endl;

#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void NFPluginServer::SetMidWareLoader(std::function<void(NFIPluginManager * p)> fun)
{
    externalPluginLoader = fun;
}

void NFPluginServer::Init()
{
    PrintfLogo();

    pPluginManager = NF_SHARE_PTR<NFIPluginManager>(NF_NEW NFPluginManager());

    ProcessParameter();

    pPluginManager->SetGetFileContentFunctor(GetFileContent);
    pPluginManager->SetConfigPath("../");

    pPluginManager->LoadPluginConfig();
    if (externalPluginLoader)
    {
        externalPluginLoader(pPluginManager.get());
    }

    pPluginManager->LoadPlugin();

    pPluginManager->Awake();
    pPluginManager->Init();
    pPluginManager->AfterInit();
    pPluginManager->CheckConfig();
    pPluginManager->ReadyExecute();
}

void NFPluginServer::Final()
{
    pPluginManager->BeforeShut();
    pPluginManager->Shut();
    pPluginManager->Finalize();

    pPluginManager = nullptr;
}

void NFPluginServer::ProcessParameter()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    
#else
    //run it as a daemon process
    if (strArgvList.find("-d") != string::npos)
    {
        InitDaemon();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
#endif

    std::vector<std::string> argList;
	std::string token;
	std::istringstream tokenStream(this->strArgvList);
	while (std::getline(tokenStream, token, ' '))
	{
		argList.push_back(token);
	}

    pPluginManager->SetConfigName(FindParameterValue(argList, "Plugin="));
	pPluginManager->SetAppName(FindParameterValue(argList, "Server="));

	std::string strAppID = FindParameterValue(argList, "ID=");
    int appID = 0;
    if (NF_StrTo(strAppID, appID))
    {
        pPluginManager->SetAppID(appID);
    }

	std::string strDockerFlag = FindParameterValue(argList, "Docker=");
	int nDockerFlag = 0;
	if (NF_StrTo(strDockerFlag, nDockerFlag))
	{
		pPluginManager->SetRunningDocker(nDockerFlag);
	}

    // NoSqlServer.xml:IP=\"127.0.0.1\"==IP=\"192.168.1.1\"
    if (strArgvList.find(".xml:") != string::npos)
    {
        for (int i = 0; i < argList.size(); i++)
        {
            std::string strPipeline = argList[i];
            size_t posFile = strPipeline.find(".xml:");
            size_t posContent = strPipeline.find("==");
            if (posFile != string::npos && posContent != string::npos)
            {
                std::string fileName = strPipeline.substr(0, posFile + 4);
                std::string content = strPipeline.substr(posFile + 5, posContent - (posFile + 5));
                std::string replaceContent = strPipeline.substr(posContent + 2, strPipeline.length() - (posContent + 2));

                pPluginManager->AddFileReplaceContent(fileName, content, replaceContent);
            }
        }
    }

    std::string strTitleName = pPluginManager->GetAppName() + std::to_string(pPluginManager->GetAppID());// +" PID" + NFGetPID();
    if (!strTitleName.empty())
    {
        size_t pos = strTitleName.find("Server");
		if (pos != string::npos)
		{
			strTitleName.replace(pos, 6, "");
			strTitleName = "NF" + strTitleName;
		}
    }
    else
    {
        strTitleName = "NFIDE";
    }

#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTitle(strTitleName.c_str());
#elif NF_PLATFORM == NF_PLATFORM_LINUX
    prctl(PR_SET_NAME, strTitleName.c_str());
    //setproctitle(strTitleName.c_str());
#endif
}

void NFPluginServer::InitDaemon()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
    daemon(1, 0);

    // ignore signals
    signal(SIGINT, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
#endif
}

bool NFPluginServer::GetFileContent(NFIPluginManager* p, const std::string& strFilePath, std::string& content)
{
    FILE* fp = fopen(strFilePath.c_str(), "rb");
    if (!fp)
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    const long filelength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    content.resize(filelength);
    fread((void*)content.data(), filelength, 1, fp);
    fclose(fp);

    std::string fileName = strFilePath.substr(strFilePath.find_last_of("/\\") + 1);
    std::vector<NFReplaceContent> contents = p->GetFileReplaceContents(fileName);
    if (!contents.empty())
    {
        for (auto it : contents)
        {
            std::size_t pos = content.find(it.content);
            if (pos != string::npos)
            {
                content.replace(pos, it.content.length(), it.newValue.c_str());
            }
        }
    }

    return true;
}

std::string NFPluginServer::FindParameterValue(const std::vector<std::string>& argList, const std::string& header)
{
	for (int i = 0; i < argList.size(); i++)
	{
		std::string name = argList[i];
		if (name.find(header) != string::npos)
		{
			name.erase(0, header.length());
			return name;
		}

	}

	return "";
}
