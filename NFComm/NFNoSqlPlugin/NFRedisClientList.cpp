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

#include "NFRedisClient.h"

bool NFRedisClient::LINDEX(const std::string &key, const int index, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LINDEX));
	cmd << key;
	cmd << index;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		value = std::string(pReply->str, pReply->len);
	}

	return true;
}

bool NFRedisClient::LLEN(const std::string &key, int& length)
{
	NFRedisCommand cmd(GET_NAME(LLEN));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		length = (int)pReply->integer;
	}

	return true;
}

bool NFRedisClient::LPOP(const std::string &key, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LPOP));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		value = std::string(pReply->str, pReply->len);
	}

	return true;
}

int NFRedisClient::LPUSH(const std::string &key, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LPUSH));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		list_len = (int)pReply->integer;
	}

	return list_len;
}

int NFRedisClient::LPUSHX(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(LPUSHX));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		list_len = (int)pReply->integer;
	}

	return list_len;
}

bool NFRedisClient::LRANGE(const std::string &key, const int start, const int end, string_vector &values) 
{
	if (end - start <= 0)
	{
		return false;
	}

	NFRedisCommand cmd(GET_NAME(LRANGE));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				values.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

bool NFRedisClient::LSET(const std::string &key, const int index, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LSET));
	cmd << key;
	cmd << index;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::RPOP(const std::string &key, std::string& value)
{

	NFRedisCommand cmd(GET_NAME(RPOP));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		value = std::string(pReply->str, pReply->len);
	}

	return true;
}

int NFRedisClient::RPUSH(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(RPUSH));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		list_len = (int)pReply->integer;
	}

	return list_len;
}

int NFRedisClient::RPUSHX(const std::string &key, const std::string &value)
{

	NFRedisCommand cmd(GET_NAME(RPUSHX));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		list_len = (int)pReply->integer;
	}

	return list_len;
}
