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

int NFRedisClient::SADD(const std::string& key, const std::string& member)
{
	NFRedisCommand cmd(GET_NAME(SADD));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int add_new_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		add_new_num = (int)pReply->integer;
	}

	return add_new_num;
}

bool NFRedisClient::SCARD(const std::string& key, int& nCount)
{
	NFRedisCommand cmd(GET_NAME(SCARD));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		nCount = (int)pReply->integer;
	}

	return true;
}

bool NFRedisClient::SDIFF(const std::string& key_1, const std::string& key_2, string_vector& output)
{
	NFRedisCommand cmd(GET_NAME(SDIFF));
	cmd << key_1;
	cmd << key_2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

int NFRedisClient::SDIFFSTORE(const std::string& store_key, const std::string& diff_key1, const std::string& diff_key2)
{
	NFRedisCommand cmd(GET_NAME(SDIFFSTORE));
	cmd << store_key;
	cmd << diff_key1;
	cmd << diff_key2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	return num;
}

bool NFRedisClient::SINTER(const std::string& key_1, const std::string& key_2, string_vector& output)
{
	NFRedisCommand cmd(GET_NAME(SINTER));
	cmd << key_1;
	cmd << key_2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

int NFRedisClient::SINTERSTORE(const std::string& inter_store_key, const std::string& inter_key1, const std::string& inter_key2)
{
	NFRedisCommand cmd(GET_NAME(SINTERSTORE));
	cmd << inter_store_key;
	cmd << inter_key1;
	cmd << inter_key2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	return num;
}

bool NFRedisClient::SISMEMBER(const std::string& key, const std::string& member)
{
	NFRedisCommand cmd(GET_NAME(SISMEMBER));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER && pReply->integer == 1)
	{
		return true;
	}

	return false;
}

bool NFRedisClient::SMEMBERS(const std::string& key, string_vector& output)
{
	NFRedisCommand cmd(GET_NAME(SMEMBERS));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

bool NFRedisClient::SMOVE(const std::string& source_key, const std::string& dest_key, const std::string& member)
{
	NFRedisCommand cmd(GET_NAME(SMOVE));
	cmd << source_key;
	cmd << dest_key;
	cmd << member;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		return (bool)pReply->integer;
	}

	return false;
}

bool NFRedisClient::SPOP(const std::string& key, std::string& output)
{
	NFRedisCommand cmd(GET_NAME(SPOP));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		output = std::string(pReply->str, pReply->len);
		return true;
	}

	return false;
}

bool NFRedisClient::SRANDMEMBER(const std::string& key, int count, string_vector& output)
{
	NFRedisCommand cmd(GET_NAME(SRANDMEMBER));
	cmd << key;
	cmd << count;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

int NFRedisClient::SREM(const std::string& key, const string_vector& members)
{
	NFRedisCommand cmd(GET_NAME(SREM));
	cmd << key;
	for (const auto& member : members)
	{
		cmd << member;
	}

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	return num;
}

bool NFRedisClient::SUNION(const std::string& union_key1, const std::string& union_key2, string_vector& output)
{
	NFRedisCommand cmd(GET_NAME(SUNION));
	cmd << union_key1;
	cmd << union_key2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k++)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				output.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
			}
		}
	}

	return true;
}

int NFRedisClient::SUNIONSTORE(const std::string& dest_store_key, const std::string& union_key1, const std::string& union_key2)
{
	NFRedisCommand cmd(GET_NAME(SUNIONSTORE));
	cmd << dest_store_key;
	cmd << union_key1;
	cmd << union_key2;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	return num;
}


