//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

int NFRedisClient::HDEL(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HDEL));
    cmd << key;
    cmd << field;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = (int)pReply->integer;

	freeReplyObject(pReply);

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

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = (int)pReply->integer;

	freeReplyObject(pReply);

	return del_num;
}

bool NFRedisClient::HEXISTS(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HEXISTS));
    cmd << key;
    cmd << field;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool exist = false;
	if (pReply->type == REDIS_REPLY_INTEGER && pReply->integer == 1)
	{
		exist = true;
	}

	freeReplyObject(pReply);

	return exist;
}

bool NFRedisClient::HGET(const std::string& key, const std::string& field, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(HGET));
	cmd << key;
	cmd << field;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		value = std::string(pReply->str, pReply->len);
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
    NFRedisCommand cmd(GET_NAME(HGETALL));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
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

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HINCRBY(const std::string &key, const std::string &field, const int by, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(HINCRBY));
    cmd << key;
    cmd << field;
    cmd << by;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		value = pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by, float& value)
{
    NFRedisCommand cmd(GET_NAME(HINCRBYFLOAT));
    cmd << key;
    cmd << field;
    cmd << by;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<float>(pReply->str, value);
	}

	freeReplyObject(pReply);

	return success;
}

bool NFRedisClient::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
    NFRedisCommand cmd(GET_NAME(HKEYS));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
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

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HLEN(const std::string &key, int& number)
{
    NFRedisCommand cmd(GET_NAME(HLEN));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		number = (int)pReply->integer;
	}

	freeReplyObject(pReply);

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

	redisReply* pReply = BuildSendCmd(cmd);
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

	freeReplyObject(pReply);

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

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	freeReplyObject(pReply);

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

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HSET(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSET));
    cmd << key;
    cmd << field;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)pReply->integer;
	}

	freeReplyObject(pReply);

	return success;
}

bool NFRedisClient::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSETNX));
    cmd << key;
    cmd << field;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		success = (bool)pReply->integer;
	}

	freeReplyObject(pReply);

	return success;
}

bool NFRedisClient::HVALS(const std::string &key, string_vector &values)
{
    NFRedisCommand cmd(GET_NAME(HVALS));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
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

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::HSTRLEN(const std::string &key, const std::string &field, int& length)
{
    NFRedisCommand cmd(GET_NAME(HSTRLEN));
    cmd << key;
    cmd << field;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		length = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}
