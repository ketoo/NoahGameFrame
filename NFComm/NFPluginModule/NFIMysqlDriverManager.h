
// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlDriverManager.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFIMysqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFI_MYSQL_DRIVER_MANAGER_H
#define NFI_MYSQL_DRIVER_MANAGER_H

#include "NFComm/NFPluginModule/NFIMysqlDriver.h"

class NFIMysqlDriverManager
{
public:
    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/) = 0;
    virtual NFIMysqlDriver* GetMysqlDriver() = 0;
    virtual void CheckMysql() = 0;
};

#endif // !NFI_MYSQL_DRIVER_MANAGER_H