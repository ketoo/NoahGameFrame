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


#ifndef NF_TEAM_REDIS_MODULE_H
#define NF_TEAM_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFITeamRedisModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFTeamRedisModule : public NFITeamRedisModule
{
public:

	NFTeamRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual bool GetMemberList(const NFGUID& teamID, std::vector<MemberData>& teamMember);
	virtual bool GetMemberInvitationList(const NFGUID& teamID, std::vector<MemberData>& teamMember);

	virtual bool CreateTeam(const NFGUID& self, const std::string& name, const NFGUID& teamID);
	virtual bool DeleteTeam(const NFGUID& teamID);

	virtual bool AddToTeam(const NFGUID& teamID, const NFGUID& other, const std::string& name);
	virtual bool LeaveFromTeam(const NFGUID& teamID, const NFGUID& other);
	virtual bool IsTeamMember(const NFGUID& teamID, const NFGUID& other);
    
    virtual bool SendInvite(const NFGUID& teamID, const NFGUID& stranger, const std::string& name);
	virtual bool DeleteInvite(const NFGUID& teamID, const NFGUID& stranger);
	virtual bool DeleteAllInvite(const NFGUID& teamID);
	virtual bool IsInvited(const NFGUID& teamID, const NFGUID& stranger);

private:
    NFICommonRedisModule* m_pCommonRedisModule;
    NFINoSqlModule* m_pNoSqlModule;
};


#endif
