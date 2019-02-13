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


#ifndef NFI_WORLD_GUILD_MODULE_H
#define NFI_WORLD_GUILD_MODULE_H

#include "NFIModule.h"

class NFIWorldGuildModule
    : public NFIModule
{
public:
    virtual const NFGUID& CreateGuild(const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP) = 0;
    virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;

    virtual bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList) = 0;
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID) = 0;
    virtual bool MemberOffline(const NFGUID& self, const NFGUID& xGuild) = 0;

private:
};

#endif