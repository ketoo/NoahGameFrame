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

#ifndef NFREDISPLUGIN_NFREDISCOMMAND_H
#define NFREDISPLUGIN_NFREDISCOMMAND_H

#include <iostream>
#include <vector>
#include <sstream>

#include "NFRedisProtocolDefine.h"

class NFRedisCommand
{
public:
    NFRedisCommand( const std::string& cmd )
    {
        mxParam.push_back( cmd );
    }

    ~NFRedisCommand()
    {

    }

    template <typename T>
    NFRedisCommand& operator<<( const T& t )
    {
        std::stringstream str;
        str << t;
        mxParam.push_back(str.str());
        return *this;
    }

    std::string Serialize() const
    {
        std::stringstream xDataString;
        xDataString << '*' << mxParam.size() << NFREDIS_CRLF;
        std::vector<std::string>::const_iterator it = mxParam.begin();
        for ( ; it != mxParam.end(); ++it )
        {
            xDataString << '$' << it->size() << NFREDIS_CRLF;
            xDataString << *it << NFREDIS_CRLF;
        }

        return xDataString.str();
    }

private:
    std::vector<std::string> mxParam;
};


#endif //NFREDISPLUGIN_NFREDISCOMMAND_H
