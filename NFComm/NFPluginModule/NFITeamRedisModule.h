/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#ifndef NFI_TEAM_REDIS_MODULE_H
#define NFI_TEAM_REDIS_MODULE_H

#include "NFIModule.h"

class NFITeamRedisModule
    : public NFIModule
{

public:
    struct MemberData
    {
        NFGUID id;
        std::string name;
		int bp;
		int leader;
    };

	virtual bool GetMemberList(const NFGUID& teamID, std::vector<MemberData>& teamMember) = 0;
	virtual bool GetMemberInvitationList(const NFGUID& teamID, std::vector<MemberData>& teamMember) = 0;

    virtual bool CreateTeam(const NFGUID& self, const NFGUID& name, const NFGUID& teamID) = 0;
	virtual bool AddToTeam(const NFGUID& teamID, const NFGUID& other) = 0;
	virtual bool LeaveFromTeam(const NFGUID& teamID, const NFGUID& other) = 0;
    
    virtual bool SendInvite(const NFGUID& self, const NFGUID& stranger) = 0;
    virtual bool AcceptInvite(const NFGUID& self, const NFGUID& inviter) = 0;
    virtual bool RejectInvite(const NFGUID& self, const NFGUID& inviter) = 0;


};

#endif