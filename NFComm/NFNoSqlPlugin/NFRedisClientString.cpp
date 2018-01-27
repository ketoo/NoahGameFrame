//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

bool NFRedisClient::APPEND(const std::string &key, const std::string &value, int& length)
{
	NFRedisCommand cmd(GET_NAME(APPEND));
    cmd << key;
    cmd << value;

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

bool NFRedisClient::DECR(const std::string& key, int64_t& value)
{
	NFRedisCommand cmd(GET_NAME(DECR));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::DECRBY(const std::string &key, const int64_t decrement, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(DECRBY));
    cmd << key;
    cmd << decrement;

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

bool NFRedisClient::GETSET(const std::string &key, const std::string &value, std::string &oldValue)
{
    NFRedisCommand cmd(GET_NAME(GETSET));
    cmd << key;
    cmd << value;

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

bool NFRedisClient::INCR(const std::string &key, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(INCR));
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

	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::INCRBY(const std::string &key, const int64_t increment, int64_t& value)
{
    NFRedisCommand cmd(GET_NAME(INCRBY));
    cmd << key;
    cmd << increment;

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

bool NFRedisClient::INCRBYFLOAT(const std::string &key, const float increment, float& value)
{
    NFRedisCommand cmd(GET_NAME(INCRBYFLOAT));
    cmd << key;
    cmd << increment;

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

bool NFRedisClient::MGET(const string_vector &keys, string_vector &values)
{
	NFRedisCommand cmd(GET_NAME(MGET));

    for (int i = 0; i < keys.size(); ++i)
    {
        cmd << keys[i];
    }


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


	const std::vector<NFRedisResult> xVector = pRedisResult->GetRespArray();
	if (keys.size() == xVector.size())
	{
		for (int i = 0; i < xVector.size(); ++i)
		{
			values.push_back(xVector[i].GetRespString());
		}
	}

	return pRedisResult->IsOKRespStatus();
}

void NFRedisClient::MSET(const string_pair_vector &values)
{
    NFRedisCommand cmd(GET_NAME(MSET));

    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }
	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);
}

bool NFRedisClient::SETEX(const std::string &key, const std::string &value, int time)
{
    NFRedisCommand cmd(GET_NAME(SETEX));
    cmd << key;
    cmd << value;
    cmd << time;

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

bool NFRedisClient::SETNX(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SETNX));
    cmd << key;
    cmd << value;

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

bool NFRedisClient::STRLEN(const std::string &key, int& length)
{
    NFRedisCommand cmd(GET_NAME(STRLEN));
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

	return pRedisResult->IsOKRespStatus();
}


bool NFRedisClient::SET(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SET));
    cmd << key;
    cmd << value;

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

bool NFRedisClient::GET(const std::string& key, std::string & value)
{
    NFRedisCommand cmd(GET_NAME(GET));
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

	return pRedisResult->IsOKRespStatus();
}
