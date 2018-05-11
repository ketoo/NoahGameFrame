#ifndef NFC_REDIS_IMPL_H
#define NFC_REDIS_IMPL_H

#include <vector>
#include <set>
#include <thread>

#include "NFRedisCluster.h"


#define _TRY_BEGIN_			try{
#define _TRY_END_(STR_KEY)   	}catch (int nError)\
							{\
								printf("redis exception, key:[%s],fun:[%s],error:[%d] ", STR_KEY.c_str(),__FUNCTION__, nError);\
							}\
							catch (char *pError)\
							{\
								printf("redis exception, key:[%s],fun:[%s],error:[%s]", STR_KEY.c_str(), __FUNCTION__, pError);\
							}\
							catch (...)\
							{\
								printf("redis exception, key:[%s],fun:[%s]", STR_KEY.c_str(), __FUNCTION__);\
							}\



using VecZRangeScoreValue = std::vector<std::pair<NFINT64, std::string>>;

#pragma pack(push,1)
struct NFZData
{
	NFINT64 nScore;
	std::string strContent;
};

struct NFGeoRediusInfo
{
	std::string strMember;
	double fDistance;
	struct stPosion
	{
		double longitude;
		double latitude;
	}vPosion;
};
#pragma pack(pop)


class NFCRedisImpl : public NFCRedisCluster
{	

public:
	NFCRedisImpl(std::thread::id threadID);
	~NFCRedisImpl();

	// SET,GET
	int SetValueByBinary(const std::string& strKey, const char* buf, size_t buf_size);
	int SetValueByString(const std::string& strKey, const std::string& strContent);
	int SetValueInt(const std::string& strKey, NFINT64 nValue);
	int GetValue(const std::string& strKey, char* buff, int buff_size, int& real_size);
	long long GetValueInt(const std::string& strKey);
	std::string GetSetValue(const std::string& strKey, const std::string& szValue);
	NFINT64 GetSetValueInt(const std::string& strKey, NFINT64 szValue);

	// exist
	bool Exist(const std::string& strKey);

	// DEL
	bool RemoveKey(const std::string& strKey);

	// incr
	NFUINT64 GetIncrNumber(const std::string& strKey);
	NFUINT64 GetDecrNumber(const std::string& strKey);
	NFINT64 GetIncrByNumber(const std::string& strKey, NFINT64 n);

