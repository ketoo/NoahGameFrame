/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <algorithm>
#include "NFAsyncNoSqlModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFAsyncNoSqlModule::NFAsyncNoSqlModule(NFIPluginManager* p)
{
	executableModule = true;
	pPluginManager = p;
}

NFAsyncNoSqlModule::~NFAsyncNoSqlModule()
{

}


bool NFAsyncNoSqlModule::Init()
{
	mLastCheckTime = 0;
	return true;
}

bool NFAsyncNoSqlModule::Shut()
{

	return true;
}

bool NFAsyncNoSqlModule::AfterInit()
{
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::NoSqlServer::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::ServerID());
			const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::NoSqlServer::Port());
			const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
			const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());

			if (this->Connect("127.0.0.1", 30001, "", true))
			//if (this->AddConnectSql(strId, ip, nPort, strAuth))
			{
				std::ostringstream strLog;
				strLog << "Connected NoSqlServer[" << ip << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << ip << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogInfo(strLog, __FUNCTION__, __LINE__);
			}
		}
	}

	return true;
}

bool NFAsyncNoSqlModule::Enable()
{
	return false;
}

bool NFAsyncNoSqlModule::Busy()
{
	return false;
}

bool NFAsyncNoSqlModule::KeepLive()
{
	return false;
}

bool NFAsyncNoSqlModule::Execute()
{
	CheckConnect();
	
	return true;
}

void NFAsyncNoSqlModule::CheckConnect()
{
	static const int CHECK_TIME = 1;
	if (mLastCheckTime + CHECK_TIME > pPluginManager->GetNowTime())
	{
		return;
	}

	mLastCheckTime = pPluginManager->GetNowTime();
	{
		NF_SHARE_PTR<NFIRedisClient> queryMasterNode = nullptr;
		std::list<NF_SHARE_PTR<RedisClientThread>> needToRemove;
		for (auto redisClient : redisClients)
		{
			if (redisClient->redisClient->Enable())
			{
				if (redisClient->redisClient->IsConnected())
				{
					if (redisClient->redisClient->Authenticated())
					{
						if (!queryMasterNode)
						{
							queryMasterNode = redisClient->redisClient;
						}
					}
					else
					{
						redisClient->redisClient->AUTH(redisClient->redisClient->GetAuthKey());
					}
				}
				else
				{
					//should be connecting
					m_pLogModule->LogInfo("connecting");
				}
			}
			else
			{
				//remove[disconnect from server, will be disabled]
				needToRemove.push_back(redisClient);
			}
		}

		//remove
		for (auto redisClient : needToRemove)
		{

		}

		//试图连接所有的master nodes
		if (queryMasterNode)
		{
			ConnectAllMasterNodes(queryMasterNode);
		}
	}
}

NF_SHARE_PTR<NFIRedisClient> NFAsyncNoSqlModule::Connect(const std::string &ip, const int port, const std::string &auth, const bool sync)
{
	NF_SHARE_PTR<NFIRedisClient> nosqlDriver(NF_NEW NFRedisClient());
	NF_SHARE_PTR<RedisClientThread> redisClientThread(NF_NEW RedisClientThread(nosqlDriver, &trackerQueue));

	nosqlDriver->Connect(ip, port, auth, sync);

	redisClients.push_back(redisClientThread);

	return nosqlDriver;
}

void NFAsyncNoSqlModule::ConnectAllMasterNodes(NF_SHARE_PTR<NFIRedisClient> redisClient)
{
	//async

	std::list<RedisMasterNode> redisMasterNodes;
	std::vector<std::string> masterNodes;
	redisClient->CLUSTERNODES(masterNodes, true, true);

	for (auto& node : masterNodes)
	{
		//masterNodes
		std::vector<std::string> vec;
		StringToVector(node, " ", vec);

		if (vec.size() == 9)
		{
			std::string host;
			std::string port;
			std::string slotStart;
			std::string slotEnd;

			std::string address = vec[1];
			std::string slot = vec[8];
			int pos1 = address.find(":");
			int pos2 = address.find("@");

			if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
			{
				host = address.substr(0, pos1);
				port = address.substr(pos1+1, pos2-pos1-1);
			}

			int pos3 = slot.find("-");
			if (pos3 != std::string::npos)
			{
				slotStart = slot.substr(0, pos3);
				slotEnd = slot.substr(pos3+1, slot.length()-pos3);
			}

			if (!host.empty() && !port.empty() && !slotStart.empty() && !slotEnd.empty())
			{
				try
				{
					RedisMasterNode redisMasterNode;
					redisMasterNode.host = host;
					redisMasterNode.port = std::atoi(port.c_str());
					redisMasterNode.slotStart = std::atoi(slotStart.c_str());
					redisMasterNode.slotEnd = std::atoi(slotEnd.c_str());

					redisMasterNodes.push_back(redisMasterNode);
				}
				catch (...)
				{
					std::cout << "std::atoi(port.c_str())" << std::endl;
				}
			}
			else
			{
				std::cout << "!host.empty() && !port.empty() && !slotStart.empty() && !slotEnd.empty()" << std::endl;
			}
		}
		else
		{
			std::cout << "vec.size() != 9" << std::endl;
		}
	}

	for (auto& node : redisMasterNodes)
	{
		bool found = false;
		for (auto redisClient : redisClients)
		{
			//如果没有找到，则需要新加
			if (redisClient->redisClient->GetIP() == node.host && redisClient->redisClient->GetPort() == node.port)
			{
				found = true;
				//update slot
				redisClient->redisClient->UpdateSlot(node.slotStart, node.slotEnd);
			}
		}

		if (!found)
		{
			//connect to new nodes
			auto r = Connect(node.host, node.port, "");
			if (r)
			{
				r->UpdateSlot(node.slotStart, node.slotEnd);
			}
		}
	}
}

