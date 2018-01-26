//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NF_SHARE_PTR<NFRedisResult>NFRedisClient::FLUSHALL()
{
    NFRedisCommand cmd(GET_NAME(FLUSHALL));


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}

NF_SHARE_PTR<NFRedisResult>NFRedisClient::FLUSHDB()
{
    NFRedisCommand cmd(GET_NAME(FLUSHDB));


	NF_SHARE_PTR<NFRedisResult> pRedisResult = BuildSendCmd(cmd);

	WaitingResult(pRedisResult);


	return pRedisResult;
}
