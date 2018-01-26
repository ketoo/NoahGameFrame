//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LINDEX(const std::string &key, const int index) 
{
	NFRedisCommand cmd(GET_NAME(LINDEX));
	cmd << key;
	cmd << index;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LLEN(const std::string &key)
{
	NFRedisCommand cmd(GET_NAME(LLEN));
	cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LPOP(const std::string &key)
{
	NFRedisCommand cmd(GET_NAME(LPOP));
	cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LPUSH(const std::string &key, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LPUSH));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LPUSHX(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(LPUSHX));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LRANGE(const std::string &key, const int start, const int end, string_vector &values) 
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

	const std::vector<NFRedisResult> xVector = pRedisResult->GetRespArray();
	if ((end - start + 1) == xVector.size())
	{
		for (int i = 0; i < xVector.size(); ++i)
		{
			values.push_back(xVector[i].GetRespString());
		}
	}

	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::LSET(const std::string &key, const int index, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(LSET));
	cmd << key;
	cmd << index;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::RPOP(const std::string &key)
{

	NFRedisCommand cmd(GET_NAME(RPOP));
	cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::RPUSH(const std::string &key, const std::string &value) 
{
	NFRedisCommand cmd(GET_NAME(RPUSH));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::RPUSHX(const std::string &key, const std::string &value)
{

	NFRedisCommand cmd(GET_NAME(RPUSHX));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}