NF_SHARE_PTR<NFIRedisClient> NFAsyncNoSqlModule::FindRedisClient(int slot)
{
	for (auto redisClient : redisClients)
	{
		//如果没有找到，则需要新加
		if (slot >= redisClient->redisClient->StartSlot() && slot <= redisClient->redisClient->EndSlot())
		{
			return redisClient->redisClient;
		}
	}

	return nullptr;
}

bool NFAsyncNoSqlModule::AUTH(const std::string &auth)
{
	return false;
}

bool NFAsyncNoSqlModule::INFO(std::string &info)
{
	return false;
}

bool NFAsyncNoSqlModule::DEL(const std::string &key)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		NFRedisCommand cmd(GET_NAME(DEL));
		cmd << key;

		NF_SHARE_PTR<NFRedisReply> pReply = redis->BuildSendCmd(cmd);
		if (pReply->reply == nullptr)
		{
			return false;
		}

		bool del_key_num = false;
		if (pReply->reply->type == REDIS_REPLY_INTEGER)
		{
			del_key_num = (bool)pReply->reply->integer;
		}

		return del_key_num;
	}

	return false;
}

bool NFAsyncNoSqlModule::EXISTS(const std::string &key)
{
	return false;
}

bool NFAsyncNoSqlModule::EXPIRE(const std::string &key, const unsigned int secs)
{
	return false;
}

bool NFAsyncNoSqlModule::EXPIREAT(const std::string &key, const int64_t unixTime)
{
	return false;
}

bool NFAsyncNoSqlModule::PERSIST(const std::string &key)
{
	return false;
}

int NFAsyncNoSqlModule::TTL(const std::string &key)
{
	return 0;
}

std::string NFAsyncNoSqlModule::TYPE(const std::string &key)
{
	return std::string();
}

bool NFAsyncNoSqlModule::APPEND(const std::string &key, const std::string &value, int &length)
{
	return false;
}

bool NFAsyncNoSqlModule::DECR(const std::string &key, int64_t &value)
{
	return false;
}

bool NFAsyncNoSqlModule::DECRBY(const std::string &key, const int64_t decrement, int64_t &value)
{
	return false;
}

