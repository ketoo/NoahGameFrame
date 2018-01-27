//
// Author: LUSHENG HUANG Created on 17/11/17.
//
#include "NFRedisClient.h"

bool NFRedisClient::DEL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(DEL));
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
		return pRedisResult->GetRespInt();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return false;
}

bool NFRedisClient::EXISTS(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(EXISTS));
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
		return (pRedisResult->GetRespInt() > 0);
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return false;
}

bool NFRedisClient::EXPIRE(const std::string &key, const unsigned int secs)
{
    NFRedisCommand cmd(GET_NAME(EXPIRE));
    cmd << key;
    cmd << secs;


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
		return (pRedisResult->GetRespInt() > 0);
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return false;
}

bool NFRedisClient::EXPIREAT(const std::string &key, const int64_t unixTime)
{
    NFRedisCommand cmd(GET_NAME(EXPIREAT));
    cmd << key;
    cmd << unixTime;


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
		return pRedisResult->IsOKRespStatus();
		break;
	default:
		break;
	}

	return false;
}

bool NFRedisClient::PERSIST(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(PERSIST));
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
		return (pRedisResult->GetRespInt() > 0);
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return false;
}

int NFRedisClient::TTL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TTL));
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
		return pRedisResult->GetRespInt();
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return false;
}

std::string NFRedisClient::TYPE(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TYPE));
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
	{
		if (pRedisResult->GetRespString() == "none")
		{
			return "";
		}

		return pRedisResult->GetRespString();
	}
		break;
	default:
		break;
	}

	return false;
}