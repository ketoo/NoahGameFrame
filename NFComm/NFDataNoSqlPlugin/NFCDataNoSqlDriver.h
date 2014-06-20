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
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"

class  NFCDataNoSqlDriver : public NFIDataNoSqlDriver
{
public:
    NFCDataNoSqlDriver();
    virtual ~NFCDataNoSqlDriver();

    virtual int Connect(const std::string& strDSN);

    //key
    virtual int Del(const std::string& strKey);
    virtual int Exists(const std::string& strKey);

    //element
    virtual int Set(const std::string& strKey, const std::string& strValue);
    virtual int Get(const std::string& strKey, std::string& strValue);

    //list
    virtual int LPush(const std::string& strKey, const std::string& value);
    virtual int LSet(const std::string& strKey, int nIndex, const std::string& value);
    virtual int LIndex(const std::string& strKey, const int nIndex, std::string& value);
    virtual int LLen(const std::string& strKey);
    virtual int LRemove(const std::string& strKey, const std::string& strValue);

    //hash
    virtual int HSet(const std::string& strKey, const std::string& key, const std::string& value);
    virtual int HGet(const std::string& strKey, const std::string& key, std::string& value);

    virtual int HGetAll(const std::string& strKey,  std::vector<std::pair<std::string, std::string>>& value);
    virtual int HMGet(const std::string& strKey, const std::vector<std::string>& keys, std::vector<std::string>& values);
    virtual int HMSet(const std::string& strKey, const std::vector<std::string>& keys, const std::vector<std::string>& values);

    virtual int HKeys(const std::string& strKey, std::vector<std::string>& value);
    virtual int HValues(const std::string& strKey, std::vector<std::string>& value);

    //set
    virtual int SAdd(const std::string& strKey, const std::string& value);
    virtual int SMembers(const std::string& strKey, std::set<std::string>& value);
    virtual int SIsMember(const std::string& strKey, const std::string& value);
    virtual int SRemove(const std::string& strKey, const std::vector<std::string>& value);

    virtual void Save();

protected:

private:
	//boost::shared_ptr<redis::client> mRedisClient;
	redis::client* m_pRedisClient;
};

#endif