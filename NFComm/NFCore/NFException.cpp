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

#include "NFException.h"


int exception_index = 0;

NFException::NFException(const std::string & msg)
{
	exception_index++;

	_msg.append(" NFException" + std::to_string(exception_index) + " ");
	_msg.append(msg);
}

NFException::NFException(const std::string& msg, const char* file, const int line)
{
	exception_index++;

	_msg.append(" NFException" + std::to_string(exception_index) + " ");
	_msg.append(msg);
	_msg.append(file);
	_msg.append(std::to_string(line));
}