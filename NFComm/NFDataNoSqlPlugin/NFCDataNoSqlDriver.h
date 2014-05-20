// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseDriver.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCDataBaseDriver
//
// -------------------------------------------------------------------------

#ifndef _NFC_DATABASEDRIVER_H_
#define _NFC_DATABASEDRIVER_H_

// #include <boost/foreach.hpp>
// #include <boost/algorithm/string.hpp>
#include "Dependencies/redis-cplusplus-client/redisclient.h"

class  NFCDataNoSqlDriver
{
public:
    NFCDataNoSqlDriver();
    virtual ~NFCDataNoSqlDriver();

    int Connect(const std::string& strDSN);

    //key
    int Del(const std::string& strKey);
    int Exists(const std::string& strKey);

    //element
    int Set(const std::string& strKey, const std::string& strValue);
    std::string Get(const std::string& strKey);

    //list
    int LPush(const std::string& strKey, const std::string& value);
    int LSet(const std::string& strKey, int nIndex, const std::string& value);
    std::string LIndex(const std::string& strKey, const int nIndex);
    int LLen(const std::string& strKey);
    int LRemove(const std::string& strKey, const std::string& strValue);

    //hash
    int HSet(const std::string& strKey, const std::string& key, const std::string& value);
    std::string HGet(const std::string& strKey, const std::string& key);

    int HGetAll(const std::string& strKey,  redis::client::string_pair_vector& value);
    int HMGet(const std::string& strKey, const redis::client::string_vector& keys, redis::client::string_vector& values);
    int HMSet(const std::string& strKey, const redis::client::string_vector& keys, redis::client::string_vector& values);

    int HKeys(const std::string& strKey, redis::client::string_vector& value);
    int HValues(const std::string& strKey, redis::client::string_vector& value);

    //set
    int SAdd(const std::string& strKey, const std::string& value);
    int SMembers(const std::string& strKey, redis::client::string_set& value);
    int SIsMember(const std::string& strKey, const std::string& value);
    int SRemove(const std::string& strKey, const redis::client::string_vector& value);

    void Save();

protected:

private:
	//boost::shared_ptr<redis::client> mRedisClient;
	redis::client* m_pRedisClient;
};

#endif