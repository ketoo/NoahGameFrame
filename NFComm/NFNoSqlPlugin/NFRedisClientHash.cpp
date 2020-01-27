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

int NFRedisClient::HDEL(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HDEL));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = 0; 
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_num = (int)pReply->integer;
	}

	return del_num;
}

int NFRedisClient::HDEL(const std::string &key, const string_vector& fields)
{
	NFRedisCommand cmd(GET_NAME(HDEL));
	cmd << key;
	for (string_vector::const_iterator it = fields.begin();
		it != fields.end(); ++it)
	{
		cmd << *it;
	}

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_num = (int)pReply->integer;
	}

	return del_num;
}

bool NFRedisClient::HEXISTS(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HEXISTS));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool exist = false;
	if (pReply->type == REDIS_REPLY_INTEGER && pReply->integer == 1)
	{
		exist = true;
	}

	return exist;
}

bool NFRedisClient::HGET(const std::string& key, const std::string& field, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(HGET));
	cmd << key;
	cmd << field;

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

bool NFRedisClient::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
    NFRedisCommand cmd(GET_NAME(HGETALL));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)pReply->elements; k = k + 2)
		{
			values.emplace_back(std::move(string_pair{ std::string(pReply->element[k]->str, pReply->element[k]->len),
				std::string(pReply->element[k + 1]->str, pReply->element[k + 1]->len) }));
		}
	}

	return true;
}

bool NFRedisClient::HINCRBY(const std::string &key, const std::string &field, const int by, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(HINCRBY));
    cmd << key;
    cmd << field;
    cmd << by;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		value = pReply->integer;
	}

	return true;
}

bool NFRedisClient::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by, float& value)
{
    NFRedisCommand cmd(GET_NAME(HINCRBYFLOAT));
    cmd << key;
    cmd << field;
    cmd << by;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<float>(pReply->str, value);
	}

	return success;
}

bool NFRedisClient::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
    NFRedisCommand cmd(GET_NAME(HKEYS));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)pReply->elements; k++)
		{
			fields.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
		}
	}

	return true;
}

bool NFRedisClient::HLEN(const std::string &key, int& number)
{
    NFRedisCommand cmd(GET_NAME(HLEN));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		number = (int)pReply->integer;
	}

	return true;
}

bool NFRedisClient::HMGET(const std::string &key, const string_vector &fields, string_vector &values)
{
    NFRedisCommand cmd(GET_NAME(HMGET));
    cmd << key;
    for (int i = 0; i < fields.size(); ++i)
    {
        cmd << fields[i];
    }

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

bool NFRedisClient::HMSET(const std::string &key, const std::vector<string_pair> &values)
{
    NFRedisCommand cmd(GET_NAME(HMSET));
    cmd << key;
    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::HMSET(const std::string & key, const string_vector & fields, const string_vector & values)
{
	if (fields.size() != values.size())
	{
		return false;
	}

	NFRedisCommand cmd(GET_NAME(HMSET));
	cmd << key;
	for (int i = 0; i < values.size(); ++i)
	{
		cmd << fields[i];
		cmd << values[i];
	}

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::HSET(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSET));
    cmd << key;
    cmd << field;
    cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	//bool success = false;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		 //success = (bool)pReply->integer;
		/*
		Return value
		Integer reply, specifically:
		1 if field is a new field in the hash and value was set.
		0 if field already exists in the hash and the value was updated.
		*/
	}

	return true;
}

bool NFRedisClient::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSETNX));
    cmd << key;
    cmd << field;
    cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)pReply->integer;
	}

	return success;
}

bool NFRedisClient::HVALS(const std::string &key, string_vector &values)
{
    NFRedisCommand cmd(GET_NAME(HVALS));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (int k = 0; k < (int)pReply->elements; k++)
		{
			values.emplace_back(std::move(std::string(pReply->element[k]->str, pReply->element[k]->len)));
		}
	}

	return true;
}

bool NFRedisClient::HSTRLEN(const std::string &key, const std::string &field, int& length)
{
    NFRedisCommand cmd(GET_NAME(HSTRLEN));
    cmd << key;
    cmd << field;

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
