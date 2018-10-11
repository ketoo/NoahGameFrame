/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <exception>

class NFException
{
private:
    NFException(){}

    std::string _msg;
public:
	NFException(const char *format, ...)
	{
		char buf[1024] = {0};
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		_msg = buf;
		va_end(args);
	}

	NFException(const std::string& msg)
	{
		time_t tt = time(NULL);
		tm* t = localtime(&tt);
		char temp[128] = { 0 };
		sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d\n",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);

		_msg.append(temp);
		_msg.append(" NFException ");
		_msg.append(msg);
	}

	NFException(const std::string& msg, const char* file, const int line)
	{
		time_t tt = time(NULL);
		tm* t = localtime(&tt);
		char temp[128] = { 0 };
		sprintf(temp, "%d-%02d-%02d %02d:%02d:%02d\n",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);

		_msg.append(temp);
		_msg.append(" NFException ");
		_msg.append(msg);
		_msg.append(file);
		_msg.append(std::to_string(line));
	}
 
    char const * what() const noexcept{ return _msg.c_str(); }
};

#endif
