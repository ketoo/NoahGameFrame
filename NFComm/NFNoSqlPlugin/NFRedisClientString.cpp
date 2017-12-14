//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"



NFRedisResult *NFRedisClient::APPEND(const std::string &key, const std::string &value)
{
    m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(APPEND));
    cmd << key;
    cmd << value;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetIntReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::DECR(const std::string &key)
{
    m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(DECR));
    cmd << key;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::DECRBY(const std::string &key, const int64_t v)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(DECRBY));
    cmd << key;
    cmd << v;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::GETSET(const std::string &key, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(GETSET));
    cmd << key;
    cmd << value;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::INCR(const std::string &key)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(INCR));
    cmd << key;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::INCRBY(const std::string &key, const int64_t v)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(INCRBY));
    cmd << key;
    cmd << v;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::INCRBYFLOAT(const std::string &key, const float fv)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(INCRBYFLOAT));
    cmd << key;
    cmd << fv;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::MGET(const string_vector &keys, string_vector &values)
{
    m_pRedisResult->Reset();
	
	NFRedisCommand cmd(GET_NAME(MGET));

    for (int i = 0; i < keys.size(); ++i)
    {
        cmd << keys[i];
    }

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

	GetArrayReply();

	const std::vector<NFRedisResult> xVector = m_pRedisResult->GetRespArray();
	if (keys.size() == xVector.size())
	{
		for (int i = 0; i < xVector.size(); ++i)
		{
			values.push_back(xVector[i].GetRespString());
		}
	}

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::MSET(const string_pair_vector &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(MSET));

    for (int i = 0; i < values.size(); ++i)
    {
        cmd << values[i].first;
        cmd << values[i].second;
    }

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::SETEX(const std::string &key, const std::string &value, int time)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(SETEX));
    cmd << key;
    cmd << value;
    cmd << time;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::SETNX(const std::string &key, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(SETNX));
    cmd << key;
    cmd << value;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::STRLEN(const std::string &key)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(STRLEN));
    cmd << key;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}


NFRedisResult* NFRedisClient::SET(const std::string &key, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(SET));
    cmd << key;
    cmd << value;

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

NFRedisResult* NFRedisClient::GET(const std::string &key)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd(GET_NAME(GET));
    cmd << key;

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        //mxRedisResult
        return m_pRedisResult;
    }

    GetBulkReply();

    return m_pRedisResult;
}
