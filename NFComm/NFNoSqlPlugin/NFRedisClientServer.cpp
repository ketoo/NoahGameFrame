//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult *NFRedisClient::FLUSHALL()
{
    NFRedisCommand cmd(GET_NAME(FLUSHALL));


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}

NFRedisResult *NFRedisClient::FLUSHDB()
{
    NFRedisCommand cmd(GET_NAME(FLUSHDB));


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult.get();
}
