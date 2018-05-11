#include "NFRedisCluster.h"
#include <string.h>


uint16_t NFCRedisCluster::crc16(const char *buf, size_t len)
{
	uint16_t crc = 0;
	for (size_t counter = 0; counter < len; counter++)
	{
		crc = (crc << 8) ^ crc16tab[((crc >> 8) ^ *buf++) & 0x00FF];
	}

	return crc;
}

bool NFCRedisCluster::getClusterSlot(RedisClientPtr rs)
{
	if (nullptr == rs)
	{
		//ERRORS("getClusterSlot nullptr == rs");
		return false;
	}

	{
		const char cmd[] = "info server";

		RedisReplyPtr _reply = rs->exeCmd(cmd);
		if (nullptr == _reply)
		{
			//ERRORS("getMode error!");
			return false;
		}

		if (_reply->type == REDIS_REPLY_STRING)
		{
			char *ptr = std::strstr(_reply->str, "redis_mode:standalone");
			if (ptr)
			{
				_mode = enum_RedisModeAlone;
				return true;
			}

			ptr = std::strstr(_reply->str, "redis_mode:cluster");
			if (ptr)
			{
				_mode = enum_RedisModeCluster;
			}
		}
	}

	if (enum_RedisModeNone == _mode)
	{
		//ERRORS("enum_RedisModeNone == _mode");
		return false;
	}

	const char cmd[] = "cluster slots";

	RedisReplyPtr _reply = rs->exeCmd(cmd);

	if (nullptr == _reply)
	{
		//ERRORS("cluster slots nullptr == _reply");
		return false;
	}

	if (_reply->type != REDIS_REPLY_ARRAY)
	{
		//ERRORS("cluster slots _reply type=%d", _reply->type);
		return false;
	}

	for (size_t i = 0; i < _reply->elements; i++)
	{
		redisReply *ptrCluster = _reply->element[i];
		if (ptrCluster && REDIS_REPLY_ARRAY == ptrCluster->type)
		{
			for (size_t j = 0; j < ptrCluster->elements; j++)
			{
				redisReply *ptrNode = ptrCluster->element[j];
				if (!ptrNode)	{ continue;	}

				if (REDIS_REPLY_INTEGER == ptrNode->type) // get node slots
				{
					if (0 == j % 2) //处理slots ,单个节点可能有多个
					{
						_nodes[i].slot[i].beg = ptrNode->integer;
					}
					else
					{
						_nodes[i].slot[i].end = ptrNode->integer;
					}
				}
				else if (REDIS_REPLY_ARRAY == ptrNode->type) //get node ip:port
				{
					if( ptrNode->elements == 0)
						continue;

					for (size_t k = 0; k < 2; k++)
					{
						redisReply *ptrHost = ptrNode->element[k];
						if (!ptrHost) { continue; }

						if (REDIS_REPLY_STRING == ptrHost->type)
						{
							if (ptrHost->len > 0 && ptrHost->len <= 15)  //IP
							{
								int ipLen = sizeof(_nodes[i].ip) - 1;
								std::strncpy(_nodes[i].ip, ptrHost->str, (ptrHost->len > ipLen) ? ipLen : ptrHost->len);
							}
							//else //node id
						}
						else if (REDIS_REPLY_INTEGER == ptrHost->type)
						{
							_nodes[i].port = (int)ptrHost->integer;
						}
					}
					break;
				}
			}
		}
	}

	return true;
}

RedisClientPtr NFCRedisCluster::getRedisConnect(const char *keyBuf, size_t keyLen)
{
	int n = crc16(keyBuf, keyLen);
	int slots = n % DEF_MAX_REDIS_SERVERS;
	RedisClientPtr rs = _client.at(slots);

	return rs;
}

RedisClientPtr NFCRedisCluster::getRedis(const char *keyBuf, size_t keyLen) throw()
{
	RedisClientPtr rs = nullptr;

	try
	{
		rs = getRedisConnect(keyBuf,keyLen);
		if (rs != nullptr)
		{
			if(!rs->IsConnect())
			{
				rs->reConnect();
			}
		}
	}
	catch (std::exception& e)
	{
		//ERRORS("redis 节点选择错误[%s]", e.what());
	}
	catch (...)
	{
		//ERRORS("redis 节点选择未知严重错误");
	}

	return rs;
}

bool NFCRedisCluster::finit()
{
	for (auto&ref : _client)
	{
		ref->_bDestroy = true;
		ref = nullptr;
	}
	return true;
}

bool NFCRedisCluster::init(const char *ip, unsigned short port)
{
	try
	{
		std::vector<RedisClientPtr> vecConnect;
		auto lambda_rs = [](std::array<RedisClientPtr, DEF_MAX_REDIS_SERVERS>& clients, std::map<size_t, NFRedisSlot> &slot, RedisClientPtr rs)
		{
			for (auto &it : slot)
			{
				for (size_t i = it.second.beg; i <= it.second.end; i++)
				{
					clients.at(i) = rs;//[i] = rs;
				}
			}
		};

		RedisClientPtr rs(new NFCRedisClient(ip, port));
		if (nullptr == rs)
		{
			return false;
		}

		vecConnect.push_back(rs);
		restNodes();

		if (!getClusterSlot(rs))
		{
			return false;
		}		

		if (enum_RedisModeAlone == _mode)
		{
			for (size_t i = 0; i < DEF_MAX_REDIS_SERVERS; i++)
			{
				_client.at(i) = rs;
			}
			return true;
		}

		//cluster mode
		std::map<size_t, NFRedisNode>::iterator iter = _nodes.begin();
		for (; iter != _nodes.end(); iter++)
		{
			NFRedisNode& node = iter->second;

			if (0 == std::strcmp(node.ip, ip) && node.port == port)
			{
				lambda_rs(_client, node.slot, rs);
				continue;
			}

			bool bFind = false;
			for (auto& ref : vecConnect)
			{
				if (strcmp(ref->getConnectMaster()->GetAddress().ip , node.ip) == 0 && ref->getConnectMaster()->GetAddress().wPort == node.port)
				{
					lambda_rs(_client, node.slot, ref);
					bFind = true;
					break;
				}
			}

			if (!bFind)
			{
				RedisClientPtr rs2(new NFCRedisClient(node.ip, node.port));

				if (nullptr == rs2)
				{
					return false;
				}

				lambda_rs(_client, node.slot, rs2);
				vecConnect.push_back(rs2);
			}
		}

	}
	catch (std::exception& e)
	{
		//ERRORS("redis 节点数组越界[%s]", e.what());
	}

	catch (...)
	{
		//ERRORS("Error catch \n");
		return false;
	}

	return true;
}
