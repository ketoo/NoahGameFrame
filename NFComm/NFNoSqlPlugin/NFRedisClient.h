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

#ifndef NFREDISPLUGIN_NFREDISCLIENT_H
#define NFREDISPLUGIN_NFREDISCLIENT_H

#define GET_NAME(functionName)   (#functionName)

#include <string>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <ctime>
#include <sstream>
#include <iostream>
#include <random>
#include <thread>

#include "NFRedisCommand.h"
#include "NFRedisClientSocket.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"

class NFRedisReply
{
public:
	int64_t index = 0;
	std::string error;
	NF_SHARE_PTR<redisReply> reply = nullptr;
};

class NFRedisClient : public NFIRedisClient
{
public:
	NFRedisClient();
	virtual ~NFRedisClient() {}

	virtual bool Connect(const std::string& ip, const int port, const std::string& auth, const bool sync = true) override ;

	virtual bool Enable() override ;
	virtual bool Authenticated() override ;

	virtual bool Execute() override ;
	virtual bool KeepLive() override ;

	virtual void UpdateSlot(int start, int end) override ;

	virtual bool IsConnected() override ;


	virtual bool AUTH(const std::string& auth) override;
	virtual bool INFO(std::string& info) override;

	/*
	ECHO
	PING
	QUIT
	*/
	/////////client key//////////////
	virtual bool DEL(const std::string& key) override;
	//NF_SHARE_PTR<NFRedisResult> DUMP(const std::string& key, std::string& out);
	virtual bool EXISTS(const std::string& key) override;
	virtual bool EXPIRE(const std::string& key, const unsigned int secs) override;
	virtual bool EXPIREAT(const std::string& key, const int64_t unixTime) override;
	virtual bool PERSIST(const std::string& key) override;
	virtual bool TTL(const std::string& key, int& ttl) override;
	virtual bool TYPE(const std::string& key, std::string& type) override;
	//NF_SHARE_PTR<NFRedisResult> SCAN(const std::string& key);


	/////////client String//////////////
	virtual bool APPEND(const std::string& key, const std::string& value, int& length) override;
	//NF_SHARE_PTR<NFRedisResult> BITCOUNT
	//NF_SHARE_PTR<NFRedisResult> BITOP
	//NF_SHARE_PTR<NFRedisResult> BITFIELD(const std::string& key);
	virtual bool DECR(const std::string& key, int64_t& value) override;
	virtual bool DECRBY(const std::string& key, const int64_t decrement, int64_t& value) override;
	virtual bool GET(const std::string& key, std::string & value) override;
	//NF_SHARE_PTR<NFRedisResult> GETBIT(const std::string& key);
	//NF_SHARE_PTR<NFRedisResult> GETRANGE(const std::string& key);
	virtual bool GETSET(const std::string& key, const std::string& value, std::string& oldValue) override;
	virtual bool INCR(const std::string& key, int64_t& value) override;
	virtual bool INCRBY(const std::string& key, const int64_t increment, int64_t& value) override;
	virtual bool INCRBYFLOAT(const std::string& key, const float increment, float& value) override;
	//NF_SHARE_PTR<NFRedisResult> MSETNX(const std::string& key);
	//NF_SHARE_PTR<NFRedisResult> PSETEX(const std::string& key);
	virtual bool SET(const std::string& key, const std::string& value) override;
	//NF_SHARE_PTR<NFRedisResult> SETBIT(const std::string& key);
	virtual bool SETEX(const std::string& key, const std::string& value, int time) override;
	virtual bool SETNX(const std::string& key, const std::string& value) override;
	//NF_SHARE_PTR<NFRedisResult> SETRANGE(const std::string& key);
	virtual bool STRLEN(const std::string& key, int& length) override;

	/////////client hash//////////////
	virtual bool HDEL(const std::string& key, const std::string& field) override;
	virtual bool HDEL(const std::string& key, const string_vector& fields) override;
	virtual bool HEXISTS(const std::string& key, const std::string& field) override;
	virtual bool HGET(const std::string& key, const std::string& field, std::string& value) override;
	virtual bool HGETALL(const std::string& key, std::vector<string_pair>& values) override;
	virtual bool HINCRBY(const std::string& key, const std::string& field, const int by, int64_t& value) override;
	virtual bool HINCRBYFLOAT(const std::string& key, const std::string& field, const float by, float& value) override;
	virtual bool HKEYS(const std::string& key, std::vector<std::string>& fields) override;
	virtual bool HLEN(const std::string& key, int& number) override;
	virtual bool HMGET(const std::string& key, const string_vector& fields, string_vector& values) override;
	virtual bool HMSET(const std::string& key, const std::vector<string_pair>& values) override;
	virtual bool HMSET(const std::string& key, const string_vector& fields, const string_vector& values) override;
	virtual bool HSET(const std::string& key, const std::string& field, const std::string& value) override;
	virtual bool HSETNX(const std::string& key, const std::string& field, const std::string& value) override;
	virtual bool HVALS(const std::string& key, string_vector& values) override;
	//NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);
	virtual bool HSTRLEN(const std::string& key, const std::string& field, int& length) override;

	/////////client list//////////////

