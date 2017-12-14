//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult *NFRedisClient::FLUSHALL()
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(FLUSHALL));

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        return m_pRedisResult;
    }

    GetStatusReply();


    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::FLUSHDB()
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(FLUSHDB));

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        return m_pRedisResult;
    }

    GetStatusReply();


    return m_pRedisResult;
}
