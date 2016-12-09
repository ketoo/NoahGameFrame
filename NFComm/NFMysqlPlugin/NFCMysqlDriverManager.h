
// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlDriverManager.h
//    @Author           :    Chuanbo.Guo
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlDriverManager
//
// -------------------------------------------------------------------------

#ifndef NFC_MYSQL_DRIVER_MANAGER_H
#define NFC_MYSQL_DRIVER_MANAGER_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIMysqlDriver.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"

class NFCMysqlDriverManager : public NFIMysqlDriverManager
{
public:
	virtual ~NFCMysqlDriverManager();

    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/);
    virtual NFIMysqlDriver* GetMysqlDriver();
    virtual void CheckMysql();

protected:
    std::string GetIPByHostName(const std::string& strHostName);

    NFMap<int, NFIMysqlDriver> mvMysql;
    NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
    NFINT64 mnLastCheckTime;
};

#endif // !_NFC_MYSQL_CONNECT_MANAGER_MODULE_H