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
#include "NFNoSqlModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

#define CALL_REDIS(func, key, ...)\
	auto redis = FindRedisClientByKey(key);\
	if (redis)\
	{\
		bool ret = redis->func(key, ...);\
		if (ret)\
		{\
			return ret;\
		}\
		if (redis->LastReply() == nullptr)\
		{\
			return false;\
		}\
		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);\
		return false;\
	}


NFNoSqlModule::NFNoSqlModule(NFIPluginManager* p)
{
	executableModule = true;
	pPluginManager = p;
}

NFNoSqlModule::~NFNoSqlModule()
{

}


bool NFNoSqlModule::Init()
{
	mLastCheckTime = 0;

	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	return true;
}

bool NFNoSqlModule::AfterInit()
{
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

	for (int i = 0; i < 200; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		Execute();

		if (this->Enable())
		{
			break;
		}
	}

	return true;
}

bool NFNoSqlModule::Shut()
{

	return true;
}

bool NFNoSqlModule::Enable()
{
	return enable;
}

bool NFNoSqlModule::Busy()
{
	return false;
}

bool NFNoSqlModule::KeepLive()
{
	return false;
}

bool NFNoSqlModule::Execute()
{
	for (NF_SHARE_PTR<NFIRedisClient> redisClient : redisClients)
	{
		redisClient->Execute();
	}

	CheckConnect();
	
	return true;
}

