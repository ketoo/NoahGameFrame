//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisResult * NFRedisClient::ZADD(const std::string & key, const std::string & member, const double score)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZADD));
	cmd << key;
	cmd << member;
	cmd << score;

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

NFRedisResult * NFRedisClient::ZCARD(const std::string & key)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZCARD));
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

NFRedisResult * NFRedisClient::ZCOUNT(const std::string & key, const double start, const double end)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZCOUNT));
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

NFRedisResult * NFRedisClient::ZINCRBY(const std::string & key, const std::string & member, const double score)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << member;
	cmd << score;

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

NFRedisResult * NFRedisClient::ZRANGE(const std::string & key, const int start, const int end, string_vector& values)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZINCRBY));
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";
	
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

NFRedisResult * NFRedisClient::ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

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

NFRedisResult * NFRedisClient::ZRANK(const std::string & key, const std::string & member)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZRANK));
	cmd << key;
	cmd << member;

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

NFRedisResult * NFRedisClient::ZREM(const std::string & key, const std::string & member)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREM));
	cmd << key;
	cmd << member;

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

NFRedisResult * NFRedisClient::ZREMRANGEBYRANK(const std::string & key, const int start, const int end)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYRANK));
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

NFRedisResult * NFRedisClient::ZREMRANGEBYSCORE(const std::string & key, const double start, const double end)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREMRANGEBYSCORE));
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

NFRedisResult * NFRedisClient::ZREVRANGE(const std::string& key, const int start, const int end, string_vector& values)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREVRANGE));
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

NFRedisResult * NFRedisClient::ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREVRANGEBYSCORE));
	cmd << key;
	cmd << key;
	cmd << start;
	cmd << end;
	cmd << "WITHSCORES";

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

NFRedisResult * NFRedisClient::ZREVRANK(const std::string & key, const std::string & member)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZREVRANK));
	cmd << key;
	cmd << member;

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

NFRedisResult * NFRedisClient::ZSCORE(const std::string & key, const std::string & member)
{
	m_pRedisResult->Reset();

	NFRedisCommand cmd(GET_NAME(ZSCORE));
	cmd << key;
	cmd << member;

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
