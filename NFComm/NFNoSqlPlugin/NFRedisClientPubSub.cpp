#include "NFRedisClient.h"

bool NFRedisClient::PUBLISH(const std::string& key, const std::string& value)
{
	NFRedisCommand cmd(GET_NAME(PUBLISH));
	cmd << key;
	cmd << value;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true; 
}

bool NFRedisClient::SUBSCRIBE(const std::string& key)
{
	NFRedisCommand cmd(GET_NAME(SUBSCRIBE));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}

bool NFRedisClient::UNSUBSCRIBE(const std::string& key)
{
	NFRedisCommand cmd(GET_NAME(UNSUBSCRIBE));
	cmd << key;

	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	return true;
}
