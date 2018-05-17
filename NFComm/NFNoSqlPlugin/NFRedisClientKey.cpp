//
// Author: LUSHENG HUANG Created on 17/11/17.
//
#include "NFRedisClient.h"

bool NFRedisClient::DEL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(DEL));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool del_key_num = false;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_key_num = (bool)pReply->integer;
	}

	return del_key_num;
}

bool NFRedisClient::EXISTS(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(EXISTS));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool exist = false;
	if (REDIS_REPLY_INTEGER == pReply->type && 1 == pReply->integer)
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

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->type && 1 == pReply->integer)
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

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->type && 1 == pReply->integer)
	{
		success = true;
	}

	return success;
}

bool NFRedisClient::PERSIST(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(PERSIST));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (REDIS_REPLY_INTEGER == pReply->type && 1 == pReply->integer)
	{
		success = true;
	}

	return success;
}

int NFRedisClient::TTL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TTL));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int left_time = -1;
	if (REDIS_REPLY_INTEGER == pReply->type)
	{
		left_time = (int)pReply->integer;
	}

	return left_time;
}

std::string NFRedisClient::TYPE(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TYPE));
    cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	std::string type_name = "";
	if (pReply->type == REDIS_REPLY_STATUS)
	{
		type_name = pReply->str;
	}

	return type_name;
}