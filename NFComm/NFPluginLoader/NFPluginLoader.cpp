// -------------------------------------------------------------------------
//    @FileName         :    NFPluginLoader.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------

//#include <crtdbg.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include "NFCActorManager.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN

#pragma comment( lib, "DbgHelp" )
// 创建Dump文件
void CreateDumpFile(const std::string& strDumpFilePathName, EXCEPTION_POINTERS* pException)
{
    // 创建Dump文件
    HANDLE hDumpFile = CreateFile(strDumpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Dump信息
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;

    // 写入Dump文件内容
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);
}

// 处理Unhandled Exception的回调函数
long ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
    time_t t = time(0);
    char szDmupName[MAX_PATH];
    tm* ptm = localtime(&t);

    sprintf_s(szDmupName, "%04d_%02d_%02d_%02d_%02d_%02d.dmp",  ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    CreateDumpFile(szDmupName, pException);

    FatalAppExit(-1,  szDmupName);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void CloseXButton()
{
#if NF_PLATFORM == NF_PLATFORM_WIN

    HWND hWnd = GetConsoleWindow();
    if (hWnd)
    {
        HMENU hMenu = GetSystemMenu(hWnd, FALSE);
        EnableMenuItem(hMenu, SC_CLOSE, MF_DISABLED | MF_BYCOMMAND);
    }

#endif
}

bool bExitApp = false;
std::thread gThread;

void ThreadFunc()
{
    while (!bExitApp)
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        //         std::string s;
        //         std::cin >> s;
        //         if ( 0 == stricmp( s.c_str(), "exit" ) )
        //         {
        //             bExitApp = true;
        //         }
    }
}

void CreateBackThread()
{
    //gThread = std::thread(std::bind(&ThreadFunc));
    //auto f = std::async (std::launch::async, std::bind(ThreadFunc));
    //std::cout << "CreateBackThread, thread ID = " << gThread.get_id() << std::endl;
}

void InitDaemon()
{
#if NF_PLATFORM == NF_PLATFORM_LINUX
    daemon(1, 0);

    // ignore signals
    signal(SIGINT,  SIG_IGN);
    signal(SIGHUP,  SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
#endif
}

void PrintfLogo()
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif

    std::cout << "\n" << std::endl;
    std::cout << "★★★★★★★★★★★★★★★★★★★★★★★★" << std::endl;
    std::cout << "★                                            ★" << std::endl;
    std::cout << "★                 NoahFrame                  ★" << std::endl;
    std::cout << "★   Copyright (c) 2011-2016  NFrame Studio   ★" << std::endl;
    std::cout << "★             All rights reserved.           ★" << std::endl;
    std::cout << "★                                            ★" << std::endl;
    std::cout << "★★★★★★★★★★★★★★★★★★★★★★★★" << std::endl;
    std::cout << "\n" << std::endl;

#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif // NF_PLATFORM
}

#if NF_PLATFORM == NF_PLATFORM_WIN || NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE
int main(int argc, char* argv[])
#elif  NF_PLATFORM == NF_PLATFORM_ANDROID || NF_PLATFORM == NF_PLATFORM_APPLE_IOS

#endif
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#elif NF_PLATFORM == NF_PLATFORM_LINUX
    bool bDaemon = false;

    if (argc > 1)
    {
        if (0 == NFSTRICMP((const char*)argv[1], "-d"))
        {
            bDaemon = true;
        }
    }

    if (bDaemon)
    {
        InitDaemon();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

#endif
    NFCActorManager::GetSingletonPtr()->Init();
    NFCActorManager::GetSingletonPtr()->AfterInit();
    NFCActorManager::GetSingletonPtr()->CheckConfig();

    PrintfLogo();

    CloseXButton();
    CreateBackThread();

    while (!bExitApp)    //DEBUG版本崩溃，RELEASE不崩
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            if (bExitApp)
            {
                break;
            }

#if NF_PLATFORM == NF_PLATFORM_WIN
            __try
            {
#endif
                NFCActorManager::GetSingletonPtr()->Execute();
#if NF_PLATFORM == NF_PLATFORM_WIN
            }
            __except (ApplicationCrashHandler(GetExceptionInformation()))
            {
            }
#endif
        }
    }

    NFCActorManager::GetSingletonPtr()->BeforeShut();
    NFCActorManager::GetSingletonPtr()->Shut();

    NFCActorManager::GetSingletonPtr()->ReleaseInstance();

    return 0;
}
