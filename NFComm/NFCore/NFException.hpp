/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement; besides, anyone who use this file/software
   must include this copyright announcement.

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

#ifndef NF_EXCEPTION_H
#define NF_EXCEPTION_H

#include <time.h>
#include <setjmp.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <exception>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <atomic>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <NFComm/NFPluginModule/NFPlatform.h>

#if NF_PLATFORM != NF_PLATFORM_WIN
#include <execinfo.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <execinfo.h>
#include <setjmp.h>

#if NF_PLATFORM == NF_PLATFORM_LINUX
#include <sys/prctl.h>
#endif



class NFExceptFrame
{
public:
	jmp_buf Jump_Buffer;
};


class NFException
{
private:
	NFException(){}

	static std::string FileName()
	{
		time_t now = time(0);
		tm *ltm = localtime(&now);

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(4) << 1900 + ltm->tm_year << "-" << std::setw(2) << 1 + ltm->tm_mon << "-" << std::setw(2) << ltm->tm_mday;
		ss << " ";
		ss << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":" << std::setw(2) << ltm->tm_min << ":" << std::setw(2) << ltm->tm_sec;
		ss << ".crash";
		return ss.str();
	}
public:

	static void StackTrace(int sig)
	{
		std::ofstream outfile;
		outfile.open(FileName(), std::ios::app);

		outfile << std::endl;
		outfile << "******************************************************************************" << std::endl;
		outfile << "crash sig:" << sig << std::endl;

		int size = 16;
		void * array[16];
		int stack_num = backtrace(array, size);
		char ** stacktrace = backtrace_symbols(array, stack_num);
		for (int i = 0; i < stack_num; ++i)
		{
			outfile << stacktrace[i] << std::endl;
		}

		free(stacktrace);

		outfile.close();
	}

	static void CrashHandler(int sig)
	{
		//std::cout << "CrashHandler" << std::endl;

		NFException::StackTrace(sig);
		if (sig > 0)
		{
			if (signal(SIGILL, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGILL, NFException::CrashHandler);
			if (signal(SIGABRT, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGABRT, NFException::CrashHandler);
			if (signal(SIGFPE, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGFPE, NFException::CrashHandler);
			if (signal(SIGSEGV, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGSEGV, NFException::CrashHandler);

			siglongjmp(NFException::ExceptStack().Jump_Buffer, 1);
		}
	}

	static NFExceptFrame& ExceptStack();
};
#define NF_CRASH_TRY_ROOT \
if (signal(SIGILL, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGILL, NFException::CrashHandler); \
if (signal(SIGABRT, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGABRT, NFException::CrashHandler); \
if (signal(SIGFPE, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGFPE, NFException::CrashHandler); \
if (signal(SIGSEGV, NFException::CrashHandler) != NFException::CrashHandler) signal(SIGSEGV, NFException::CrashHandler); \

#define NF_CRASH_TRY \
{\
if(sigsetjmp(NFException::ExceptStack().Jump_Buffer, 1) == 0)\
{\
try\
{

#define NF_CRASH_END \
}\
catch (...)\
{\
    NFException::CrashHandler(0);\
}\
}\
}




#endif

/*
void fun()
{
	std::cout << "fun" << std::endl;

	std::string * p = nullptr;
	std::cout << p->length();
}

int main()
{
	NF_CRASH_TRY_ROOT


	NF_CRASH_TRY
		fun();
	NF_CRASH_END

	std::cout << "fun1" << std::endl;
	NF_CRASH_TRY
		fun();
	NF_CRASH_END
	std::cout << "fun2" << std::endl;
	NF_CRASH_TRY
				fun();
	NF_CRASH_END
	std::cout << "fun3" << std::endl;
	NF_CRASH_TRY
				fun();
	NF_CRASH_END
	std::cout << "fun4" << std::endl;
	NF_CRASH_TRY
				fun();
	NF_CRASH_END
	std::cout << "fun5" << std::endl;
	NF_CRASH_TRY
				fun();
	NF_CRASH_END
	std::cout << "fun6" << std::endl;

	return 0;
}
 */
#endif