// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlConnectMgrModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFIMysqlConnectMgrModule
// -------------------------------------------------------------------------

#ifndef _NFI_MYSQL_CONNECT_MANAGER_MODULE_H_
#define _NFI_MYSQL_CONNECT_MANAGER_MODULE_H_

#include "NFILogicModule.h"
#include "NFIMysqlDriver.h"

class NFIMysqlConnectMgrModule : public NFILogicModule
{
public:
    virtual bool ConnectNewMysql() = 0;
    
    virtual bool AddMysqlServer( const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, 
        const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

	virtual NFIMysqlDriver* GetMysqlDriver() = 0;
};

#endif // !_NFI_MYSQL_CONNECT_MANAGER_MODULE_H_