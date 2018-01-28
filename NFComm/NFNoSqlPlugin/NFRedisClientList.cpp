//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

bool NFRedisClient::LINDEX(const std::string &key, const int index, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LINDEX));
	cmd << key;
	cmd << index;

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
	{
		value = pRedisResult->GetRespString();
		return true;
	}
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

	return false;
}

bool NFRedisClient::LLEN(const std::string &key, int& length)
{
	NFRedisCommand cmd(GET_NAME(LLEN));
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
	{
		length = pRedisResult->GetRespInt();
		return true;
	}
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

bool NFRedisClient::LPOP(const std::string &key, std::string& value)
{
	NFRedisCommand cmd(GET_NAME(LPOP));
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
	{
		value = pRedisResult->GetRespString();
		return true;
	}
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


	return false;
}

int NFRedisClient::LPUSH(const std::string &key, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LPUSH));
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
	{
		return pRedisResult->GetRespInt();
	}
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_NIL:
		break;
	case NFREDIS_RESP_TYPE::NFREDIS_RESP_STATUS:
		break;
	default:
		break;
	}

	return 0;;
}

int NFRedisClient::LPUSHX(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(LPUSHX));
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
	{
		return pRedisResult->GetRespInt();
	}
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

bool NFRedisClient::LRANGE(const std::string &key, const int start, const int end, string_vector &values) 
{
	if (end - start <= 0)
	{
		return nullptr;
	}

	NFRedisCommand cmd(GET_NAME(LRANGE));
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
	{
		const std::vector<NFRedisResult> xVector = pRedisResult->GetRespArray();
		if ((end - start + 1) == xVector.size())
		{
			for (int i = 0; i < xVector.size(); ++i)
			{
				values.push_back(xVector[i].GetRespString());
			}

			return true;
		}
	}
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


	return false;
}

bool NFRedisClient::LSET(const std::string &key, const int index, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LSET));
	cmd << key;
	cmd << index;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

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

	WaitingResult(pRedisResult);


	return pRedisResult->IsOKRespStatus();
}

bool NFRedisClient::RPOP(const std::string &key, std::string& value)
{

	NFRedisCommand cmd(GET_NAME(RPOP));
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
	{
		value = pRedisResult->GetRespString();
		return true;
	}
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

	return false;
}

int NFRedisClient::RPUSH(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(RPUSH));
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
		return pRedisResult->GetRespInt();
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

int NFRedisClient::RPUSHX(const std::string &key, const std::string &value)
{

	NFRedisCommand cmd(GET_NAME(RPUSHX));
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
		return pRedisResult->GetRespInt();
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
