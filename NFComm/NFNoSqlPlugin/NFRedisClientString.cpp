//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"



NF_SHARE_PTR<NFRedisResult>NFRedisClient::APPEND(const std::string &key, const std::string &value)
{
	NFRedisCommand cmd(GET_NAME(APPEND));
    cmd << key;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::DECR(const std::string &key)
{
	NFRedisCommand cmd(GET_NAME(DECR));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::DECRBY(const std::string &key, const int64_t v)
{
    NFRedisCommand cmd(GET_NAME(DECRBY));
    cmd << key;
    cmd << v;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::GETSET(const std::string &key, const std::string &value)
{

    NFRedisCommand cmd(GET_NAME(GETSET));
    cmd << key;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::INCR(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(INCR));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::INCRBY(const std::string &key, const int64_t v)
{
    NFRedisCommand cmd(GET_NAME(INCRBY));
    cmd << key;
    cmd << v;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::INCRBYFLOAT(const std::string &key, const float fv)
{
    NFRedisCommand cmd(GET_NAME(INCRBYFLOAT));
    cmd << key;
    cmd << fv;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::MGET(const string_vector &keys, string_vector &values)
{
	NFRedisCommand cmd(GET_NAME(MGET));

    for (int i = 0; i < keys.size(); ++i)
    {
        cmd << keys[i];
    }


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);



	const std::vector<NFRedisResult> xVector = pRedisResult->GetRespArray();
	if (keys.size() == xVector.size())
	{
		for (int i = 0; i < xVector.size(); ++i)
		{
			values.push_back(xVector[i].GetRespString());
		}
	}

	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::MSET(const string_pair_vector &values)
{
    NFRedisCommand cmd(GET_NAME(MSET));

    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }
	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::SETEX(const std::string &key, const std::string &value, int time)
{
    NFRedisCommand cmd(GET_NAME(SETEX));
    cmd << key;
    cmd << value;
    cmd << time;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::SETNX(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SETNX));
    cmd << key;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::STRLEN(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(STRLEN));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}


NF_SHARE_PTR<NFRedisResult> NFRedisClient::SET(const std::string &key, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(SET));
    cmd << key;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult> NFRedisClient::GET(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(GET));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}
