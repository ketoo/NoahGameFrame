// -------------------------------------------------------------------------
//    @FileName         :    NFIDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFIDataNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATANOSQL_MODULE_H_
#define _NFI_DATANOSQL_MODULE_H_

#include <set>
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

class NFIDataNoSqlDriver
{
public:

    virtual int Connect(const std::string& strDSN) = 0;

    //key
    virtual int Del(const std::string& strKey) = 0;
    virtual int Exists(const std::string& strKey) = 0;

    //element
    virtual int Set(const std::string& strKey, const std::string& strValue) = 0;
    virtual int Get(const std::string& strKey, std::string& strValue) = 0;

    //list
    virtual int LPush(const std::string& strKey, const std::string& value) = 0;
    virtual int LSet(const std::string& strKey, int nIndex, const std::string& value) = 0;
    virtual int LIndex(const std::string& strKey, const int nIndex, std::string& value) = 0;
    virtual int LLen(const std::string& strKey) = 0;
    virtual int LRemove(const std::string& strKey, const std::string& strValue) = 0;

    //hash
    virtual int HSet(const std::string& strKey, const std::string& key, const std::string& value) = 0;
    virtual int HGet(const std::string& strKey, const std::string& key, std::string& strValue) = 0;

    virtual int HGetAll(const std::string& strKey,  std::vector<std::pair<std::string, std::string>>& value) = 0;
    virtual int HMGet(const std::string& strKey, const std::vector<std::string>& keys, std::vector<std::string>& values) = 0;
    virtual int HMSet(const std::string& strKey, const std::vector<std::string>& keys, const std::vector<std::string>& values) = 0;

    virtual int HKeys(const std::string& strKey, std::vector<std::string>& value) = 0;
    virtual int HValues(const std::string& strKey, std::vector<std::string>& value) = 0;

    //set
    virtual int SAdd(const std::string& strKey, const std::string& value) = 0;
    virtual int SMembers(const std::string& strKey, std::set<std::string>& value) = 0;
    virtual int SIsMember(const std::string& strKey, const std::string& value) = 0;
    virtual int SRemove(const std::string& strKey, const std::vector<std::string>& value) = 0;

    virtual void Save() = 0;
};

class NFIDataNoSqlModule
    : public NFILogicModule
{
public:

    virtual NFIDataNoSqlDriver* GetDriver() = 0;
    virtual bool ConnectSql(const std::string& strIP) = 0;

};

#endif