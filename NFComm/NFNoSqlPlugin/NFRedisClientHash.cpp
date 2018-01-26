//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult *NFRedisClient::HDEL(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HDEL));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HEXISTS(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HEXISTS));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HGET(const std::string &key, const std::string &field)
{
    NFRedisCommand cmd(GET_NAME(HGET));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
    NFRedisCommand cmd(GET_NAME(HGETALL));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	const std::vector<NFRedisResult> xVector = pRedisResult->GetRespArray();
	if (xVector.size() % 2 == 0)
	{
		for (int i = 0; i < xVector.size(); i+=2)
		{
			values.push_back(string_pair(xVector[i].GetRespString(), xVector[i+1].GetRespString()));
		}
	}

	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HINCRBY(const std::string &key, const std::string &field, const int by)
{
    NFRedisCommand cmd(GET_NAME(HINCRBY));
    cmd << key;
    cmd << field;
    cmd << by;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by)
{
    NFRedisCommand cmd(GET_NAME(HINCRBYFLOAT));
    cmd << key;
    cmd << field;
    cmd << by;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
    NFRedisCommand cmd(GET_NAME(HKEYS));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	const std::vector<NFRedisResult>& xRedisResultList = pRedisResult->GetRespArray();
	fields.clear();

	for (int i = 0; i < xRedisResultList.size(); ++i)
	{
		fields.push_back(xRedisResultList[i].GetRespString());
	}

	return pRedisResult.get();

}

NFRedisResult *NFRedisClient::HLEN(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(HLEN));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HMGET(const std::string &key, const string_vector &fields, string_vector &values)
{
    NFRedisCommand cmd(GET_NAME(HMGET));
    cmd << key;
    for (int i = 0; i < fields.size(); ++i)
    {
        cmd << fields[i];
    }

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	const std::vector<NFRedisResult>& xRedisResultList = pRedisResult->GetRespArray();
	if (fields.size() == xRedisResultList.size())
	{
		values.clear();

		for (int i = 0; i < fields.size(); ++i)
		{
			values.push_back(xRedisResultList[i].GetRespString());
		}
	}

	return pRedisResult.get();

}

NFRedisResult *NFRedisClient::HMSET(const std::string &key, const std::vector<string_pair> &values)
{
    NFRedisCommand cmd(GET_NAME(HMSET));
    cmd << key;
    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HSET(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSET));
    cmd << key;
    cmd << field;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
    NFRedisCommand cmd(GET_NAME(HSETNX));
    cmd << key;
    cmd << field;
    cmd << value;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HVALS(const std::string &key, string_vector &values)
{
    NFRedisCommand cmd(GET_NAME(HVALS));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::HSTRLEN(const std::string &key, const std::string &field)
{

    NFRedisCommand cmd(GET_NAME(HSTRLEN));
    cmd << key;
    cmd << field;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}
