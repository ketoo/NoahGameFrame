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

bool NFRedisClient::ZCARD(const std::string & key, int &nCount)
{
	NFRedisCommand cmd(GET_NAME(ZCARD));
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

bool NFRedisClient::ZCOUNT(const std::string & key, const double start, const double end, int &nCount)
{
	NFRedisCommand cmd(GET_NAME(ZCOUNT));
	cmd << key;
	cmd << start;
	cmd << end;

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

bool NFRedisClient::ZINCRBY(const std::string & key, const std::string & member, const double score, double& newScore)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << member;
	cmd << score;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->str, newScore);
	}

	return success;
}

bool NFRedisClient::ZRANGE(const std::string & key, const int start, const int end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";
	
	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->element[k]->str, pReply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->element[k]->str, pReply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::ZRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->integer;
	}

	return true;
}

bool NFRedisClient::ZREM(const std::string & key, const std::string & member)
{
	NFRedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

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

	return (bool)del_num;
}

bool NFRedisClient::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
	cmd << key;
	cmd << start;
	cmd << end;

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

	return (bool)del_num;
}

bool NFRedisClient::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;

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

	return (bool)del_num;
}

bool NFRedisClient::ZREVRANGE(const std::string& key, const int start, const int end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGE));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->element[k]->str, pReply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	try
	{
		if (pReply->type == REDIS_REPLY_ARRAY)
		{
			for (size_t k = 0; k < pReply->elements; k = k + 2)
			{
				if (pReply->element[k]->type == REDIS_REPLY_STRING)
				{
					string_score_pair vecPair;
					vecPair.first = std::string(pReply->element[k]->str, pReply->element[k]->len);
					vecPair.second = lexical_cast<double>(pReply->element[k + 1]->str);
					values.emplace_back(vecPair);
				}
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::ZREVRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_INTEGER)
	{
		rank = (int)pReply->integer;
	}

	return true;
}

bool NFRedisClient::ZSCORE(const std::string & key, const std::string & member, double& score)
{
	NFRedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;


	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	bool success = false;
	if (pReply->type == REDIS_REPLY_STRING)
	{
		success = NF_StrTo<double>(pReply->str, score);
	}

	return success;
}
