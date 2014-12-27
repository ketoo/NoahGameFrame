// -------------------------------------------------------------------------
//    @FileName         :    NFPluginLoader.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :
//    @Module           :    NFPluginLoader
//
// -------------------------------------------------------------------------

#include <crtdbg.h>
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
#include "NFComm/NFCore/NFPlatform.h"
#include "boost/thread.hpp"

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

    sprintf(szDmupName, "%d_%d_%d_%d_%d_%d.dmp",  ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    CreateDumpFile(szDmupName, pException);

    FatalAppExit(-1,  szDmupName);

    return EXCEPTION_EXECUTE_HANDLER;
}

void CloseXButton()
{
#if NF_PLATFORM == NF_PLATFORM_WIN

    HWND hWnd = GetConsoleWindow();  
    if(hWnd)  
    {  
        HMENU hMenu = GetSystemMenu(hWnd, FALSE);  
        EnableMenuItem(hMenu, SC_CLOSE, MF_DISABLED | MF_BYCOMMAND);  
    }

#endif
}

bool bExitApp = false;
boost::thread gThread;

void ThreadFunc()
{
    while ( !bExitApp )
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string s;
        std::cin >> s;
        if ( 0 == stricmp( s.c_str(), "exit" ) )
        {
            bExitApp = true;
        }
    }
}

void CreateBackThread()
{
    gThread = boost::thread(boost::bind(&ThreadFunc));
    //std::cout << "CreateBackThread, thread ID = " << gThread.get_id() << std::endl;
}

void PrintfLogo()
{
    std::cout << "\n" << std::endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "★★★★★★★★★★★★★★★★★★★★★★★★" << std::endl;
    std::cout << "★                                            ★" << std::endl;
    std::cout << "★                 NoahFrame                  ★" << std::endl;
    std::cout << "★     Copyright (c) 2011  kytoo              ★" << std::endl;
    std::cout << "★             All rights reserved.           ★" << std::endl;
    std::cout << "★                                            ★" << std::endl;
    std::cout << "★               www.yowoyo.com               ★" << std::endl;
    std::cout << "★                                            ★" << std::endl;
    std::cout << "★★★★★★★★★★★★★★★★★★★★★★★★\n" << std::endl;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    std::cout << "\n" << std::endl;
}

<<<<<<< HEAD
unsigned long unStartTickTime = 0;
unsigned long unLastTickTime = 0;
=======
unsigned long nStartTickTime = 0;
unsigned long nLastTickTime = 0;
>>>>>>> fc41e1f3b83190429904de8a9203e116d5ec564e

#if NF_PLATFORM == NF_PLATFORM_WIN || NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE
int main()
#elif  NF_PLATFORM == NF_PLATFORM_ANDROID || NF_PLATFORM == NF_PLATFORM_APPLE_IOS

#endif
{
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	NFCActorManager::GetSingletonPtr()->Init();
	NFCActorManager::GetSingletonPtr()->AfterInit();
    NFCActorManager::GetSingletonPtr()->CheckConfig();

    PrintfLogo();

    CloseXButton();
    CreateBackThread();

    nStartTickTime = ::NF_GetTickCount();
    nLastTickTime = nStartTickTime;

    while (!bExitApp)    //DEBUG版本崩溃，RELEASE不崩
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        while (true)
        {
            if (bExitApp)
            {
                break;
            }
			
			unsigned long unNowTickTime = ::NF_GetTickCount();
			float fStartedTime = float(unNowTickTime - unStartTickTime) / 1000;
			float fLastTime = float(unNowTickTime - unLastTickTime) / 1000;

			if (fStartedTime < 0.001f)
			{
				fStartedTime = 0.0f;
			}

			if (fLastTime < 0.001f)
			{
				fLastTime = 0.0f;
			}
            __try
            {
<<<<<<< HEAD
                NFCActorManager::GetSingletonPtr()->Execute(fLastTime, fStartedTime);
				unLastTickTime = unNowTickTime;
=======
                unsigned long nNowTickTime = ::NF_GetTickCount();
                float fStartedTime = float(nNowTickTime - nStartTickTime) / 1000;
                float fLastTime = float(nNowTickTime - nLastTickTime) / 1000;
                if (fStartedTime < 0.001f)
                {
                    fStartedTime = 0.0f;
                }

                if (fLastTime < 0.001f)
                {
                    fLastTime = 0.0f;
                }

                NFCActorManager::GetSingletonPtr()->Execute(fLastTime, fStartedTime);
>>>>>>> fc41e1f3b83190429904de8a9203e116d5ec564e
            }
            __except (ApplicationCrashHandler(GetExceptionInformation()))
            {
            }
        }
    }

	NFCActorManager::GetSingletonPtr()->BeforeShut();
	NFCActorManager::GetSingletonPtr()->Shut();

    NFCActorManager::GetSingletonPtr()->ReleaseInstance();

    return 0;
}