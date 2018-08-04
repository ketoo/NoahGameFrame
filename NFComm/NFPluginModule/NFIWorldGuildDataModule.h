/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: flyicegood
   
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


#ifndef NFI_WORLD_GUILD_DATA_MODULE_H
#define NFI_WORLD_GUILD_DATA_MODULE_H

#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

class NFIWorldGuildDataModule
    : public NFIModule
{
public:
    struct SearchGuildObject
    {
        SearchGuildObject()
        {
            mnGuildMemberCount  = 0;
            mnGuildMemberMaxCount  = 0;
            mnGuildHonor = 0;
            mnGuildRank  = 0;
        }

        NFGUID      mxGuildID    ;
        std::string     mstrGuildName  ;
        std::string     mnGuildIcon  ;
        NFINT32         mnGuildMemberCount  ;
        NFINT32         mnGuildMemberMaxCount  ;
        NFINT32         mnGuildHonor ;
        NFINT32         mnGuildRank  ;
    };

    virtual bool ExitGuild(const NFGUID& self, const std::string& strName, bool& bExit) = 0;
    virtual void CheckLoadGuild(const NFGUID& self, const NFGUID& xGuild) = 0;
    virtual bool GetPlayerInfo(const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP) = 0;

    virtual const NFGUID CreateGuild(const NFGUID& xPlayeID, const std::string& strName, const std::string& strRoleName,
                                     const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1, const int nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT) = 0;
    virtual const bool DeleteGuild(const NFGUID& xGuild) = 0;
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFGUID& xGuild) = 0;

    virtual bool GetPlayerGuild(const NFGUID& self, NFGUID& xGuild) = 0;
    virtual bool SearchGuild(const NFGUID& self, const std::string& strName, std::vector<SearchGuildObject>& xList) = 0;
    virtual bool GetPlayerGameID(const NFGUID& self, int& nGameID) = 0;

protected:
private:
};

#endif