//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
	mnIndex = 0;
	port_ = 0;
    m_pRedisClientSocket = new NFRedisClientSocket();
}

bool NFRedisClient::Enable()
{
	return true;
}

bool NFRedisClient::Busy()
{
	return true;
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
	std::string msg = cmd.Serialize();
	int nRet = m_pRedisClientSocket->Write(msg.data(), msg.length());
	if (nRet != 0)
	{
		//lost net
		//do some thing
		return nullptr;
	}

	mnIndex++;

	int64_t index = mnIndex;
	mnIndexList.push_back(index);

	struct redisReply* reply = nullptr;
	while (true)
	{
		//get reply
		int64_t nFrontIndex = mnIndexList.front();
		if (nFrontIndex == index)
		{
			int ret = redisReaderGetReply(m_pRedisClientSocket->GetRedisReader(), (void**)&reply);
			if (ret == REDIS_OK && reply != nullptr)
			{
				mnIndexList.pop_front();
				break;
			}
			else
			{
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