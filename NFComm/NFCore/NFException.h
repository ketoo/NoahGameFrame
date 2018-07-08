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

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>

class NFException
{
private:
    NFException(){}

    static std::string _msgDelay;
    static int index;

    std::string _msg;
public:
    NFException(const char *format, ...)
	{
        index++;
        
        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay) + " ";
            _msgDelay.clear();
        }

		char buf[1024] = {0};
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		va_end(args);

        _msg.append(" NFException" + std::to_string(index) + " ");
		_msg.append(buf);
        
	}

    NFException(const std::string& msg)
    {
        index++;
        
        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay) + " ";
            _msgDelay.clear();
        }

        _msg.append(" NFException" + std::to_string(index) + " ");
        _msg.append(msg);
    }

    NFException(const std::string& msg, const char* file, const char* line)
    {
        index++;

        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay);
            _msgDelay.clear();
        }

        _msg.append(" NFException" + std::to_string(index) + " ");
        _msg.append(msg);
        _msg.append(file);
        _msg.append(line);
    }

    static void Delay(const std::string& msg, const char* file, const int line)
    {
        index++;

        _msgDelay.append(" NFException" + std::to_string(index) + " ");
        _msgDelay.append(msg);
        _msgDelay.append(file);
        _msgDelay.append(std::to_string(line));
    }

    char const * what() const noexcept{ return _msg.c_str(); }
/*
#if NF_PLATFORM == NF_PLATFORM_WIN
    char const * what() const noexcept{ return _msg.c_str(); }
#else
    char const * what() const noexcept{ return _msg.c_str(); }
#endif
*/
};

#endif
