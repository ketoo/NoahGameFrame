// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlDriver.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCNoSqlDriver
//
// -------------------------------------------------------------------------

#include "NFCNoSqlDriver.h"

#define  REDIS_CATCH(function, line)     catch(redis::connection_error er)\
{\
    mbEnable = false;\
    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
    return false;\
}\
catch(redis::timeout_error er)\
{\
    mbEnable = false;\
    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
    return false;\
}\
catch(redis::protocol_error er)\
{\
    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
    return false;\
}\
catch(redis::key_error er)\
{\
    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
    return false;\
}\
catch(redis::value_error er)\
{\
    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
    return false;\
}\
catch (...)\
{\
    return false;\
}

//catch(boost::bad_lexical_cast er)\
//{\
//    std::cout<< "Redis Error:"<< er.what() << " Function:" << function << " Line:" << line << std::endl;\
//    return false;\
//}\

NFCNoSqlDriver::NFCNoSqlDriver()
{
	mbEnable = false;
	m_pRedisClient = NULL;
}

NFCNoSqlDriver::~NFCNoSqlDriver()
{
}

const bool NFCNoSqlDriver::Connect(const std::string & strDns, const int nPort, const std::string & strAuthKey)
{
	try
	{
		if (m_pRedisClient)
		{
			delete m_pRedisClient;
			m_pRedisClient = NULL;
		}

		m_pRedisClient = new redis::client(strDns);

		mbEnable = true;
	}
	catch (...)
	{
		mbEnable = false;
	}

	return mbEnable;
}

const bool NFCNoSqlDriver::Enable()
{
	return mbEnable;
}

const std::string & NFCNoSqlDriver::GetIP()
{
	// TODO: insert return statement here

	return NULL_STR;
}

const int NFCNoSqlDriver::GetPort()
{
	return 0;
}

const std::string & NFCNoSqlDriver::GetAuthKey()
{
	// TODO: insert return statement here

	return NULL_STR;
}

const bool NFCNoSqlDriver::Del(const std::string & strKey)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		return m_pRedisClient->del(strKey);
	}

	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::Exists(const std::string & strKey)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		return m_pRedisClient->exists(strKey);
	}

	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::Expire(const std::string & strKey, unsigned int nSecs)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->expire(strKey, nSecs);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::Expireat(const std::string & strKey, unsigned int nUnixTime)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		//m_pRedisClient->expire(strKey, nUnixTime);
		//return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::Set(const std::string & strKey, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->set(strKey, strValue);
		return true;
	}

	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::Get(const std::string & strKey, std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		strValue = m_pRedisClient->get(strKey);
		return true;
	}

	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::SetNX(const std::string & strKey, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		return m_pRedisClient->setnx(strKey, strValue);
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::SetEX(const std::string & strKey, const std::string & strValue, const unsigned int nSeconds)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->setex(strKey, strValue, nSeconds);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HSet(const std::string & strKey, const std::string & strField, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	if (strKey.empty() || strField.empty())
	{
		return false;
	}
	try
	{
		m_pRedisClient->hset(strKey, strField, strValue);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HGet(const std::string & strKey, const std::string & strField, std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		strValue = m_pRedisClient->hget(strKey, strField);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::HMSet(const std::string & strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	if (!Enable())
	{
		return false;
	}

	if (strKey.empty() <= 0 || fieldVec.size() != valueVec.size())
	{
		return false;
	}

	try
	{
		m_pRedisClient->hmset(strKey, fieldVec, valueVec);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HMGet(const std::string & strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->hmget(strKey, fieldVec, valueVec);

		return fieldVec.size() == valueVec.size();
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);


	return false;
}

const bool NFCNoSqlDriver::HExists(const std::string & strKey, const std::string & strField)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		return m_pRedisClient->hexists(strKey, strField);
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HDel(const std::string & strKey, const std::string & strField)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		return m_pRedisClient->hdel(strKey, strField);
	}

	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HLength(const std::string & strKey, int & nLen)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nLen = m_pRedisClient->hlen(strKey);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::HKeys(const std::string & strKey, std::vector<std::string>& fieldVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->hkeys(strKey, fieldVec);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HValues(const std::string & strKey, std::vector<std::string>& valueVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->hvals(strKey, valueVec);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::HGetAll(const std::string & strKey, std::vector<std::pair<std::string, std::string>>& valueVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->hgetall(strKey, valueVec);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ZAdd(const std::string & strKey, const double nScore, const std::string & strData)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zadd(strKey, nScore, strData);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZIncrBy(const std::string & strKey, const std::string & strMember, const int nIncrement)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zincrby(strKey, strMember, nIncrement);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZRem(const std::string & strKey, const std::string & strMember)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zrem(strKey, strMember);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZRemRangeByRank(const std::string & strKey, const int nStart, const int nStop)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zremrangebyrank(strKey, nStart, nStop);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ZRemRangeByScore(const std::string & strKey, const int nMin, const int nMax)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zremrangebyscore(strKey, nMin, nMax);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ZScore(const std::string & strKey, const std::string & strMember, int & nScore)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nScore = m_pRedisClient->zscore(strKey, strMember);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ZCard(const std::string & strKey, int & nCount)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nCount = m_pRedisClient->zcard(strKey);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ZCount(const std::string & strKey, const int nMin, const int nMax, int & nCount)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nCount = m_pRedisClient->zcount(strKey, nMin, nMax);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZRevRange(const std::string & strKey, const int nStart, const int nStop, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zrevrange(strKey, nStart, nStop, memberScoreVec);
		return true;

	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZRangeByScore(const std::string & strKey, const int nMin, const int nMax, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->zrangebyscore(strKey, (double)nMin, (double)nMax, memberScoreVec);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ZRevRank(const std::string & strKey, const std::string & strMember, int & nRank)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nRank = m_pRedisClient->zrevrank(strKey, strMember);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ListPush(const std::string & strKey, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}
	try
	{

		m_pRedisClient->rpush(strKey, strValue);
		return true;

	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ListPop(const std::string & strKey, std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{

		strValue = m_pRedisClient->rpop(strKey);
		return true;

	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ListRange(const std::string & strKey, const int nStar, const int nEnd, std::vector<std::string>& xList)
{
	if (!Enable())
	{
		return false;
	}

	try
	{

		m_pRedisClient->lrange(strKey, nStar, nEnd, xList);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}

const bool NFCNoSqlDriver::ListLen(const std::string & strKey, int & nLength)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		nLength = m_pRedisClient->llen(strKey);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ListIndex(const std::string & strKey, const int nIndex, std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		strValue = m_pRedisClient->lindex(strKey, nIndex);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);


	return false;
}

const bool NFCNoSqlDriver::ListRem(const std::string & strKey, const int nCount, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{

		m_pRedisClient->lrem(strKey, nCount, strValue);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ListSet(const std::string & strKey, const int nCount, const std::string & strValue)
{
	if (!Enable())
	{
		return false;
	}

	try
	{

		m_pRedisClient->lset(strKey, nCount, strValue);
		return true;

	}
	REDIS_CATCH(__FUNCTION__, __LINE__);

	return false;
}

const bool NFCNoSqlDriver::ListTrim(const std::string & strKey, const int nStar, const int nEnd)
{
	if (!Enable())
	{
		return false;
	}

	try
	{
		m_pRedisClient->ltrim(strKey, nStar, nEnd);
		return true;
	}
	REDIS_CATCH(__FUNCTION__, __LINE__);
	return false;
}
