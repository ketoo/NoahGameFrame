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

#ifndef NF_ASYNC_DATANOSQL_CLUSTER_MODULE_H
#define NF_ASYNC_DATANOSQL_CLUSTER_MODULE_H

#include "NFRedisClient.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "Dependencies/concurrentqueue/concurrentqueue.h"

//only support write cmd
//only support write cmd
//only support write cmd
//only support write cmd
//only support write cmd
//only support write cmd

enum RedisCMD
{
	AUTH,
	INFO,
	DEL,
	EXISTS,
	EXPIRE,
	EXPIREAT,
	PERSIST,
	TTL,
	TYPE,
	APPEND,
	DECR,
	DECRBY,
	GET,
	GETSET,
	INCR,
	INCRBY,
	INCRBYFLOAT,
	MGET,
	MSET,
	SET,
	SETEX,
	SETNX,
	STRLEN,
	HDEL,
	HEXISTS,
	HGET,
	HGETALL,
	HINCRBY,
	HINCRBYFLOAT,
	HKEYS,
	HLEN,
	HMGET,
	HMSET,
	HSET,
	HSETNX,
	HVALS,
	HSTRLEN,
	LINDEX,
	LLEN,
	LPOP,
	LPUSH,
	LPUSHX,
	LRANGE,
	LSET,
	RPOP,
	RPUSH,
	RPUSHX,
	SADD,
	SCARD,
	SDIFF,
	SDIFFSTORE,
	SINTER,
	SINTERSTORE,
	SISMEMBER,
	SMEMBERS,
	SMOVE,
	SPOP,
	SRANDMEMBER,
	SREM,
	SUNION,
	SUNIONSTORE,
	ZADD,
	ZCARD,
	ZCOUNT,
	ZINCRBY,
	ZRANGE,
	ZRANGEBYSCORE,
	ZRANK,
	ZREM,
	ZREMRANGEBYRANK,
	ZREMRANGEBYSCORE,
	ZREVRANGE,
	ZREVRANGEBYSCORE,
	ZREVRANK,
	ZSCORE,
	FLUSHALL,
	FLUSHDB,

	CLUSTERNODES,
	CLUSTERINFO,
};

class RedisClientThread
{
private:
	RedisClientThread(){};

	void Execute()
	{
		while (1)
		{
			sleep(1);

			NF_SHARE_PTR<NFRedisCommand> redisCommand = nullptr;
			if (queue.try_dequeue(redisCommand) && redisCommand)
			{
				NF_SHARE_PTR<NFRedisReply> redisReply = redisClient->BuildSendCmd(*redisCommand);
				trackerQueue->enqueue(redisReply);
			}
		}
	}
public:
	RedisClientThread(NF_SHARE_PTR<NFIRedisClient> r, moodycamel::ConcurrentQueue<NF_SHARE_PTR<NFRedisReply>>* tracker)
	{
		redisClient = r;
		trackerQueue = tracker;
		thread = new std::thread(&RedisClientThread::Execute, this);
	}

	void Enqueue(NF_SHARE_PTR<NFRedisCommand> cmd)
	{
		queue.enqueue(cmd);
	}

	NF_SHARE_PTR<NFIRedisClient> redisClient;

private:
	std::thread* thread;
	moodycamel::ConcurrentQueue<NF_SHARE_PTR<NFRedisCommand>> queue;
	moodycamel::ConcurrentQueue<NF_SHARE_PTR<NFRedisReply>>* trackerQueue;
};

class NFAsyncNoSqlModule
	: public NFIAsyncNoSqlModule
{
public:

	NFAsyncNoSqlModule(NFIPluginManager* p);
	virtual ~NFAsyncNoSqlModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	virtual bool Connect(const std::string& ip, const int port, const std::string& auth, const bool sync = true);

	virtual NF_SHARE_PTR<NFIRedisClient> FindRedisClient(int slot);

	virtual bool Enable();
	virtual bool Busy();
	virtual bool KeepLive();


protected:

	void CheckConnect();
	//如果收到move消息，就调用此函数强制刷新nodes
	void ConnectAllMasterNodes(NF_SHARE_PTR<NFIRedisClient> redisClient);

protected:
	NFINT64 mLastCheckTime;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;

	moodycamel::ConcurrentQueue<NF_SHARE_PTR<NFRedisReply>> trackerQueue;
	moodycamel::ConcurrentQueue<NF_SHARE_PTR<NFRedisReply>> masterNodesQueue;
	std::list<NF_SHARE_PTR<RedisClientThread>> redisClients;
};

#endif