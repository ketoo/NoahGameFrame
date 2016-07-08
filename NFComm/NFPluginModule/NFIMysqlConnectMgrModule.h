// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlConnectMgrModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFIMysqlConnectMgrModule
// -------------------------------------------------------------------------

#ifndef NFI_MYSQL_CONNECT_MANAGER_MODULE_H
#define NFI_MYSQL_CONNECT_MANAGER_MODULE_H

#include "NFIModule.h"
#include "NFIMysqlDriver.h"

class NFIMysqlConnectMgrModule : public NFIModule
{
public:
    //virtual bool ConnectNewMysql() = 0;

    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort,
                                const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

    virtual NFIMysqlDriver* GetMysqlDriver() = 0;
};

#endif // !NFI_MYSQL_CONNECT_MANAGER_MODULE_H