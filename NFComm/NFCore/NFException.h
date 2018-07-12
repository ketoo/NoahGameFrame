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
#include <stdarg.h>
#include <stdlib.h>
#include <exception>

class NFException
{
private:
    NFException(){}

    std::string _msg;
public:
	NFException(const char *format, ...);

	NFException(const std::string& msg);

	NFException(const std::string& msg, const char* file, const int line);
 
    char const * what() const noexcept{ return _msg.c_str(); }
};

#endif