bool NFAsyncNoSqlModule::GET(const std::string &key, std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::GETSET(const std::string &key, const std::string &value, std::string &oldValue)
{
	return false;
}

bool NFAsyncNoSqlModule::INCR(const std::string &key, int64_t &value)
{
	return false;
}

bool NFAsyncNoSqlModule::INCRBY(const std::string &key, const int64_t increment, int64_t &value)
{
	return false;
}

bool NFAsyncNoSqlModule::INCRBYFLOAT(const std::string &key, const float increment, float &value)
{
	return false;
}

bool NFAsyncNoSqlModule::MGET(const string_vector &keys, string_vector &values)
{
	return false;
}

void NFAsyncNoSqlModule::MSET(const string_pair_vector &values)
{


}

bool NFAsyncNoSqlModule::SET(const std::string &key, const std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::SETEX(const std::string &key, const std::string &value, int time)
{
	return false;
}

bool NFAsyncNoSqlModule::SETNX(const std::string &key, const std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::STRLEN(const std::string &key, int &length)
{
	return false;
}

int NFAsyncNoSqlModule::HDEL(const std::string &key, const std::string &field)
{
	return 0;
}

int NFAsyncNoSqlModule::HDEL(const std::string &key, const string_vector &fields)
{
	return 0;
}

bool NFAsyncNoSqlModule::HEXISTS(const std::string &key, const std::string &field)
{
	return false;
}

bool NFAsyncNoSqlModule::HGET(const std::string &key, const std::string &field, std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
	return false;
}

bool NFAsyncNoSqlModule::HINCRBY(const std::string &key, const std::string &field, const int by, int64_t &value)
{
	return false;
}

bool NFAsyncNoSqlModule::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by, float &value)
{
	return false;
}

bool NFAsyncNoSqlModule::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
	return false;
}

bool NFAsyncNoSqlModule::HLEN(const std::string &key, int &number)
{
	return false;
}

bool NFAsyncNoSqlModule::HMGET(const std::string &key, const string_vector &fields, string_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::HMSET(const std::string &key, const std::vector<string_pair> &values)
{
	return false;
}

bool NFAsyncNoSqlModule::HMSET(const std::string &key, const string_vector &fields, const string_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::HSET(const std::string &key, const std::string &field, const std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::HVALS(const std::string &key, string_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::HSTRLEN(const std::string &key, const std::string &field, int &length)
{
	return false;
}

bool NFAsyncNoSqlModule::LINDEX(const std::string &key, const int index, std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::LLEN(const std::string &key, int &length)
{
	return false;
}

bool NFAsyncNoSqlModule::LPOP(const std::string &key, std::string &value)
{
	return false;
}

int NFAsyncNoSqlModule::LPUSH(const std::string &key, const std::string &value)
{
	return 0;
}

int NFAsyncNoSqlModule::LPUSHX(const std::string &key, const std::string &value)
{
	return 0;
}

bool NFAsyncNoSqlModule::LRANGE(const std::string &key, const int start, const int end, string_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::LSET(const std::string &key, const int index, const std::string &value)
{
	return false;
}

bool NFAsyncNoSqlModule::RPOP(const std::string &key, std::string &value)
{
	return false;
}

int NFAsyncNoSqlModule::RPUSH(const std::string &key, const std::string &value)
{
	return 0;
}

int NFAsyncNoSqlModule::RPUSHX(const std::string &key, const std::string &value)
{
	return 0;
}

int NFAsyncNoSqlModule::SADD(const std::string &key, const std::string &member)
{
	return 0;
}

bool NFAsyncNoSqlModule::SCARD(const std::string &key, int &count)
{
	return false;
}

bool NFAsyncNoSqlModule::SDIFF(const std::string &key_1, const std::string &key_2, string_vector &output)
{
	return false;
}

int NFAsyncNoSqlModule::SDIFFSTORE(const std::string &store_key, const std::string &diff_key1, const std::string &diff_key2)
{
	return 0;
}

bool NFAsyncNoSqlModule::SINTER(const std::string &key_1, const std::string &key_2, string_vector &output)
{
	return false;
}

int NFAsyncNoSqlModule::SINTERSTORE(const std::string &inter_store_key, const std::string &inter_key1,
                               const std::string &inter_key2)
{
	return 0;
}

bool NFAsyncNoSqlModule::SISMEMBER(const std::string &key, const std::string &member)
{
	return false;
}

bool NFAsyncNoSqlModule::SMEMBERS(const std::string &key, string_vector &output)
{
	return false;
}

bool NFAsyncNoSqlModule::SMOVE(const std::string &source_key, const std::string &dest_key, const std::string &member)
{
	return false;
}

bool NFAsyncNoSqlModule::SPOP(const std::string &key, std::string &output)
{
	return false;
}

bool NFAsyncNoSqlModule::SRANDMEMBER(const std::string &key, int count, string_vector &output)
{
	return false;
}

int NFAsyncNoSqlModule::SREM(const std::string &key, const string_vector &members)
{
	return 0;
}

bool NFAsyncNoSqlModule::SUNION(const std::string &union_key1, const std::string &union_key2, string_vector &output)
{
	return false;
}

int NFAsyncNoSqlModule::SUNIONSTORE(const std::string &dest_store_key, const std::string &union_key1,
                               const std::string &union_key2)
{
	return 0;
}

int NFAsyncNoSqlModule::ZADD(const std::string &key, const std::string &member, const double score)
{
	return 0;
}

bool NFAsyncNoSqlModule::ZCARD(const std::string &key, int &count)
{
	return false;
}

bool NFAsyncNoSqlModule::ZCOUNT(const std::string &key, const double start, const double end, int &count)
{
	return false;
}

bool NFAsyncNoSqlModule::ZINCRBY(const std::string &key, const std::string &member, const double score, double &newScore)
{
	return false;
}

bool NFAsyncNoSqlModule::ZRANGE(const std::string &key, const int start, const int end, string_score_vector &values)
{
	return false;
}

bool
NFAsyncNoSqlModule::ZRANGEBYSCORE(const std::string &key, const double start, const double end, string_score_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::ZRANK(const std::string &key, const std::string &member, int &rank)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREM(const std::string &key, const std::string &member)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREMRANGEBYRANK(const std::string &key, const int start, const int end)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREMRANGEBYSCORE(const std::string &key, const double min, const double max)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREVRANGE(const std::string &key, const int start, const int end, string_score_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREVRANGEBYSCORE(const std::string &key, const double start, const double end,
                                     string_score_vector &values)
{
	return false;
}

bool NFAsyncNoSqlModule::ZREVRANK(const std::string &key, const std::string &member, int &rank)
{
	return false;
}

bool NFAsyncNoSqlModule::ZSCORE(const std::string &key, const std::string &member, double &score)
{
	return false;
}

void NFAsyncNoSqlModule::FLUSHALL()
{

}

void NFAsyncNoSqlModule::FLUSHDB()
{

}

bool NFAsyncNoSqlModule::CLUSTERNODES(std::vector<std::string> &clusters, bool onlyMasterNode, bool includeSelfMaster)
{
	return false;
}

bool NFAsyncNoSqlModule::CLUSTERINFO(std::string &clusterInfo)
{
	return false;
}
