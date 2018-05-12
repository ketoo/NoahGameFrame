//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

bool NFRedisClient::LINDEX(const std::string &key, const int index, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LINDEX));
	cmd << key;
	cmd << index;

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

bool NFRedisClient::LLEN(const std::string &key, int& length)
{
	NFRedisCommand cmd(GET_NAME(LLEN));
	cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		length = pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::LPOP(const std::string &key, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LPOP));
	cmd << key;

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

int NFRedisClient::LPUSH(const std::string &key, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LPUSH));
	cmd << key;
	cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = pReply->integer;

	freeReplyObject(pReply);

	return list_len;
}

int NFRedisClient::LPUSHX(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(LPUSHX));
	cmd << key;
	cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = pReply->integer;

	freeReplyObject(pReply);

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

bool NFRedisClient::LSET(const std::string &key, const int index, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LSET));
	cmd << key;
	cmd << index;
	cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::RPOP(const std::string &key, std::string& value)
{

	NFRedisCommand cmd(GET_NAME(RPOP));
	cmd << key;

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

int NFRedisClient::RPUSH(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(RPUSH));
	cmd << key;
	cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = pReply->integer;

	freeReplyObject(pReply);

	return list_len;
}

int NFRedisClient::RPUSHX(const std::string &key, const std::string &value)
{

	NFRedisCommand cmd(GET_NAME(RPUSHX));
	cmd << key;
	cmd << value;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int list_len = pReply->integer;

	freeReplyObject(pReply);

	return list_len;
}
