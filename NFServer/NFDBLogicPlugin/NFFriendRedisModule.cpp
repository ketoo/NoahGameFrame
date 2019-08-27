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

#include "NFFriendRedisModule.h"

NFFriendRedisModule::NFFriendRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFFriendRedisModule::Init()
{
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFFriendRedisModule::Shut()
{
	return true;
}

bool NFFriendRedisModule::Execute()
{
	return true;
}

bool NFFriendRedisModule::AfterInit()
{

	return true;
}

bool NFFriendRedisModule::GetFriendsList(const NFGUID& self, NFList<FriendData>& friendList)
{
	std::string strKey = m_pCommonRedisModule->GetFriendCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		std::vector<string_pair> values;
		if (xNoSqlDriver->HGETALL(strKey, values))
		{
			for (int i = 0; i < values.size(); ++i)
			{
				FriendData friendData;
				friendData.id = values[i].first;
				friendData.name = values[i].second;

				friendList.Add(friendData);
			}

			return true;
		}
	}
    return false;
}

bool NFFriendRedisModule::GetInvitationList(const NFGUID& self, NFList<FriendData>& friendList)
{
	std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		std::vector<string_pair> values;
		if (xNoSqlDriver->HGETALL(strKey, values))
		{
			for (int i = 0; i < values.size(); ++i)
			{
				FriendData friendData;
				friendData.id = values[i].first;
				friendData.name = values[i].second;

				friendList.Add(friendData);
			}

			return true;
		}
	}
    return false;
}

bool NFFriendRedisModule::SendInvite(const NFGUID& self, const std::string& selfName, const NFGUID& stranger)
{
    std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(stranger);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HSET(strKey, self.ToString(), selfName))
		{
			return true;
		}
	}

    return false;
}

bool NFFriendRedisModule::AcceptInvite(const NFGUID& self, const std::string& selfName, const NFGUID& inviter)
{
    std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
        std::string strangerName;
		if (xNoSqlDriver->HGET(strKey, inviter.ToString(), strangerName))
		{
            AddFriend(self, inviter, strangerName);
            AddFriend(inviter, self, selfName);
			return true;
		}
	}

    return false;
}

bool NFFriendRedisModule::RejectInvite(const NFGUID& self, const NFGUID& inviter)
{
    std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
        xNoSqlDriver->HDEL(strKey, inviter.ToString());
		return true;
	}

    return false;
}

bool NFFriendRedisModule::IgnoreInvite(const NFGUID& self, const NFGUID& inviter)
{
    std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
        xNoSqlDriver->HDEL(strKey, inviter.ToString());
		return true;
	}

    return false;
}

bool NFFriendRedisModule::AddFriend(const NFGUID& self, const NFGUID& stranger, const std::string& strangerName)
{
    std::string strKey = m_pCommonRedisModule->GetFriendCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HSET(strKey, stranger.ToString(), strangerName))
		{
			return true;
		}
	}

    return false;
}

bool NFFriendRedisModule::DeleteFriend(const NFGUID& self, const NFGUID& other)
{
    std::string strKey = m_pCommonRedisModule->GetFriendInviteCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
        xNoSqlDriver->HDEL(strKey, other.ToString());
		return true;
	}

    return false;
}

bool NFFriendRedisModule::BlockPlayer(const NFGUID& self, const NFGUID& other)
{
    std::string strKey = m_pCommonRedisModule->GetBlockCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->HSET(strKey, other.ToString(), ""))
		{
			return true;
		}
	}

    return false;
}

bool NFFriendRedisModule::UnBlockPlayer(const NFGUID& self, const NFGUID& other)
{
    std::string strKey = m_pCommonRedisModule->GetBlockCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		xNoSqlDriver->HDEL(strKey, other.ToString());
		return true;
	}

    return false;
}

bool NFFriendRedisModule::IsBlockPlayer(const NFGUID& self, const NFGUID& other)
{
	    std::string strKey = m_pCommonRedisModule->GetBlockCacheKey(self);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strKey);
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HEXISTS(strKey, other.ToString());
	}

    return false;
}