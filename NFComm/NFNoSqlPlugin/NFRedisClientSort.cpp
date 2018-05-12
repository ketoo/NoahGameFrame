//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

int NFRedisClient::ZADD(const std::string & key, const std::string & member, const double score)
{
	NFRedisCommand cmd(GET_NAME(ZADD));
	cmd << key;
	cmd << member;
	cmd << score;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int add_new_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		add_new_num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return add_new_num;
}

int NFRedisClient::ZCARD(const std::string & key)
{
	NFRedisCommand cmd(GET_NAME(ZCARD));
	cmd << key;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return num;
}

int NFRedisClient::ZCOUNT(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZCOUNT));
	cmd << key;
	cmd << start;
	cmd << end;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::ZINCRBY(const std::string & key, const std::string & member, const double score, double& newScore)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << member;
	cmd << score;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->str, newScore);
	}

	freeReplyObject(pReply);

	return success;
}

bool NFRedisClient::ZRANGE(const std::string & key, const int start, const int end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";
	
	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k = k + 2)
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

bool NFRedisClient::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_pair_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k = k + 2)
		{
			if (pReply->element[k]->type == REDIS_REPLY_STRING)
			{
				values.emplace_back(std::move(
					string_pair(std::string(pReply->element[k]->str, pReply->element[k]->len),
						std::string(pReply->element[k + 1]->str, pReply->element[k + 1]->len))
				));
			}
		}
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::ZRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::ZREM(const std::string & key, const std::string & member)
{
	NFRedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return (bool)del_num;
}

bool NFRedisClient::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
	cmd << key;
	cmd << start;
	cmd << end;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	int del_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return (bool)del_num;
}

bool NFRedisClient::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}


	int del_num = 0;
	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		del_num = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return (bool)del_num;
}

bool NFRedisClient::ZREVRANGE(const std::string& key, const int start, const int end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGE));
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
		for (size_t k = 0; k < pReply->elements; k = k + 2)
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

bool NFRedisClient::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_ARRAY)
	{
		for (size_t k = 0; k < pReply->elements; k = k + 2)
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

bool NFRedisClient::ZREVRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->integer;
	}

	freeReplyObject(pReply);

	return true;
}

bool NFRedisClient::ZSCORE(const std::string & key, const std::string & member, double& score)
{
	NFRedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;


	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->str, score);
	}

	freeReplyObject(pReply);

	return success;
}
