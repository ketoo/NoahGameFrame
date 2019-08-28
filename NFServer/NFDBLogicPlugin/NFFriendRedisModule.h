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


#ifndef NF_FRIEND_REDIS_MODULE_H
#define NF_FRIEND_REDIS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIFriendRedisModule.h"
#include "NFComm/NFPluginModule/NFICommonRedisModule.h"

class NFFriendRedisModule : public NFIFriendRedisModule
{
public:

	NFFriendRedisModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

    virtual bool GetFriendsList(const NFGUID& self, std::vector<FriendData>& friendList);
    virtual bool GetInvitationList(const NFGUID& self, std::vector<FriendData>& friendList);
    virtual bool AddFriend(const NFGUID& self, const NFGUID& stranger, const std::string& strangerName);
    virtual bool DeleteFriend(const NFGUID& self, const NFGUID& other);
    
    virtual bool SendInvite(const NFGUID& self, const std::string& selfName, const NFGUID& stranger);
    virtual bool AcceptInvite(const NFGUID& self, const std::string& selfName, const NFGUID& inviter);
    virtual bool RejectInvite(const NFGUID& self, const NFGUID& inviter);
    virtual bool IgnoreInvite(const NFGUID& self, const NFGUID& inviter);

    virtual bool BlockPlayer(const NFGUID& self, const NFGUID& other);
    virtual bool UnBlockPlayer(const NFGUID& self, const NFGUID& other);
    virtual bool IsBlockPlayer(const NFGUID& self, const NFGUID& other);

private:
    NFICommonRedisModule* m_pCommonRedisModule;
    NFINoSqlModule* m_pNoSqlModule;
};


#endif
