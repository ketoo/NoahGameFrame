//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

bool NFRedisClient::APPEND(const std::string &key, const std::string &value, int& length)
{
	NFRedisCommand cmd(GET_NAME(APPEND));
    cmd << key;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		length = pReply->len;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::DECR(const std::string& key, int64_t& value)
{
	NFRedisCommand cmd(GET_NAME(DECR));
    cmd << key;

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

bool NFRedisClient::DECRBY(const std::string &key, const int64_t decrement, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(DECRBY));
    cmd << key;
    cmd << decrement;

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

bool NFRedisClient::GETSET(const std::string &key, const std::string &value, std::string &oldValue)
{
    NFRedisCommand cmd(GET_NAME(GETSET));
    cmd << key;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		oldValue = pReply->str;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::INCR(const std::string &key, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(INCR));
    cmd << key;

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

bool NFRedisClient::INCRBY(const std::string &key, const int64_t increment, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(INCRBY));
    cmd << key;
    cmd << increment;

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

bool NFRedisClient::INCRBYFLOAT(const std::string &key, const float increment, float& value)
{
    NFRedisCommand cmd(GET_NAME(INCRBYFLOAT));
    cmd << key;
    cmd << increment;

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

bool NFRedisClient::MGET(const string_vector &keys, string_vector &values)
{
	NFRedisCommand cmd(GET_NAME(MGET));

    for (int i = 0; i < keys.size(); ++i)
    {
        cmd << keys[i];
    }

	redisReply* pReply = BuildSendCmd(cmd);
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
				values.emplace_back(std::move(std::string(pReply->element[k]->str)));
			}
		}
	}

	freeReplyObject(pReply);

	return true;
}

void NFRedisClient::MSET(const string_pair_vector &values)
{
    NFRedisCommand cmd(GET_NAME(MSET));

    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply != nullptr)
	{
		freeReplyObject(pReply);
	}
}

bool NFRedisClient::SETEX(const std::string &key, const std::string &value, int time)
{
    NFRedisCommand cmd(GET_NAME(SETEX));
    cmd << key;
    cmd << value;
    cmd << time;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::SETNX(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SETNX));
    cmd << key;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int success = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		success = pReply->integer;
	}

	freeReplyObject(pReply);

	return bool(success);
}

bool NFRedisClient::STRLEN(const std::string &key, int& length)
{
    NFRedisCommand cmd(GET_NAME(STRLEN));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		length = (int)pReply->integer;
		freeReplyObject(pReply);
		return true;
	}

	freeReplyObject(pReply);

	return false;
}


bool NFRedisClient::SET(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SET));
    cmd << key;
    cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::GET(const std::string& key, std::string & value)
{
    NFRedisCommand cmd(GET_NAME(GET));
    cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STRING)
	{
		value.append(pReply->str, pReply->len);
	}

	freeReplyObject(pReply);

	return true;
}
