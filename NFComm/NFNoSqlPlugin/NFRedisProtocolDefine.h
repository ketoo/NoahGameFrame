/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H
#define NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H
#include <string>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

/*
状态回复（status reply）的第一个字节是 "+"           //For Simple Strings the first byte of the reply is "+"
错误回复（error reply）的第一个字节是 "-"            //For Errors the first byte of the reply is "-"
整数回复（integer reply）的第一个字节是 ":"          //For Integers the first byte of the reply is ":"
批量回复（bulk reply）的第一个字节是 "$"             //For Bulk Strings the first byte of the reply is "$"
多条批量回复（multi bulk reply）的第一个字节是 "*"    //For Arrays the first byte of the reply is "*"
 */
enum NFREDIS_RESULT_STATUS
{
	NFREDIS_RESULT_STATUS_OK,
	NFREDIS_RESULT_STATUS_UNKNOW,
	NFREDIS_RESULT_STATUS_IMCOMPLETE,
};

enum NFREDIS_RESP_TYPE
{
    NFREDIS_RESP_UNKNOW,
    NFREDIS_RESP_NIL,
    NFREDIS_RESP_STATUS,
    NFREDIS_RESP_ERROR,
    NFREDIS_RESP_INT,
    NFREDIS_RESP_BULK,
    NFREDIS_RESP_ARRAY,
};

static const std::string NFREDIS_CRLF = "\r\n";	///结束标志 terminated by CRLF
static const int NFREDIS_SIZEOF_CRLF = 2;	///the size of CRLF
static const std::string NFREDIS_STATUS_OK = "OK";

static const char* NFREDIS_STATUS_REPLY = "+";    //状态回复（status reply）的第一个字节是 "+"
static const char* NFREDIS_ERROR_REPLY = "-";	    //错误回复（error reply）的第一个字节是 "-"
static const char* NFREDIS_INT_REPLY = ":";	    //整数回复（integer reply）的第一个字节是 ":"
static const char* NFREDIS_BULK_REPLY = "$";	    //批量回复（bulk reply）的第一个字节是 "$"
static const char* NFREDIS_ARRAY_REPLY = "*";	    //多条批量回复（multi bulk reply）的第一个字节是 "*"

#define GET_NAME(functionName)   (#functionName)

#endif //NFREDISPLUGIN_NFREDISPROTOCOLDEFINE_H