	//NF_SHARE_PTR<NFRedisResult> BLPOP(const std::string& key, string_vector& values);
	//NF_SHARE_PTR<NFRedisResult> BRPOP(const std::string& key, string_vector& values);
	//NF_SHARE_PTR<NFRedisResult> BRPOPLPUSH(const std::string& key, string_vector& values);
	virtual bool LINDEX(const std::string& key, const int index, std::string& value) override;
	//NF_SHARE_PTR<NFRedisResult> LINSERT(const std::string& key, const std::string& value1, const std::string& value2);
	virtual bool LLEN(const std::string& key, int& length) override;
	virtual bool LPOP(const std::string& key, std::string& value) override;
	virtual bool LPUSH(const std::string& key, const std::string& value, int& length) override;
	virtual bool LPUSHX(const std::string& key, const std::string& value, int& length) override;
	virtual bool LRANGE(const std::string& key, const int start, const int end, string_vector& values) override;
	//NF_SHARE_PTR<NFRedisResult> LREM(const std::string& key, string_vector& values);
	virtual bool LSET(const std::string& key, const int index, const std::string& value) override;
	//NF_SHARE_PTR<NFRedisResult> LTRIM(const std::string& key, string_vector& values);
	virtual bool RPOP(const std::string& key, std::string& value) override;
	//NF_SHARE_PTR<NFRedisResult> RPOPLPUSH(const std::string& key, string_vector& values);
	virtual bool RPUSH(const std::string& key, const std::string& value, int& length) override;
	virtual bool RPUSHX(const std::string& key, const std::string& value, int& length) override;

	/////////client set//////////////

	virtual bool SADD(const std::string& key, const std::string& member, int& length) override;
	virtual bool SCARD(const std::string& key, int& count) override;
	//virtual bool SDIFF(const std::string& key_1, const std::string& key_2, string_vector& output) override;
	virtual bool SDIFFSTORE(const std::string& store_key, const std::string& diff_key1, const std::string& diff_key2, int& num) override;
	//virtual bool SINTER(const std::string& key_1, const std::string& key_2, string_vector& output) override;
	//virtual bool SINTERSTORE(const std::string& inter_store_key, const std::string& inter_key1, const std::string& inter_key2) override;
	virtual bool SISMEMBER(const std::string& key, const std::string& member) override;
	virtual bool SMEMBERS(const std::string& key, string_vector& output) override;
	//virtual bool SMOVE(const std::string& source_key, const std::string& dest_key, const std::string& member) override;
	virtual bool SPOP(const std::string& key, std::string& output) override;
	virtual bool SRANDMEMBER(const std::string& key, int count, string_vector& output) override;
	virtual bool SREM(const std::string& key, const string_vector& members, int& number) override;
	//virtual bool SUNION(const std::string& union_key1, const std::string& union_key2, string_vector& output) override;
	virtual bool SUNIONSTORE(const std::string& dest_store_key, const std::string& union_key1, const std::string& union_key2) override;


	/////////client SortedSet//////////////
	virtual bool ZADD(const std::string& key, const std::string& member, const double score, int& number) override;
	virtual bool ZCARD(const std::string& key, int &count) override;
	virtual bool ZCOUNT(const std::string& key, const double start, const double end, int &count) override;
	virtual bool ZINCRBY(const std::string& key, const std::string & member, const double score, double& newScore) override;
	virtual bool ZRANGE(const std::string& key, const int start, const int end, string_score_vector& values) override;
	virtual bool ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values) override;
	virtual bool ZRANK(const std::string & key, const std::string & member, int& rank) override;
	virtual bool ZREM(const std::string& key, const std::string& member) override;
	virtual bool ZREMRANGEBYRANK(const std::string& key, const int start, const int end) override;
	virtual bool ZREMRANGEBYSCORE(const std::string& key, const double min, const double max) override;
	virtual bool ZREVRANGE(const std::string& key, const int start, const int end, string_score_vector& values) override;
	virtual bool ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_score_vector& values) override;
	virtual bool ZREVRANK(const std::string& key, const std::string& member, int& rank) override;
	virtual bool ZSCORE(const std::string& key, const std::string& member, double& score) override;


	/////////client server//////////////
	virtual void FLUSHALL() override;
	virtual void FLUSHDB() override;


	/////////client pubsub//////////////
	//virtual bool PUBLISH(const std::string& key, const std::string& value) override;
	//virtual bool SUBSCRIBE(const std::string& key) override;
	//virtual	bool UNSUBSCRIBE(const std::string& key) override;

	//cluster
	virtual	bool CLUSTERNODES(std::vector<std::string>& clusters, bool onlyMasterNode = true, bool includeSelfMaster = false) override;
	virtual	bool CLUSTERINFO(std::string& clusterInfo) override;


	virtual NF_SHARE_PTR<NFRedisReply> LastReply();

protected:

	virtual NF_SHARE_PTR<NFRedisReply> BuildSendCmd(const NFRedisCommand& cmd);
	virtual NF_SHARE_PTR<NFRedisReply> ParseForReply();

private:
	NF_SHARE_PTR<NFRedisReply> lastReply = nullptr;
	bool synchronization = true;
	bool mbEnable = true;
	bool mbAuthed = false;
	NFRedisClientSocket* m_pRedisClientSocket;
};


#endif //NFREDISPLUGIN_NFREDISCLIENT_H
