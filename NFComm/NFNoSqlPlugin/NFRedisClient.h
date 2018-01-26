//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISCLIENT_H
#define NFREDISPLUGIN_NFREDISCLIENT_H


#include <string>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <ctime>
#include <sstream>
#include <iostream>
#include <random>

#include "NFRedisResult.h"
#include "NFRedisCommand.h"

typedef std::string string_type;
typedef std::vector<string_type> string_vector;
typedef std::pair<string_type, string_type> string_pair;
typedef std::vector<string_pair> string_pair_vector;
typedef std::pair<string_type, double> string_score_pair;
typedef std::vector<string_score_pair> string_score_vector;
typedef std::set<string_type> string_set;

typedef void(*CoroutineYieldFunction)();
typedef void(*CoroutineStartFunction)();

static CoroutineYieldFunction YieldFunction = NULL;
static CoroutineStartFunction StartFunction = NULL;

class NFRedisClient
{
public:
    NFRedisClient();

    bool ConnectTo(const std::string& ip, const int port, const std::string& auth);


    bool KeepLive();

    bool Execute();

	/**
	* @brie if you have setted a password for Redis, you much use AUTH cmd to connect to the server than you can use other cmds
	* @param password
	* @return success: true; fail: false
	*/
	NF_SHARE_PTR<NFRedisResult> AUTH(const std::string& auth);

	/**
	* @brie select DB
	* @param DB index
	* @return success: true; fail: false
	*/
	bool SelectDB(int dbnum);

/*
    ECHO
    PING
    QUIT
    */
    /////////client key//////////////
	/**
	* @brie del a key
	* @param key's name
	* @return success: true; fail: false
	*/
    NF_SHARE_PTR<NFRedisResult> DEL(const std::string& key);

    //NF_SHARE_PTR<NFRedisResult> DUMP(const std::string& key, std::string& out);

	/**
	* @brie Returns if key exists.
	* @param key's name
	* @return true if the key exists, false if the key does not exist.
	*/
    NF_SHARE_PTR<NFRedisResult> EXISTS(const std::string& key);

	/**
	* @brief Set a timeout on key. After the timeout has expired, the key will automatically be deleted
	* @param keys [in] name of key
	* @param seconds [in] the key will be destroy after this second
	* @return The true for set or reset success, false for failed(if key does not exist)
	*/
    NF_SHARE_PTR<NFRedisResult> EXPIRE(const std::string& key, const unsigned int secs);

