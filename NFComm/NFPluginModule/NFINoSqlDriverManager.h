// -------------------------------------------------------------------------
//    @FileName			:    NFINoSqlDriverManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFINoSqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFI_NOSQL_DRIVER_MANAGER_H
#define NFI_NOSQL_DRIVER_MANAGER_H

#include "NFComm/NFPluginModule/NFINoSqlDriver.h"

class NFINoSqlDriverManager
{
public:
    virtual bool AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/) = 0;
    virtual NFINoSqlDriver* GetNoSqlDriver() = 0;
    virtual void CheckMysql() = 0;
};

#endif