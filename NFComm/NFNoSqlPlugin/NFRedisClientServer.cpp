//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

void NFRedisClient::FLUSHALL()
{
    NFRedisCommand cmd(GET_NAME(FLUSHALL));

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply != nullptr)
	{
		freeReplyObject(pReply);
	}
}

void NFRedisClient::FLUSHDB()
{
    NFRedisCommand cmd(GET_NAME(FLUSHDB));

	redisReply* pReply = BuildSendCmd(cmd);
	if (pReply != nullptr)
	{
		freeReplyObject(pReply);
	}
}
