// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlDriver.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCDataNoSqlDriver
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCDataNoSqlDriver.h"

NFCDataNoSqlDriver::NFCDataNoSqlDriver()
{
    m_pRedisClient = NULL;
}

NFCDataNoSqlDriver::~NFCDataNoSqlDriver()
{
    if (m_pRedisClient)
    {
        delete m_pRedisClient;
        m_pRedisClient = NULL;
    }
}

int NFCDataNoSqlDriver::Connect(const std::string& strDSN)
{
    m_pRedisClient = new redis::client(strDSN);

    return 1;
}

int NFCDataNoSqlDriver::Del(const std::string& strKey)
{
    return (int)m_pRedisClient->del(strKey);
}

int NFCDataNoSqlDriver::Exists(const std::string& strKey)
{
    return (int)m_pRedisClient->exists(strKey);
}

int NFCDataNoSqlDriver::Expired(const std::string& strKey, unsigned int nSecs)
{
    m_pRedisClient->expire(strKey, nSecs);
    return 0;
}

int NFCDataNoSqlDriver::Set(const std::string& strKey, const std::string& strValue)
{
    if (strKey.length() > 0)
    {
        m_pRedisClient->set(strKey, strValue);

        return 1;
    }

    return 0;
}

int NFCDataNoSqlDriver::Get(const std::string& strKey, std::string& strValue)
{
    strValue = m_pRedisClient->get(strKey);

    return 1;
}

int NFCDataNoSqlDriver::HSet(const std::string& strKey, const std::string& key, const std::string& value)
{
    if (strKey.length() > 0
        && key.length() > 0)
    {
        return (int)m_pRedisClient->hset(strKey, key, value);
    }

    return 0;
}

int NFCDataNoSqlDriver::HGet(const std::string& strKey, const std::string& key, std::string& value)
{
    value = m_pRedisClient->hget(strKey, key);

    return 1;
}

int NFCDataNoSqlDriver::HGetAll(const std::string& strKey, std::vector<std::pair<std::string, std::string>>& value)
{
    m_pRedisClient->hgetall(strKey, value);

    return value.size();
}

int NFCDataNoSqlDriver::HMGet(const std::string& strKey, const std::vector<std::string>& keys, std::vector<std::string>& values)
{
    m_pRedisClient->hmget(strKey, keys, values);

    return values.size();
}

int NFCDataNoSqlDriver::HMSet(const std::string& strKey, const std::vector<std::string>& keys, const std::vector<std::string>& values)
{
    if (keys.size() != values.size())
    {
        return 0;
    }

    m_pRedisClient->hmset(strKey, keys, values);

    return keys.size();
}

int NFCDataNoSqlDriver::HKeys(const std::string& strKey, std::vector<std::string>& value)
{
    m_pRedisClient->hkeys(strKey, value);
    return value.size();
}

int NFCDataNoSqlDriver::HValues(const std::string& strKey, std::vector<std::string>& value)
{
    m_pRedisClient->hvals(strKey, value);
    return value.size();
}

int NFCDataNoSqlDriver::SAdd(const std::string& strKey, const std::string& value)
{
    return (int)m_pRedisClient->sadd(strKey, value);
}

int NFCDataNoSqlDriver::SMembers(const std::string& strKey, std::set<std::string>& value)
{
    m_pRedisClient->smembers(strKey, value);

    return value.size();
}

int NFCDataNoSqlDriver::SIsMember(const std::string& strKey, const std::string& value)
{
    return (int)m_pRedisClient->sismember(strKey, value);
}

int NFCDataNoSqlDriver::SRemove(const std::string& strKey, const std::vector<std::string>& value)
{
    std::vector<std::string>::const_iterator it = value.begin();
    for (; it != value.end(); ++it)
    {
        m_pRedisClient->srem(strKey, *it);
    }

    return 1;
}

int NFCDataNoSqlDriver::LPush(const std::string& strKey, const std::string& value)
{
    return m_pRedisClient->lpush(strKey, value);
}

int NFCDataNoSqlDriver::LSet(const std::string& strKey, int nIndex, const std::string& value)
{
    if (strKey.length() > 0)
    {
        m_pRedisClient->lset(strKey, nIndex, value);
    }

    return 1;
}

int NFCDataNoSqlDriver::LIndex(const std::string& strKey, const int nIndex, std::string& value)
{
    value = m_pRedisClient->lindex(strKey, nIndex);
    return 1;
}

int NFCDataNoSqlDriver::LLen(const std::string& strKey)
{
    return m_pRedisClient->llen(strKey);
}

int NFCDataNoSqlDriver::LRemove(const std::string& strKey, const std::string& strValue)
{
    return m_pRedisClient->lrem(strKey, -1, strValue);
}

void NFCDataNoSqlDriver::Save()
{
    m_pRedisClient->save();
}

