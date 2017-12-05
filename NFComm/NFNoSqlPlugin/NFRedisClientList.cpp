//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult *NFRedisClient::LINDEX(const std::string &key, const int index) 
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LINDEX");
	cmd << key;
	cmd << index;

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

NFRedisResult *NFRedisClient::LLEN(const std::string &key)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LLEN");
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

NFRedisResult *NFRedisClient::LPOP(const std::string &key)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LPOP");
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

NFRedisResult *NFRedisClient::LPUSH(const std::string &key, const std::string &value)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LPUSH");
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

NFRedisResult *NFRedisClient::LPUSHX(const std::string &key, const std::string &value) 
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LPUSHX");
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

NFRedisResult *NFRedisClient::LRANGE(const std::string &key, const int start, const int end, string_vector &values) 
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LRANGE");
	cmd << key;
	cmd << start;
	cmd << end;

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

NFRedisResult *NFRedisClient::LSET(const std::string &key, const int index, const std::string &value)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("LSET");
	cmd << key;
	cmd << index;
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

NFRedisResult *NFRedisClient::RPOP(const std::string &key)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("RPOP");
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

NFRedisResult *NFRedisClient::RPUSH(const std::string &key, const std::string &value) 
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("RPUSH");
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

NFRedisResult *NFRedisClient::RPUSHX(const std::string &key, const std::string &value)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd("RPUSHX");
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