void NFNoSqlModule::CheckConnect()
{
	static const int CHECK_TIME = 1;
	if (mLastCheckTime > 0)
	{
		if (mLastCheckTime + CHECK_TIME > NFGetTimeS())
		{
			return;
		}
	}

	mLastCheckTime = NFGetTimeS();
	{
		NF_SHARE_PTR<NFIRedisClient> queryMasterNode = nullptr;
		std::list<NF_SHARE_PTR<NFIRedisClient>> needToRemove;
		for (auto redisClient : redisClients)
		{
			if (redisClient->Enable())
			{
				if (redisClient->IsConnected())
				{
					if (redisClient->Authenticated())
					{
						if (!queryMasterNode)
						{
							queryMasterNode = redisClient;
						}
					}
					else
					{
						redisClient->AUTH(redisClient->GetAuthKey());
					}
				}
				else
				{
					//should be connecting
					m_pLogModule->LogInfo("connecting........");
				}
			}
			else
			{
				//remove[disconnect from server, will be disabled]
				needToRemove.push_back(redisClient);
			}
		}

		//remove
		for (auto r : needToRemove)
		{
			for (auto it = redisClients.begin(); it != redisClients.end(); )
			{
				if (it->get()->GetIP() == r->GetIP() && it->get()->GetPort() == r->GetPort())
				{
					it = redisClients.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		//试图连接所有的master nodes
		if (queryMasterNode)
		{
			ConnectAllMasterNodes(queryMasterNode);
		}
	}
}

bool NFNoSqlModule::Connect(const std::string &ip, const int port, const std::string &auth, const bool sync)
{
	NF_SHARE_PTR<NFIRedisClient> noSqlDriver(NF_NEW NFRedisClient());
	redisClients.push_back(noSqlDriver);

	m_pLogModule->LogInfo("NFNoSqlModule connect to:" + ip + std::to_string(port));

	return noSqlDriver->Connect(ip, port, auth, sync);
}

void NFNoSqlModule::ConnectAllMasterNodes(NF_SHARE_PTR<NFIRedisClient> redisClient)
{
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
		else if (vec.size() == 8)
		{
			std::cout << "vec.size() != 8, master node down:" <<  node << std::endl;
		}
		else
		{
			std::cout << "vec.size() != 8,9 " << std::endl;
		}
	}

	//connect to all master nodes
	for (auto& node : redisMasterNodes)
	{
		bool found = false;
		for (auto redisClient : redisClients)
		{
			//如果没有找到，则需要新加
			if (redisClient->GetIP() == node.host && redisClient->GetPort() == node.port)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			//connect to new nodes
			Connect(node.host, node.port, "");
		}
	}

	//update slot
	for (auto& node : redisMasterNodes)
	{
		bool found = false;
		for (auto redisClient : redisClients)
		{
			//如果没有找到，则需要新加
			if (redisClient->GetIP() == node.host && redisClient->GetPort() == node.port)
			{
				found = true;
				redisClient->UpdateSlot(node.slotStart, node.slotEnd);
				break;
			}
		}

		if (!found)
		{
			//error
			m_pLogModule->LogError(" failed to update slot for nodes:" + node.host + std::to_string(node.port));
		}
	}

	enable = true;
}

NF_SHARE_PTR<NFIRedisClient> NFNoSqlModule::FindRedisClient(int slot)
{
	for (auto redisClient : redisClients)
	{
		//如果没有找到，则需要新加
		if (slot >= redisClient->StartSlot() && slot <= redisClient->EndSlot())
		{
			return redisClient;
		}
	}

	return nullptr;
}

bool NFNoSqlModule::AUTH(const std::string &auth)
{
	return false;
}

bool NFNoSqlModule::INFO(std::string &info)
{
	return false;
}

bool NFNoSqlModule::DEL(const std::string &key)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->DEL(key);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::EXISTS(const std::string &key)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->EXISTS(key);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::EXPIRE(const std::string &key, const unsigned int secs)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->EXPIRE(key, secs);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::EXPIREAT(const std::string &key, const int64_t unixTime)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->EXPIREAT(key, unixTime);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::PERSIST(const std::string &key)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->PERSIST(key);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::TTL(const std::string &key, int& ttl)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->TTL(key, ttl);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::TYPE(const std::string &key, std::string& type)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->TYPE(key, type);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::APPEND(const std::string &key, const std::string &value, int &length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->APPEND(key, value, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::DECR(const std::string &key, int64_t &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->DECR(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::DECRBY(const std::string &key, const int64_t decrement, int64_t &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->DECRBY(key, decrement, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::GET(const std::string &key, std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->GET(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::GETSET(const std::string &key, const std::string &value, std::string &oldValue)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->GETSET(key, value, oldValue);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::INCR(const std::string &key, int64_t &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->INCR(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::INCRBY(const std::string &key, const int64_t increment, int64_t &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->INCRBY(key, increment, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::INCRBYFLOAT(const std::string &key, const float increment, float &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->INCRBYFLOAT(key, increment, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SET(const std::string &key, const std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SET(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SETEX(const std::string &key, const std::string &value, int time)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SETEX(key, value, time);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SETNX(const std::string &key, const std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SETNX(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::STRLEN(const std::string &key, int &length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->STRLEN(key, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HDEL(const std::string &key, const std::string &field)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HDEL(key, field);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HDEL(const std::string &key, const string_vector &fields)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HDEL(key, fields);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HEXISTS(const std::string &key, const std::string &field)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HEXISTS(key, field);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HGET(const std::string &key, const std::string &field, std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HGET(key, field, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HGETALL(const std::string &key, std::vector<string_pair> &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HGETALL(key, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HINCRBY(const std::string &key, const std::string &field, const int by, int64_t &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HINCRBY(key, field, by, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HINCRBYFLOAT(const std::string &key, const std::string &field, const float by, float &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HINCRBYFLOAT(key, field, by, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HKEYS(const std::string &key, std::vector<std::string> &fields)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HKEYS(key, fields);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HLEN(const std::string &key, int &number)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HLEN(key, number);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HMGET(const std::string &key, const string_vector &fields, string_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HMGET(key, fields, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HMSET(const std::string &key, const std::vector<string_pair> &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HMSET(key, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HMSET(const std::string &key, const string_vector &fields, const string_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HMSET(key, fields, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HSET(const std::string &key, const std::string &field, const std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HSET(key, field, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HSETNX(const std::string &key, const std::string &field, const std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HSETNX(key, field, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HVALS(const std::string &key, string_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HVALS(key, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::HSTRLEN(const std::string &key, const std::string &field, int &length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->HSTRLEN(key, field, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LINDEX(const std::string &key, const int index, std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LINDEX(key, index, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LLEN(const std::string &key, int &length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LLEN(key, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LPOP(const std::string &key, std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LPOP(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LPUSH(const std::string &key, const std::string &value, int& length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LPUSH(key, value, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LPUSHX(const std::string &key, const std::string &value, int& length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LPUSHX(key, value, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LRANGE(const std::string &key, const int start, const int end, string_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LRANGE(key, start, end, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::LSET(const std::string &key, const int index, const std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->LSET(key, index, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::RPOP(const std::string &key, std::string &value)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->RPOP(key, value);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::RPUSH(const std::string &key, const std::string &value, int& length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->RPUSH(key, value, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::RPUSHX(const std::string &key, const std::string &value, int& length)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->RPUSHX(key, value, length);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SADD(const std::string &key, const std::string &member, int& count)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SADD(key, member, count);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SCARD(const std::string &key, int &count)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SCARD(key, count);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}
/*
bool NFNoSqlModule::SDIFF(const std::string &key_1, const std::string &key_2, string_vector &output)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SDIFF(key, member, count);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}
*/
bool NFNoSqlModule::SDIFFSTORE(const std::string &store_key, const std::string &diff_key1, const std::string &diff_key2, int& num)
{
	auto redis = FindRedisClientByKey(store_key);
	if (redis)
	{
		bool ret = redis->SDIFFSTORE(store_key, diff_key1, diff_key2, num);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}
/*
bool NFNoSqlModule::SINTER(const std::string &key_1, const std::string &key_2, string_vector &output)
{

}

bool NFNoSqlModule::SINTERSTORE(const std::string &inter_store_key, const std::string &inter_key1,
                               const std::string &inter_key2)
{
	return false;
}
*/
bool NFNoSqlModule::SISMEMBER(const std::string &key, const std::string &member)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SISMEMBER(key, member);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SMEMBERS(const std::string &key, string_vector &output)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SMEMBERS(key, output);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}
/*
bool NFNoSqlModule::SMOVE(const std::string &source_key, const std::string &dest_key, const std::string &member)
{

	return false;
}
*/

bool NFNoSqlModule::SPOP(const std::string &key, std::string &output)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SPOP(key, output);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SRANDMEMBER(const std::string &key, int count, string_vector &output)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SRANDMEMBER(key, count, output);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::SREM(const std::string &key, const string_vector &members, int& number)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->SREM(key, members, number);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}
/*
bool NFNoSqlModule::SUNION(const std::string &union_key1, const std::string &union_key2, string_vector &output)
{

	return false;
}
*/
bool NFNoSqlModule::SUNIONSTORE(const std::string &dest_store_key, const std::string &union_key1,
                               const std::string &union_key2)
{
	auto redis = FindRedisClientByKey(dest_store_key);
	if (redis)
	{
		bool ret = redis->SUNIONSTORE(dest_store_key, union_key1, union_key2);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZADD(const std::string &key, const std::string &member, const double score, int& number)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZADD(key, member, score, number);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZCARD(const std::string &key, int &count)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZCARD(key, count);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZCOUNT(const std::string &key, const double start, const double end, int &count)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZCOUNT(key, start, end, count);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZINCRBY(const std::string &key, const std::string &member, const double score, double &newScore)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZINCRBY(key, member, score, newScore);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZRANGE(const std::string &key, const int start, const int end, string_score_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZRANGE(key, start, end, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZRANGEBYSCORE(const std::string &key, const double start, const double end, string_score_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZRANGEBYSCORE(key, start, end, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZRANK(const std::string &key, const std::string &member, int &rank)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZRANK(key, member, rank);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREM(const std::string &key, const std::string &member)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREM(key, member);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREMRANGEBYRANK(const std::string &key, const int start, const int end)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREMRANGEBYRANK(key, start, end);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREMRANGEBYSCORE(const std::string &key, const double min, const double max)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREMRANGEBYRANK(key, min, max);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREVRANGE(const std::string &key, const int start, const int end, string_score_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREVRANGE(key, start, end, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREVRANGEBYSCORE(const std::string &key, const double start, const double end,
                                     string_score_vector &values)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREVRANGEBYSCORE(key, start, end, values);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZREVRANK(const std::string &key, const std::string &member, int &rank)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZREVRANK(key, member, rank);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

bool NFNoSqlModule::ZSCORE(const std::string &key, const std::string &member, double &score)
{
	auto redis = FindRedisClientByKey(key);
	if (redis)
	{
		bool ret = redis->ZSCORE(key, member, score);
		if (ret)
		{
			return ret;
		}

		if (redis->LastReply() == nullptr)
		{
			//crash
			return false;
		}

		m_pLogModule->LogError(redis->LastReply()->error, __FUNCTION__, __LINE__);
		//move????

		return false;
	}

	return false;
}

void NFNoSqlModule::FLUSHALL()
{
	for (NF_SHARE_PTR<NFIRedisClient> redisClient : redisClients)
	{
		redisClient->FLUSHALL();
	}
}

void NFNoSqlModule::FLUSHDB()
{
	for (NF_SHARE_PTR<NFIRedisClient> redisClient : redisClients)
	{
		redisClient->FLUSHDB();
	}
}

bool NFNoSqlModule::CLUSTERNODES(std::vector<std::string> &clusters, bool onlyMasterNode, bool includeSelfMaster)
{
	return false;
}

bool NFNoSqlModule::CLUSTERINFO(std::string &clusterInfo)
{
	return false;
}
