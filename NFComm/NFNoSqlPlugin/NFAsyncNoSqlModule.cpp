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

bool NFAsyncNoSqlModule::Connect(const std::string &ip, const int port, const std::string &auth, const bool sync)
{
	NF_SHARE_PTR<NFIRedisClient> nosqlDriver(NF_NEW NFRedisClient());
	NF_SHARE_PTR<RedisClientThread> redisClientThread(NF_NEW RedisClientThread(nosqlDriver, &trackerQueue));

	nosqlDriver->Connect(ip, port, auth, sync);

	redisClients.push_back(redisClientThread);

	return true;
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
				//r->UpdateSlot(node.slotStart, node.slotEnd);
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
