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

class NFRedisClient
{
public:
    NFRedisClient();

    bool ConnectTo(const std::string& ip, const int port, const std::string& auth);

    bool SelectDB(int dbnum);

    bool KeepLive();

    bool Execute();

    NFRedisResult* GetRedisResult();


	NFRedisResult* AUTH(const std::string& auth);
/*
    ECHO
    PING
    QUIT
    SELECT
    */
    /////////client key//////////////

    NFRedisResult* DEL(const std::string& key);
    //NFRedisResult* DUMP(const std::string& key, std::string& out);

    NFRedisResult* EXISTS(const std::string& key);
    NFRedisResult* EXPIRE(const std::string& key, const unsigned int secs);
    NFRedisResult* EXPIREAT(const std::string& key, const int64_t unixTime);
    //NFRedisResult* KEYS(const std::string& key);
    //NFRedisResult* MIGRATE(const std::string& key);
    //NFRedisResult* MOVE(const std::string& key);
    //NFRedisResult* OBJECT(const std::string& key);
    NFRedisResult* PERSIST(const std::string& key);
    //NFRedisResult* PEXPIRE(const std::string& key);
    //NFRedisResult* PEXPIREAT(const std::string& key);
    //NFRedisResult* PTTL(const std::string& key);
    //NFRedisResult* RANDOMKEY(const std::string& key);
    //NFRedisResult* RENAME(const std::string& key);
    //NFRedisResult* RENAMENX(const std::string& key);
    //NFRedisResult* RESTORE(const std::string& key);
    //NFRedisResult* SORT(const std::string& key);
    NFRedisResult* TTL(const std::string& key);
    NFRedisResult* TYPE(const std::string& key);
    //NFRedisResult* SCAN(const std::string& key);
    /////////client String//////////////

    NFRedisResult* APPEND(const std::string& key, const std::string& value);
    //NFRedisResult* BITCOUNT
    //NFRedisResult* BITOP
    //NFRedisResult* BITFIELD(const std::string& key);
    NFRedisResult* DECR(const std::string& key);
    NFRedisResult* DECRBY(const std::string& key, const int64_t v);
    NFRedisResult* GET(const std::string& key);
    //NFRedisResult* GETBIT(const std::string& key);
    //NFRedisResult* GETRANGE(const std::string& key);

    NFRedisResult* GETSET(const std::string& key, const std::string& value);
    NFRedisResult* INCR(const std::string& key);
    NFRedisResult* INCRBY(const std::string& key, const int64_t v);
    NFRedisResult* INCRBYFLOAT(const std::string& key, const float fv);

    NFRedisResult* MGET(const string_vector& keys, string_vector& values);
    NFRedisResult* MSET(const string_pair_vector& values);

    //NFRedisResult* MSETNX(const std::string& key);
    //NFRedisResult* PSETEX(const std::string& key);

    NFRedisResult* SET(const std::string& key, const std::string& value);

    //NFRedisResult* SETBIT(const std::string& key);
    NFRedisResult* SETEX(const std::string& key, const std::string& value, int time);
    NFRedisResult* SETNX(const std::string& key, const std::string& value);
    //NFRedisResult* SETRANGE(const std::string& key);
    NFRedisResult* STRLEN(const std::string& key);

    /////////client hash//////////////
    NFRedisResult* HDEL(const std::string& key, const std::string& field);
    NFRedisResult* HEXISTS(const std::string& key, const std::string& field);
    NFRedisResult* HGET(const std::string& key, const std::string& field);
    NFRedisResult* HGETALL(const std::string& key, std::vector<string_pair>& values);
    NFRedisResult* HINCRBY(const std::string& key, const std::string& field, const int by);
    NFRedisResult* HINCRBYFLOAT(const std::string& key, const std::string& field, const float by);
    NFRedisResult* HKEYS(const std::string& key, std::vector<std::string>& fields);
    NFRedisResult* HLEN(const std::string& key);
    NFRedisResult* HMGET(const std::string& key, const string_vector& fields, string_vector& values);
    NFRedisResult* HMSET(const std::string& key, const std::vector<string_pair>& values);
    NFRedisResult* HSET(const std::string& key, const std::string& field, const std::string& value);
    NFRedisResult* HSETNX(const std::string& key, const std::string& field, const std::string& value);
    NFRedisResult* HVALS(const std::string& key, string_vector& values);
    //NFRedisResult* HSCAN(const std::string& key, const std::string& field);
    NFRedisResult* HSTRLEN(const std::string& key, const std::string& field);