	//-------------------SortedSet------------------------
	// zadd
	bool ZaddScore(const std::string& strKey, NFINT64 nScore, const std::string& member);
	void ZsetValueByBinary(const std::string& strKey, NFINT64 nScore, const std::string& member);
	void ZsetValue(const std::string& strKey, const NFINT64 nScore, const std::string& strName);
	// ZRANGEBYSCORE
	std::string ZgetValue(const std::string& strKey, NFINT64 nScore);
	// ZSCORE
	NFINT64 ZgetScore(const std::string& strKey, const std::string& member);
	// ZRANGE WITHSCORES
	void ZCommonRange(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd, VecZRangeScoreValue& vecValue);
	// ZRANGE
	size_t ZCommonRangeWithoutScore(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd, std::string* szContent, size_t size);
	// ZCARD
	int ZgetCount(const std::string& strKey);
	// ZSCORE + ZREM + ZADD
	int ZMondifyValue(const std::string& strKey, const std::string& strOldName, const std::string& strNewName);
	// ZREM
	int ZDelValue(const std::string& strKey, const std::string& strName);
	// ZINCRBY
	NFINT64 ZIncrScore(const std::string& strKey, const std::string& strName, NFINT64 nScore);
	// ZREMRANGEBYRANK
	int ZRemByRank(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd);
	// ZRANGEBYSCORE inf WITHSCORES LIMIT 0 
	int ZgetRangeByScoreByOpenInterval(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount = -1, NFINT64 nEnd = -1);
	int ZgetRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount, std::string min = "-inf", std::string max = "+inf");
	int ZgetRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, NFINT64 nStart, int nCount = -1, NFINT64 nEnd = -1);
	// ZREVRANGE WITHSCORES
	int ZReverRangeValue(const std::string& strKey, std::vector<NFZData>& vecContent, int nRankStart, int nRankEnd = -1);
	// ZREVRANK
	int ZReverRank(const std::string& strKey, const std::string& strName);
	// ZREM + ZADD
	int ZModifyScore(const std::string& strKey, const std::string& strName, NFINT64 nScore);
	// ZCOUNT
	int ZCount(const std::string& strKey, std::string min = "-inf", std::string max = "+inf");
	//ZREMRANGEBYSCORE
	int ZRemByScore(const std::string& strKey, NFINT64 nStart, NFINT64 nEnd);
	int ZReverRangeByScore(const std::string& strKey, VecZRangeScoreValue& vecValue, int offset = 0, int nCount = -1, std::string max = "+inf", std::string min = "-inf");
	// ZRANGEBYLEX
	int ZRangeLex(const std::string& strKey, const std::string& strMin, const std::string& strMax, int nStart, int nCount, std::array<std::string,200>& arrString);
	// ZLEXCOUNT
	int ZLexCount(const std::string& strKey, const std::string& strMin, const std::string& strMax);
	// ZREMRANGEBYLEX
	int ZRemLex(const std::string& strKey, const std::string& strMin, const std::string& strMax);
	// ZRANK
	int ZRank(const std::string& strKey, const std::string& member);


	//-----------------------List-----------------------------
	// LPUSH
	void InsertToListByBinary(const std::string& strKey, const char* buf, size_t buf_size);
	// RPUSH
	void InsertToRListByBinary(const std::string& strKey, const char* buf, size_t buf_size);
	// RPOP
	bool GetRedisMsg(const std::string& strKey, std::string& strResult);
	// LLEN
	int GetListLen(const std::string& strKey);
	// LREM  0 
	bool DelListData(const std::string& strKey, const std::string& strContent);
	// LRANGE
	bool GetAllListData(const std::string& strKey, std::string* pszContent, int&size, int nBegin = 0, int nCount = -1);
	// LINDEX
	int ListGetByIndex(const std::string& strKey, std::string& strResult, int nIndex);
	// LSET
	int ListModifyValueByIndex(const std::string& strKey, const std::string& strContent, int nIndex);
	// LTRIM
	bool TrimListData(const std::string& strKey, int nBegin = 0, int nCount = -1);
	// BRPOP
	int PopPushListData(const std::string& strKey,std::string& vecContent);
	// LREM
	int RemoveListValue(const std::string& strKey, const std::string& strContent);
	// LPUSHX
	int LPushEx(const std::string& strKey, const std::string& strContent);
	// LPUSHX
	int LPushEx(const std::string& strKey, char* pBuff, int wSize);


	// SETNX, PEXPIRE
	int SetMutexVauleByStr(const std::string& strKey, const std::string& strContent);
	int SetMutexValueByBinary(const std::string& strKey, const char* buf, size_t buf_size);
	int SetMutexValueInt(const std::string& strKey, NFINT64 nID);
	int SetMutexKeyWithTime(const std::string& strKey, const std::string& strContent, NFUINT32 time = 100);
	NFUINT64 SetMutexValueTimeOut(const std::string& strKey, int nTime);

	// EXPIRE, PERSIST, TTL
	NFUINT64 SetKeyTime(const std::string& strKey, NFUINT32 dwExpiredTime);
	int RemoveKeyTime(const std::string& strKey);
	NFUINT64 GetKeyTime(const std::string& strKey);

	// transaction : WATCH, MULTI, EXEC, DISCARD, UNWATCH
	int WatchKey(const std::string& strKey);
	int Multi(const std::string& strKey);
	int Exec(const std::string& strKey);
	int Discard(const std::string& strKey);
	int Unwatch(const std::string& strKey);


	//---------------------------Set---------------------------
	// SADD
	int SAddValueStr(const std::string& strKey, const std::string& strContent);
	int SAddValueByBinary(const std::string& strKey, const char* buf, size_t buf_size);
	int SAddValueInt(const std::string& strKey, NFINT64 nID);
	// SMEMBERS
	int SMemberValue(const std::string& strKey, std::string *pszContent, int& nSize);
	// SISMEMBER
	bool SisMember(const std::string& strKey, const std::string& strContent);
	// SREM
	int SremMember(const std::string& strKey, const std::string& strContent);
	// SPOP
	bool SPopValue(const std::string& strKey, std::string& strResult);
	// SCARD
	int SGetCount(const std::string& strKey);
	// SADD


	// PUBLISH, UNSUBSCRIBE
	int PublishStr(const std::string& strKey, const std::string& strContent);
	int PublishBinary(const std::string& strKey, const char* buf, size_t buf_size);
	int Unsubscribe(const std::string& strKey);


	// GEO: GEOADD, GEODIST,GEORADIUS m WITHDIST WITHCOORD COUNT, GEOPOS, GEOHASH
	NFUINT64 GeoAdd(const std::string& strKey, double longitude, double latitude, const std::string& strName);
	double GeoDist(const std::string& strKey, const std::string& strMember_1, const std::string& strMember_2);
	int GeoRedius(const std::string& strKey, double longitude, double latitude, int dist, std::vector<NFGeoRediusInfo>& vGeoInfo,int nMaxCount = 200);
	int GeoPos(const std::string& strKey, const std::string& strMember, NFGeoRediusInfo::stPosion& vPos);
	int GeoHashCode(const std::string& strKey, const std::string& strMember, std::string& sResult);


	//------------------------Hash---------------------
	// HSET
	int HAddValue( const std::string& strKey, const std::string& szFiled, const std::string& strContent );
	// HGET
	std::string GetHValueByFiled( const std::string& strKey, const std::string& szFiled );
	// HDEL
	int HDelValueByFiled( const std::string& strKey , const std::vector<std::string>& vecFiled );
	int HDelValueByFiled( const std::string& strKey , const std::string& vecFiled );

	// TYPE
	std::string GetKeyType(const std::string& strKey);

	bool exeCmdPipe(std::string& szCMD, std::vector<std::pair<std::string, std::string>>& vecKeyValue);

public:

	void GetAllClient(std::list<RedisClientPtr>& vecClient );
	void GetSingleAllKeys(RedisClientPtr ptrClient,const std::string& pattan,std::list<std::string>& szKeys);

private:

	std::thread::id  _threadID;
};



#endif