
// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlConnectMgrModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlConnectMgrModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MYSQL_CONNECT_MANAGER_MODULE_H
#define NFC_MYSQL_CONNECT_MANAGER_MODULE_H

#include "NFComm/NFPluginModule/NFIMysqlConnectMgrModule.h"

class NFCMysqlConnectMgrModule : public NFIMysqlConnectMgrModule
{
public:
    NFCMysqlConnectMgrModule(NFIPluginManager* p);
    virtual ~NFCMysqlConnectMgrModule();
        
    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute();


    virtual bool AddMysqlServer( const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);
     
	virtual NFIMysqlDriver* GetMysqlDriver();

protected:
	void CheckMysql();
    std::string GetIPByHostName(const std::string& strHostName);

	NFMap<int, NFIMysqlDriver> mvMysql;
	NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
	NFINT64 mnLastCheckTime;
};

#endif // !_NFC_MYSQL_CONNECT_MANAGER_MODULE_H