    /////////client list//////////////
    //NFRedisResult* BLPOP(const std::string& key, string_vector& values);
    //NFRedisResult* BRPOP(const std::string& key, string_vector& values);
    //NFRedisResult* BRPOPLPUSH(const std::string& key, string_vector& values);
    NFRedisResult* LINDEX(const std::string& key, const int index);
    //NFRedisResult* LINSERT(const std::string& key, const std::string& value1, const std::string& value2);
    NFRedisResult* LLEN(const std::string& key);
    NFRedisResult* LPOP(const std::string& key);
    NFRedisResult* LPUSH(const std::string& key, const std::string& value);
    NFRedisResult* LPUSHX(const std::string& key, const std::string& value);
    NFRedisResult* LRANGE(const std::string& key, const int start, const int end, string_vector& values);
    //NFRedisResult* LREM(const std::string& key, string_vector& values);
    NFRedisResult* LSET(const std::string& key, const int index, const std::string& value);
    //NFRedisResult* LTRIM(const std::string& key, string_vector& values);
    NFRedisResult* RPOP(const std::string& key);
    //NFRedisResult* RPOPLPUSH(const std::string& key, string_vector& values);
    NFRedisResult* RPUSH(const std::string& key, const std::string& value);
    NFRedisResult* RPUSHX(const std::string& key, const std::string& value);

    /////////client set//////////////
    /*
    NFRedisResult* SADD(const std::string& key, const std::string& value);
    NFRedisResult* SCARD(const std::string& key, const std::string& value);
    NFRedisResult* SDIFF(const std::string& key, const std::string& value);
    NFRedisResult* SDIFFSTORE(const std::string& key, const std::string& value);
    NFRedisResult* SINTER(const std::string& key, const std::string& value);
    NFRedisResult* SINTERSTORE(const std::string& key, const std::string& value);
    NFRedisResult* SISMEMBER(const std::string& key, const std::string& value);
    NFRedisResult* SMEMBERS(const std::string& key, const std::string& value);
    NFRedisResult* SMOVE(const std::string& key, const std::string& value);
    NFRedisResult* SPOP(const std::string& key, const std::string& value);
    NFRedisResult* SRANDMEMBER(const std::string& key, const std::string& value);
    NFRedisResult* SREM(const std::string& key, const std::string& value);
    NFRedisResult* SUNION(const std::string& key, const std::string& value);
    NFRedisResult* SUNIONSTORE(const std::string& key, const std::string& value);
    NFRedisResult* SSCAN(const std::string& key, const std::string& value);
*/

    /////////client SortedSet//////////////
    /*
    NFRedisResult* ZADD(const std::string& key, const std::string& member, const double score);
    NFRedisResult* ZCARD(const std::string& key, const std::string& value);
    NFRedisResult* ZCOUNT(const std::string& key, const double start, const double end);
    NFRedisResult* ZINCRBY(const std::string& key, const std::string& value);
    NFRedisResult* ZRANGE(const std::string& key, const std::string& value);
    NFRedisResult* ZRANGEBYSCORE(const std::string& key, const std::string& value);
    NFRedisResult* ZRANK(const std::string& key, const std::string& value);
    NFRedisResult* ZREM(const std::string& key, const std::string& value);
    NFRedisResult* ZREMRANGEBYRANK(const std::string& key, const std::string& value);
    NFRedisResult* ZREMRANGEBYSCORE(const std::string& key, const std::string& value);
    NFRedisResult* ZREVRANGE(const std::string& key, const std::string& value);
    NFRedisResult* ZREVRANGEBYSCORE(const std::string& key, const std::string& value);
    NFRedisResult* ZREVRANK(const std::string& key, const std::string& value);
    NFRedisResult* ZSCORE(const std::string& key, const std::string& value);
    NFRedisResult* ZUNIONSTORE(const std::string& key, const std::string& value);
    NFRedisResult* ZINTERSTORE(const std::string& key, const std::string& value);
    NFRedisResult* ZSCAN(const std::string& key, const std::string& value);
    NFRedisResult* ZRANGEBYLEX(const std::string& key, const std::string& value);
    NFRedisResult* ZLEXCOUNT(const std::string& key, const std::string& value);
    NFRedisResult* ZREMRANGEBYLEX(const std::string& key, const std::string& value);
*/

    /////////client GEO//////////////

    /*
    NFRedisResult* GEOADD(const std::string& key, const std::string& value);
    NFRedisResult* GEOPOS(const std::string& key, const std::string& value);
    NFRedisResult* GEODIST(const std::string& key, const std::string& value);
    NFRedisResult* GEORADIUS(const std::string& key, const std::string& value);
    NFRedisResult* GEORADIUSBYMEMBER(const std::string& key, const std::string& value);
    NFRedisResult* GEOHASH(const std::string& key, const std::string& value);
*/

    /////////client PUB SUB//////////////
/*
    NFRedisResult* PSUBSCRIBE(const std::string& key, const std::string& value);
    NFRedisResult* PUBLISH(const std::string& key, const std::string& value);
    NFRedisResult* PUBSUB(const std::string& key, const std::string& value);
    NFRedisResult* PUNSUBSCRIBE(const std::string& key, const std::string& value);
    NFRedisResult* SUBSCRIBE(const std::string& key, const std::string& value);
    NFRedisResult* UNSUBSCRIBE(const std::string& key, const std::string& value);
 */
    /////////client server//////////////
    NFRedisResult* FLUSHALL();
    NFRedisResult* FLUSHDB();

private:


    bool GetStatusReply();
    bool GetIntReply();
    bool GetBulkReply();
    bool GetArrayReply();


private:
    NFRedisClientSocket* m_pRedisClientSocket;
    NFRedisResult* m_pRedisResult;
};


#endif //NFREDISPLUGIN_NFREDISCLIENT_H
