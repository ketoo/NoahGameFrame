//
// Author: LUSHENG HUANG Created on 17/11/17.
//
#include "NFRedisClient.h"

NF_SHARE_PTR<NFRedisResult> NFRedisClient::DEL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(DEL));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult> NFRedisClient::EXISTS(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(EXISTS));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::EXPIRE(const std::string &key, const unsigned int secs)
{
    NFRedisCommand cmd(GET_NAME(EXPIRE));
    cmd << key;
    cmd << secs;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::EXPIREAT(const std::string &key, const int64_t unixTime)
{
    NFRedisCommand cmd(GET_NAME(EXPIREAT));
    cmd << key;
    cmd << unixTime;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);

	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::PERSIST(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(PERSIST));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::TTL(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TTL));
    cmd << key;

	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::TYPE(const std::string &key)
{
    NFRedisCommand cmd(GET_NAME(TYPE));
    cmd << key;


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}