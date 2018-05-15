//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
	port_ = 0;
	bBusy = false;
    m_pRedisClientSocket = new NFRedisClientSocket();
}

bool NFRedisClient::Enable()
{
	return true;
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
		if (!auth.empty())
		{
			return AUTH(auth);
		}

		ip_ = ip;
		port_ = port;
		authKey_ = auth;
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
	return ip_;
}

const int NFRedisClient::GetPort()
{
	return port_;
}

const std::string& NFRedisClient::GetAuthKey()
{
	return authKey_;
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

redisReply* NFRedisClient::BuildSendCmd(const NFRedisCommand& cmd)
{
	if (bBusy)
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (YieldFunction)
			{
				YieldFunction();
			}
			else
			{
				Execute();
			}
		}
	}

	std::string msg = cmd.Serialize();
	int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
	if (nRet != 0)
	{
		//lost net
		//do some thing
		return nullptr;
	}

	struct redisReply* reply = nullptr;
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

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

	if (reply == nullptr)
	{
		return nullptr;
	}

	struct redisReply* r = (struct redisReply*)reply;
	if (REDIS_REPLY_ERROR == r->type)
	{
		// write log
		freeReplyObject(reply);
		return nullptr;
	}

	return reply;
}

bool NFRedisClient::AUTH(const std::string & auth)
{
	NFRedisCommand cmd(GET_NAME(AUTH));
	cmd << auth;

	redisReply* reply = BuildSendCmd(cmd);
	

	return false;
}