/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

#include "NFRedisClient.h"

bool NFRedisClient::DEL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(DEL));
    cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool del_key_num = false;
	if (pReply->reply->type == REDIS_REPLY_INTEGER)
	{
		del_key_num = (bool)pReply->reply->integer;

		return true;
	}

	return false;
}

bool NFRedisClient::EXISTS(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(EXISTS));
    cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool exist = false;
	if (REDIS_REPLY_INTEGER == pReply->reply->type && 1 == pReply->reply->integer)
	{
		exist = true;
	}

	return exist;
}

bool NFRedisClient::EXPIRE(const std::string &key, const unsigned int secs)
{
    NFRedisCommand cmd(GET_NAME(EXPIRE));
    cmd << key;
    cmd << secs;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->reply->type && 1 == pReply->reply->integer)
	{
		success = true;
	}

	return success;
}

bool NFRedisClient::EXPIREAT(const std::string &key, const int64_t unixTime)
{
    NFRedisCommand cmd(GET_NAME(EXPIREAT));
    cmd << key;
    cmd << unixTime;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->reply->type && 1 == pReply->reply->integer)
	{
		success = true;
	}

	return success;
}

bool NFRedisClient::PERSIST(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(PERSIST));
    cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->reply->type && 1 == pReply->reply->integer)
	{
		success = true;
	}

	return success;
}

bool NFRedisClient::TTL(const std::string &key, int& ttl)
{
    NFRedisCommand cmd(GET_NAME(TTL));
    cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (REDIS_REPLY_INTEGER == pReply->reply->type)
	{
		ttl = (int)pReply->reply->integer;

		return true;
	}

	return false;
}

bool NFRedisClient::TYPE(const std::string &key, std::string& type)
{
    NFRedisCommand cmd(GET_NAME(TYPE));
    cmd << key;

	NF_SHARE_PTR<NFRedisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr || pReply->reply == nullptr)
	{
		return false;
	}

	if (pReply->reply->type == REDIS_REPLY_STATUS)
	{
		type = pReply->reply->str;

		return true;
	}

	return false;
}