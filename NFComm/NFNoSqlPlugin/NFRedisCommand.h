//
// Author: LUSHENG HUANG Created on 17/11/17.
//

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

    std::string Serialize()
    {
        std::stringstream xDataString;
        xDataString << '*' << mxParam.size() << NFREDIS_CRLF;
        std::vector<std::string>::iterator it = mxParam.begin();
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
