//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"

NFRedisClient::NFRedisClient()
{
	mnIndex = 0;
    m_pRedisClientSocket = new NFRedisClientSocket();
}


bool NFRedisClient::ConnectTo(const std::string &ip, const int port, const std::string &auth)
{
    int64_t nFD = m_pRedisClientSocket->Connect(ip, port);
	if (nFD > 0)
	{
		if (!auth.empty())
		{
			return AUTH(auth);
		}

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

	void* reply = nullptr;
	while (true)
	{
		//get reply
		const int nFrontIndex = mnIndexList.front();
		if (nFrontIndex == index)
		{
			int ret = redisReaderGetReply(m_pRedisClientSocket->GetRedisReader(), &reply);
			if (ret == REDIS_OK)
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
		printf("redisReply return REDIS_REPLY_ERROR!\n");
		freeReplyObject(reply);
		//redisReaderFree(reader);
		//reader = NULL;
		return nullptr;
	}

	if (REDIS_REPLY_STATUS != r->type)
	{
		printf("redisReply return is't REDIS_REPLY_STATUS!\n");
		freeReplyObject(reply);
		//redisReaderFree(reader);
		//reader = NULL;
		return nullptr;
	}

	return nullptr;
}

bool NFRedisClient::AUTH(const std::string & auth)
{
	NFRedisCommand cmd(GET_NAME(AUTH));
	cmd << auth;

	redisReply* reply = BuildSendCmd(cmd);
	

	return false;
}