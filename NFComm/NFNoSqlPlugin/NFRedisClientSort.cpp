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


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return 0;
}

int NFRedisClient::ZCARD(const std::string & key)
{
	NFRedisCommand cmd(GET_NAME(ZCARD));
	cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return 0;
}

int NFRedisClient::ZCOUNT(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZCOUNT));
	cmd << key;
	cmd << start;
	cmd << end;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}


	return pRedisResult->GetRespInt();
}

bool NFRedisClient::ZINCRBY(const std::string & key, const std::string & member, const double score, double& newScore)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << member;
	cmd << score;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZRANGE(const std::string & key, const int start, const int end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";
	

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREM(const std::string & key, const std::string & member)
{
	NFRedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREVRANGE(const std::string& key, const int start, const int end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGE));
	cmd << key;
	cmd << start;
	cmd << end;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZREVRANK(const std::string & key, const std::string & member, int& rank)
{
	NFRedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::ZSCORE(const std::string & key, const std::string & member, double& score)
{
	NFRedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	switch (pRedisResult->GetRespType())
	{
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ERROR:
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_UNKNOW:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_ARRAY:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_BULK:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_INT:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return pRedisResult->IsOKRespStatus();
}
