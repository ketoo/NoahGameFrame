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
#include <future>
#include <functional>
#include <atomic>
#include "NFCPluginManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/prctl.h>
#endif

bool bExitApp = false;
std::thread gThread;
std::string strArgvList;
std::string strPluginName;
std::string strAppName;
std::string strAppID;
std::string strTitleName;

#if NF_PLATFORM == NF_PLATFORM_WIN

#pragma comment( lib, "DbgHelp" )

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

void ThreadFunc()
{
    while (!bExitApp)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string s;
        std::cin >> s;
        if ( 0 == stricmp( s.c_str(), "exit" ) )
        {
            bExitApp = true;
            gThread.detach();
        }
    }
}

void CreateBackThread()
{
    gThread = std::thread(std::bind(&ThreadFunc));
    auto f = std::async (std::launch::deferred, std::bind(ThreadFunc));
    std::cout << "CreateBackThread, thread ID = " << gThread.get_id() << std::endl;
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
    std::cout << "************************************************" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "**                 NoahFrame                  **" << std::endl;
    std::cout << "**   Copyright (c) 2011, LvSheng.Huang        **" << std::endl;
    std::cout << "**             All rights reserved.           **" << std::endl;
    std::cout << "**                                            **" << std::endl;
    std::cout << "************************************************" << std::endl;
    std::cout << "\n" << std::endl;
	std::cout << "-d Run itas daemon mode, only on linux" << std::endl;
	std::cout << "-x Closethe 'X' button, only on windows" << std::endl;
	std::cout << "Instance: name.xml File's name to instead of \"Plugin.xml\" when programs be launched, all platform" << std::endl;
	std::cout << "Instance: \"ID=number\", \"Server=GameServer\"  when programs be launched, all platform" << std::endl;
	std::cout << "\n" << std::endl;

#if NF_PLATFORM == NF_PLATFORM_WIN
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
}

void ProcessParameter(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++)
	{
		strArgvList += " ";
		strArgvList += argv[i];
	}

#if NF_PLATFORM == NF_PLATFORM_WIN
	if (strArgvList.find("-x") != string::npos)
	{
		CloseXButton();
	}
#elif NF_PLATFORM == NF_PLATFORM_LINUX
    //run it as a daemon process
	if (strArgvList.find("-d") != string::npos)
	{
		InitDaemon();
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
#endif

	if (strArgvList.find(".xml") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strPluginName = argv[i];
			if (strPluginName.find(".xml") != string::npos)
			{
				break;
			}
		}

		NFCPluginManager::GetSingletonPtr()->SetConfigName(strPluginName);
	}

    if (strArgvList.find("Server=") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strAppName = argv[i];
			if (strAppName.find("Server=") != string::npos)
			{
                strAppName.erase(0, 7);
				break;
			}
		}

		NFCPluginManager::GetSingletonPtr()->SetAppName(strAppName);
	}

	if (strArgvList.find("ID=") != string::npos)
	{
		for (int i = 0; i < argc; i++)
		{
			strAppID = argv[i];
			if (strAppID.find("ID=") != string::npos)
			{
                strAppID.erase(0, 3);
				break;
			}
		}

		int nAppID = 0;
        if(NF_StrTo(strAppID, nAppID))
        {
            NFCPluginManager::GetSingletonPtr()->SetAppID(nAppID);
        }
	}

	strTitleName = strAppName + strAppID;// +" PID" + NFGetPID();
	strTitleName.replace(strTitleName.find("Server"), 6, "");
	strTitleName = "NF" + strTitleName;
#if NF_PLATFORM == NF_PLATFORM_WIN
	SetConsoleTitle(strTitleName.c_str());
#elif NF_PLATFORM == NF_PLATFORM_LINUX
	prctl(PR_SET_NAME, strTitleName.c_str());
	//setproctitle(strTitleName.c_str());
#endif
}

int main(int argc, char* argv[])
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#elif NF_PLATFORM == NF_PLATFORM_LINUX
#endif

    ProcessParameter(argc, argv);

	PrintfLogo();
	CreateBackThread();

	NFCPluginManager::GetSingletonPtr()->Awake();
	NFCPluginManager::GetSingletonPtr()->Init();
	NFCPluginManager::GetSingletonPtr()->AfterInit();
	NFCPluginManager::GetSingletonPtr()->CheckConfig();
	NFCPluginManager::GetSingletonPtr()->ReadyExecute();

	

    while (!bExitApp)
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
				NFCPluginManager::GetSingletonPtr()->Execute();
#if NF_PLATFORM == NF_PLATFORM_WIN
            }
            __except (ApplicationCrashHandler(GetExceptionInformation()))
            {
            }
#endif
        }
    }

	NFCPluginManager::GetSingletonPtr()->BeforeShut();
	NFCPluginManager::GetSingletonPtr()->Shut();
	NFCPluginManager::GetSingletonPtr()->Finalize();

	NFCPluginManager::GetSingletonPtr()->ReleaseInstance();

    return 0;
}
