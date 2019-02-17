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

#include "NFComm/NFNetPlugin/NFNet.h"
#include <thread>
#include <string>
#include "NFComm/NFLogPlugin/easylogging++.h"
#if NF_PLATFORM != NF_PLATFORM_WIN
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <execinfo.h>
#endif
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"NFNet_d.lib")

INITIALIZE_EASYLOGGINGPP

class TestClientClass
{
public:
    TestClientClass()
    {
        pNet = new NFNet(this, &TestClientClass::ReciveHandler, &TestClientClass::EventHandler);
        pNet->Initialization("127.0.0.1", 8088);
		bConnected = false;
    }

    virtual ~TestClientClass()
    {

    }

    void ReciveHandler(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        std::string str;
        str.assign(msg, nLen);

        std::cout << " fd: " << nSockIndex << " msg_id: " << nMsgID /*<<  " data: " << str */ << " thread_id: " << std::this_thread::get_id() << std::endl;
    };

    void EventHandler(const NFSOCK nSockIndex, const NF_NET_EVENT e, NFINet* p)
    {
        std::cout << " fd: " << nSockIndex << " event_id: " << e << " thread_id: " << std::this_thread::get_id() << std::endl;
		if(e & BEV_EVENT_CONNECTED)
		{
			bConnected = true;
		}
        else
        {
            bConnected = false;
        }
    }

    void Execute()
    {
		if(bConnected)
		{
			pNet->SendMsgWithOutHead(1, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100, 0);
		}

        pNet->Execute();
    }

protected:
    NFINet* pNet;
	bool bConnected;
private:
};


#if NF_PLATFORM == NF_PLATFORM_WIN

#pragma comment( lib, "DbgHelp" )
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
		bExitApp = true;
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

    sprintf_s(szDmupName, "%04d_%02d_%02d_%02d_%02d_%02d.dmp",  ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    CreateDumpFile(szDmupName, pException);

    FatalAppExit(-1,  szDmupName);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

#if NF_PLATFORM != NF_PLATFORM_WIN
void CrashHandler(int sig) {
	// FOLLOWING LINE IS ABSOLUTELY NEEDED AT THE END IN ORDER TO ABORT APPLICATION
	//el::base::debug::StackTrace();
	//el::Helpers::logCrashReason(sig, true);


	LOG(FATAL) << "crash sig:" << sig;

	int size = 16;
	void * array[16];
	int stack_num = backtrace(array, size);
	char ** stacktrace = backtrace_symbols(array, stack_num);
	for (int i = 0; i < stack_num; ++i)
	{
		//printf("%s\n", stacktrace[i]);
		LOG(FATAL) << stacktrace[i];
	}

	free(stacktrace);
}
#endif

int main(int argc, char** argv)
{
#if NF_PLATFORM == NF_PLATFORM_WIN
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#else
	el::Helpers::setCrashHandler(CrashHandler);
#endif

    std::list<TestClientClass*> list;

    for (int i = 0; i < 10; ++i)
    {
        TestClientClass* x = new TestClientClass();;
        list.push_back(x);
    }

    try
    {

        while (1)
        {
            sleep(1);
            
            std::list<TestClientClass*>::iterator it = list.begin();
            for (it; it != list.end(); ++it)
            {

                (*it)->Execute();
            }
        }
    }
    catch(std::exception& e)
    {
	    LOG(FATAL) << e.what();
    }
    catch(...)
    {
	    LOG(FATAL) << "unknow exception";
    }


    return 0;
}