	/**
	* @brief this function like EXPIRE, which is to set the lfie time for one key
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @param timestamp [in] the key will be destroy after this timestamp
	* @return true if the timeout was set, false if key does not exist.
	*/
    NF_SHARE_PTR<NFRedisResult> EXPIREAT(const std::string& key, const int64_t unixTime);
    //NF_SHARE_PTR<NFRedisResult> KEYS(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> MIGRATE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> MOVE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> OBJECT(const std::string& key);
	/**
	* @brief Remove the existing timeout on key, turning the key from volatile (a key with an expire set) to persistent(a key that will never expire as no timeout is associated).
	* the difference is the times of the cmd EXPIREAT is unix timestamp
	* @param keys [in] name of key
	* @param timestamp [in] the key will be destroy after this timestamp
	* @return true if the timeout was set, false if key does not exist.
	*/
	NF_SHARE_PTR<NFRedisResult> PERSIST(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PEXPIRE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PEXPIREAT(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PTTL(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RANDOMKEY(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RENAME(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RENAMENX(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RESTORE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> SORT(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> TTL(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> TYPE(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> SCAN(const std::string& key);


    /////////client String//////////////

    NF_SHARE_PTR<NFRedisResult> APPEND(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> BITCOUNT
    //NF_SHARE_PTR<NFRedisResult> BITOP
    //NF_SHARE_PTR<NFRedisResult> BITFIELD(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> DECR(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> DECRBY(const std::string& key, const int64_t v);
    NF_SHARE_PTR<NFRedisResult> GET(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> GETBIT(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> GETRANGE(const std::string& key);

    NF_SHARE_PTR<NFRedisResult> GETSET(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> INCR(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> INCRBY(const std::string& key, const int64_t v);
    NF_SHARE_PTR<NFRedisResult> INCRBYFLOAT(const std::string& key, const float fv);

    NF_SHARE_PTR<NFRedisResult> MGET(const string_vector& keys, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> MSET(const string_pair_vector& values);

    //NF_SHARE_PTR<NFRedisResult> MSETNX(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> PSETEX(const std::string& key);

    NF_SHARE_PTR<NFRedisResult> SET(const std::string& key, const std::string& value);

    //NF_SHARE_PTR<NFRedisResult> SETBIT(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> SETEX(const std::string& key, const std::string& value, int time);
    NF_SHARE_PTR<NFRedisResult> SETNX(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> SETRANGE(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> STRLEN(const std::string& key);

    /////////client hash//////////////
    NF_SHARE_PTR<NFRedisResult> HDEL(const std::string& key, const std::string& field);
    NF_SHARE_PTR<NFRedisResult> HEXISTS(const std::string& key, const std::string& field);
    NF_SHARE_PTR<NFRedisResult> HGET(const std::string& key, const std::string& field);
    NF_SHARE_PTR<NFRedisResult> HGETALL(const std::string& key, std::vector<string_pair>& values);
    NF_SHARE_PTR<NFRedisResult> HINCRBY(const std::string& key, const std::string& field, const int by);
    NF_SHARE_PTR<NFRedisResult> HINCRBYFLOAT(const std::string& key, const std::string& field, const float by);
    NF_SHARE_PTR<NFRedisResult> HKEYS(const std::string& key, std::vector<std::string>& fields);
    NF_SHARE_PTR<NFRedisResult> HLEN(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> HMGET(const std::string& key, const string_vector& fields, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> HMSET(const std::string& key, const std::vector<string_pair>& values);
    NF_SHARE_PTR<NFRedisResult> HSET(const std::string& key, const std::string& field, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> HSETNX(const std::string& key, const std::string& field, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> HVALS(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> HSCAN(const std::string& key, const std::string& field);
    NF_SHARE_PTR<NFRedisResult> HSTRLEN(const std::string& key, const std::string& field);

    /////////client list//////////////
    //NF_SHARE_PTR<NFRedisResult> BLPOP(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> BRPOP(const std::string& key, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> BRPOPLPUSH(const std::string& key, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> LINDEX(const std::string& key, const int index);
    //NF_SHARE_PTR<NFRedisResult> LINSERT(const std::string& key, const std::string& value1, const std::string& value2);
    NF_SHARE_PTR<NFRedisResult> LLEN(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> LPOP(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> LPUSH(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> LPUSHX(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> LRANGE(const std::string& key, const int start, const int end, string_vector& values);
    //NF_SHARE_PTR<NFRedisResult> LREM(const std::string& key, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> LSET(const std::string& key, const int index, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> LTRIM(const std::string& key, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> RPOP(const std::string& key);
    //NF_SHARE_PTR<NFRedisResult> RPOPLPUSH(const std::string& key, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> RPUSH(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> RPUSHX(const std::string& key, const std::string& value);

    /////////client set//////////////
    /*
    NF_SHARE_PTR<NFRedisResult> SADD(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SCARD(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SDIFF(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SDIFFSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SINTER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SINTERSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SISMEMBER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SMEMBERS(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SMOVE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SPOP(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SRANDMEMBER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SREM(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SUNION(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SUNIONSTORE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SSCAN(const std::string& key, const std::string& value);
*/

    /////////client SortedSet//////////////
 
    NF_SHARE_PTR<NFRedisResult> ZADD(const std::string& key, const std::string& member, const double score);
    NF_SHARE_PTR<NFRedisResult> ZCARD(const std::string& key);
    NF_SHARE_PTR<NFRedisResult> ZCOUNT(const std::string& key, const double start, const double end);
    NF_SHARE_PTR<NFRedisResult> ZINCRBY(const std::string& key, const std::string & member, const double score);
    NF_SHARE_PTR<NFRedisResult> ZRANGE(const std::string& key, const int start, const int end, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> ZRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> ZRANK(const std::string & key, const std::string & member);
    NF_SHARE_PTR<NFRedisResult> ZREM(const std::string& key, const std::string& member);
    NF_SHARE_PTR<NFRedisResult> ZREMRANGEBYRANK(const std::string& key, const int start, const int end);
    NF_SHARE_PTR<NFRedisResult> ZREMRANGEBYSCORE(const std::string& key, const double start, const double end);
    NF_SHARE_PTR<NFRedisResult> ZREVRANGE(const std::string& key, const int start, const int end, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> ZREVRANGEBYSCORE(const std::string & key, const double start, const double end, string_vector& values);
    NF_SHARE_PTR<NFRedisResult> ZREVRANK(const std::string& key, const std::string& member);
    NF_SHARE_PTR<NFRedisResult> ZSCORE(const std::string& key, const std::string& member);
    //NF_SHARE_PTR<NFRedisResult> ZUNIONSTORE(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZINTERSTORE(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZSCAN(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZRANGEBYLEX(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZLEXCOUNT(const std::string& key, const std::string& value);
    //NF_SHARE_PTR<NFRedisResult> ZREMRANGEBYLEX(const std::string& key, const std::string& value);

    /////////client GEO//////////////

    /*
    NF_SHARE_PTR<NFRedisResult> GEOADD(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> GEOPOS(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> GEODIST(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> GEORADIUS(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> GEORADIUSBYMEMBER(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> GEOHASH(const std::string& key, const std::string& value);
*/

    /////////client PUB SUB//////////////
	/*
    NF_SHARE_PTR<NFRedisResult> PSUBSCRIBE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> PUBLISH(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> PUBSUB(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> PUNSUBSCRIBE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> SUBSCRIBE(const std::string& key, const std::string& value);
    NF_SHARE_PTR<NFRedisResult> UNSUBSCRIBE(const std::string& key, const std::string& value);
	*/
    /////////client server//////////////
    NF_SHARE_PTR<NFRedisResult> FLUSHALL();
    NF_SHARE_PTR<NFRedisResult> FLUSHDB();

private:

	NF_SHARE_PTR<NFRedisResult> GetUnuseResult();
	NF_SHARE_PTR<NFRedisResult> BuildSendCmd(const NFRedisCommand& cmd);
	void WaitingResult(NF_SHARE_PTR<NFRedisResult> xRedisResult);

private:
	std::list<NF_SHARE_PTR<NFRedisResult>> mlCmdResultList;
	std::list<NF_SHARE_PTR<NFRedisResult>> mlUnusedResultList;

private:
    NFRedisClientSocket* m_pRedisClientSocket;
};


#endif //NFREDISPLUGIN_NFREDISCLIENT_H
