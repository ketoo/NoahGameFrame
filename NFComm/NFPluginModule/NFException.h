/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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
    static std::string _msgDelay;
    static std::string indexStr;
    static int index;

    std::string _msg;
public:
    NFException(const char *format, ...)
	{
		char buf[1024] = {0};
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		_msg = std::string(buf);
		va_end(args);

        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

        index++;
        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay) + " ";
            _msgDelay.clear();
        }
	}

    NFException(const std::string& msg)
    {
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

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
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

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
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

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
