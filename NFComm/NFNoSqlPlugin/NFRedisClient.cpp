//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
	mnPort = 0;
	bBusy = false;
    m_pRedisClientSocket = new NFRedisClientSocket();
}

bool NFRedisClient::Enable()
{
	return m_pRedisClientSocket->IsConnect();
}

bool NFRedisClient::Busy()
{
	return bBusy;
}

bool NFRedisClient::Connect(const std::string &ip, const int port, const std::string &auth)
{
    int64_t nFD = m_pRedisClientSocket->Connect(ip, port);
	if (nFD > 0)
	{
		mstrIP = ip;
		mnPort = port;
		mstrAuthKey = auth;
		return true;
	}

    return false;
}

bool NFRedisClient::SelectDB(int dbnum)
{
    return false;
}

bool NFRedisClient::KeepLive()
{
    return false;
}

const std::string& NFRedisClient::GetIP()
{
	return mstrIP;
}

const int NFRedisClient::GetPort()
{
	return mnPort;
}

const std::string& NFRedisClient::GetAuthKey()
{
	return mstrAuthKey;
}

bool NFRedisClient::IsConnect()
{
	if (m_pRedisClientSocket)
	{
		return m_pRedisClientSocket->IsConnect();
	}

	return false;
}

bool NFRedisClient::Execute()
{
    m_pRedisClientSocket->Execute();

    return false;
}

NF_SHARE_PTR<redisReply> NFRedisClient::BuildSendCmd(const NFRedisCommand& cmd)
{
	while (bBusy)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		//you can not use build send cmd funciton again if you are not using coroutine
		YieldFunction();
	}

	std::string msg = cmd.Serialize();
	int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
	if (nRet != 0)
	{
		//lost net
		//do some thing
		return nullptr;
	}
	
	bBusy = true;

	return ParseForReply();
}

NF_SHARE_PTR<redisReply> NFRedisClient::ParseForReply()
{
	struct redisReply* reply = nullptr;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		// When the buffer is empty, reply will be null
		int ret = redisReaderGetReply(m_pRedisClientSocket->GetRedisReader(), (void**)&reply);
		if (ret == REDIS_OK && reply != nullptr)
		{
			break;
		}

		if (YieldFunction)
		{
			YieldFunction();
		}
		else
		{
			Execute();
		}
	}

	bBusy = false;

	if (reply == nullptr)
	{
		return nullptr;
	}

	if (REDIS_REPLY_ERROR == reply->type)
	{
		// write log
		freeReplyObject(reply);
		return nullptr;
	}

	return NF_SHARE_PTR<redisReply>(reply, [](redisReply* r) { if (r) freeReplyObject(r); });
}

bool NFRedisClient::AUTH(const std::string& auth)
{
	NFRedisCommand cmd(GET_NAME(AUTH));
	cmd << auth;

	// if password error, redis will return REDIS_REPLY_ERROR
	// pReply will be null
	NF_SHARE_PTR<redisReply> pReply = BuildSendCmd(cmd);
	if (pReply == nullptr)
	{
		return false;
	}

	if (pReply->type == REDIS_REPLY_STATUS)
	{
		if (std::string("OK") == std::string(pReply->str, pReply->len) ||
			std::string("ok") == std::string(pReply->str, pReply->len))
		{
			return true;
		}
	}
	
	return false;
}