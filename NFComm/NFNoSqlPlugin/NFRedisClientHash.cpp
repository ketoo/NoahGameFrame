//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult *NFRedisClient::HDEL(const std::string &key, const std::string &field)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HDEL");
    cmd << key;
    cmd << field;

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

NFRedisResult *NFRedisClient::HEXISTS(const std::string &key, const std::string &field)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HEXISTS");
    cmd << key;
    cmd << field;

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

NFRedisResult *NFRedisClient::HGET(const std::string &key, const std::string &field)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HGET");
    cmd << key;
    cmd << field;

    std::string msg = cmd.Serialize();

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HGETALL");
    cmd << key;

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

NFRedisResult *NFRedisClient::HINCRBY(const std::string &key, const std::string &field, const int by)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HINCRBY");
    cmd << key;
    cmd << field;
    cmd << by;

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

NFRedisResult *NFRedisClient::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HINCRBYFLOAT");
    cmd << key;
    cmd << field;
    cmd << by;

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

NFRedisResult *NFRedisClient::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HKEYS");
    cmd << key;

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

NFRedisResult *NFRedisClient::HLEN(const std::string &key)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HLEN");
    cmd << key;

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

NFRedisResult *NFRedisClient::HMGET(const std::string &key, const string_vector &fields, string_vector &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HMGET");
    cmd << key;
    for (int i = 0; i < fields.size(); ++i)
    {
        cmd << fields[i];
    }

    std::string msg = cmd.Serialize();
	m_pRedisResult->SetCommand(msg);

    int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
    if (nRet != 0)
    {
        return m_pRedisResult;
    }

    GetBulkReply();

	const std::vector<NFRedisResult>& xRedisResultList = m_pRedisResult->GetRespArray();
	if (fields.size() == xRedisResultList.size())
	{
		values.clear();

		for (int i = 0; i < fields.size(); ++i)
		{
			values.push_back(xRedisResultList[i].GetRespString());
		}
	}

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::HMSET(const std::string &key, const std::vector<string_pair> &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HMSET");
    cmd << key;
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
        return m_pRedisResult;
    }

    GetStatusReply();

    return m_pRedisResult;
}

NFRedisResult *NFRedisClient::HSET(const std::string &key, const std::string &field, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HSET");
    cmd << key;
    cmd << field;
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

NFRedisResult *NFRedisClient::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HSETNX");
    cmd << key;
    cmd << field;
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

NFRedisResult *NFRedisClient::HVALS(const std::string &key, string_vector &values)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HVALS");
    cmd << key;

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

NFRedisResult *NFRedisClient::HSTRLEN(const std::string &key, const std::string &field)
{
    m_pRedisResult->Reset();

    NFRedisCommand cmd("HSTRLEN");
    cmd << key;
    cmd << field;

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
