
// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlConnectMgrModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlConnectMgrModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MYSQL_DRIVER_MANAGER_H
#define NFC_MYSQL_DRIVER_MANAGER_H
#include "NFComm/NFPluginModule/NFIMysqlDriver.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFIMysqlDriverManager.h"


class NFCMysqlDriverManager : public NFIMysqlDriverManager
{
public:
    virtual bool AddMysqlServer( const int nServerID, const std::string& strDnsIp, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/);
	virtual NFIMysqlDriver* GetMysqlDriver();
    virtual void CheckMysql();

protected:
	NFMap<int, NFIMysqlDriver> mvMysql;
	NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
	NFINT64 mnLastCheckTime;
};

#endif // !_NFC_MYSQL_CONNECT_MANAGER_MODULE_H