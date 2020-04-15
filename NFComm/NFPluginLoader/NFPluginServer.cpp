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

#if NF_PLATFORM == NF_PLATFORM_WIN
bool ApplicationCtrlHandler(DWORD fdwctrltype)
{
    switch (fdwctrltype)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    {
        //bExitApp = true;
    }
    return true;
    default:
        return false;
    }
}

void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException)
{
    //Dump
    HANDLE hDumpFile = CreateFile(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    time_t t = time(0);
    char szDmupName[MAX_PATH];
    tm* ptm = localtime(&t);

    sprintf_s(szDmupName, "%04d_%02d_%02d_%02d_%02d_%02d.dmp", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    CreateDumpFile(szDmupName, pException);

    FatalAppExit(-1, szDmupName);

    return EXCEPTION_EXECUTE_HANDLER;
}

#else

void NFCrashHandler(int sig)
{
    // FOLLOWING LINE IS ABSOLUTELY NEEDED AT THE END IN ORDER TO ABORT APPLICATION
    //el::base::debug::StackTrace();
    //el::Helpers::logCrashReason(sig, true);


    LOG(FATAL) << "crash sig:" << sig;

    int size = 16;
    void* array[16];
    int stack_num = backtrace(array, size);
    char** stacktrace = backtrace_symbols(array, stack_num);
    for (int i = 0; i < stack_num; ++i)
    {
        //printf("%s\n", stacktrace[i]);
        LOG(FATAL) << stacktrace[i];
    }

    free(stacktrace);
}

#endif

NFPluginServer::NFPluginServer(const std::string& strArgv)
{
    this->strArgvList = strArgv;

#if NF_PLATFORM == NF_PLATFORM_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#else
    signal(SIGSEGV, NFCrashHandler);
    //el::Helpers::setCrashHandler(CrashHandler);
#endif
}

void NFPluginServer::Execute()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    __try
#else
    try
#endif
    {
        pPluginManager->Execute();
    }
#if NF_PLATFORM == NF_PLATFORM_WIN
    __except (ApplicationCrashHandler(GetExceptionInformation()))
    {
    }
#else
    catch (const std::exception & e)
    {
        NFException::StackTrace(11);
    }
    catch (...)
    {
        NFException::StackTrace(11);
    }
#endif
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

void NFPluginServer::SetMidwareLoader(std::function<void(NFIPluginManager * p)> fun)
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
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationCtrlHandler, true);
#else
    //run it as a daemon process
    if (strArgvList.find("-d") != string::npos)
    {
        InitDaemon();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
#endif

    NFDataList argList;
    argList.Split(this->strArgvList, " ");

    for (int i = 0; i < argList.GetCount(); i++)
    {
        strPluginName = argList.String(i);
        if (strPluginName.find("Plugin=") != string::npos)
        {
            strPluginName.erase(0, 7);
            break;
        }

        strPluginName = "";
    }

    pPluginManager->SetConfigName(strPluginName);

    for (int i = 0; i < argList.GetCount(); i++)
    {
        strAppName = argList.String(i);
        if (strAppName.find("Server=") != string::npos)
        {
            strAppName.erase(0, 7);
            break;
        }

        strAppName = "";
    }

    pPluginManager->SetAppName(strAppName);

    for (int i = 0; i < argList.GetCount(); i++)
    {
        strAppID = argList.String(i);
        if (strAppID.find("ID=") != string::npos)
        {
            strAppID.erase(0, 3);
            break;
        }

        strAppID = "";
    }

    int nAppID = 0;
    if (NF_StrTo(strAppID, nAppID))
    {
        pPluginManager->SetAppID(nAppID);
    }

    // NoSqlServer.xml:IP=\"127.0.0.1\"==IP=\"192.168.1.1\"
    if (strArgvList.find(".xml:") != string::npos)
    {
        for (int i = 0; i < argList.GetCount(); i++)
        {
            std::string strPipeline = argList.String(i);
            int posFile = strPipeline.find(".xml:");
            int posContent = strPipeline.find("==");
            if (posFile != string::npos && posContent != string::npos)
            {
                std::string fileName = strPipeline.substr(0, posFile + 4);
                std::string content = strPipeline.substr(posFile + 5, posContent - (posFile + 5));
                std::string replaceContent = strPipeline.substr(posContent + 2, strPipeline.length() - (posContent + 2));

                pPluginManager->AddFileReplaceContent(fileName, content, replaceContent);
            }
        }
    }

    std::string strDockerFlag = "0";
    for (int i = 0; i < argList.GetCount(); i++)
    {
        strDockerFlag = argList.String(i);
        if (strDockerFlag.find("Docker=") != string::npos)
        {
            strDockerFlag.erase(0, 7);
            break;
        }

        strDockerFlag = "";
    }

    int nDockerFlag = 0;
    if (NF_StrTo(strDockerFlag, nDockerFlag))
    {
        pPluginManager->SetRunningDocker(nDockerFlag);
    }

    strTitleName = strAppName + strAppID;// +" PID" + NFGetPID();
    if (!strTitleName.empty())
    {
        strTitleName.replace(strTitleName.find("Server"), 6, "");
        strTitleName = "NF" + strTitleName;
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

bool NFPluginServer::GetFileContent(NFIPluginManager* p, const std::string& strFilePath, std::string& strContent)
{
    FILE* fp = fopen(strFilePath.c_str(), "rb");
    if (!fp)
    {
        return false;
    }

    fseek(fp, 0, SEEK_END);
    const long filelength = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    strContent.resize(filelength);
    fread((void*)strContent.data(), filelength, 1, fp);
    fclose(fp);

    std::string strFileName = strFilePath.substr(strFilePath.find_last_of("/\\") + 1);
    std::vector<NFReplaceContent> contents = p->GetFileReplaceContents(strFileName);
    if (!contents.empty())
    {
        for (auto it : contents)
        {
            std::size_t pos = strContent.find(it.content);
            if (pos != string::npos)
            {
                strContent.replace(pos, it.content.length(), it.newValue.c_str());
            }
        }
    }

    return true;
}