//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"



NFRedisResult *NFRedisClient::APPEND(const std::string &key, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("APPEND");
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

    NFRedisCommand cmd("DECR");
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

    NFRedisCommand cmd("DECRBY");
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

    NFRedisCommand cmd("GETSET");
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

    NFRedisCommand cmd("INCR");
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

    NFRedisCommand cmd("INCRBY");
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

    NFRedisCommand cmd("INCRBYFLOAT");
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

    NFRedisCommand cmd("MGET");

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

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::MSET(const string_pair_vector &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("MSET");

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

    NFRedisCommand cmd("SETEX");
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

    NFRedisCommand cmd("SETNX");
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

    NFRedisCommand cmd("STRLEN");
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

    NFRedisCommand cmd("SET");
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

    NFRedisCommand cmd("GET");
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
