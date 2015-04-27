
// -------------------------------------------------------------------------
//    @FileName         :    NFCMysqlConnectMgrModule.h
//    @Author           :    eliteYang
//    @Date             :    2014-11-10
//    @Module           :    NFCMysqlConnectMgrModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_MYSQL_CONNECT_MANAGER_MODULE_H_
#define _NFC_MYSQL_CONNECT_MANAGER_MODULE_H_

#include "NFComm/NFPluginModule/NFIMysqlConnectMgrModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriver.h"

class NFCMysqlConnectMgrModule : public NFIMysqlConnectMgrModule
{
public:
    NFCMysqlConnectMgrModule(NFIPluginManager* p);
    virtual ~NFCMysqlConnectMgrModule();
        
    virtual bool Init();
    virtual bool Shut();

    virtual bool BeforeShut();
    virtual bool AfterInit();

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);


    virtual bool AddMysqlServer( const int nServerID, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd);
     
	virtual NFIMysqlDriver* GetMysqlDriver();

protected:
	void CheckMysql(float fLastFrameTime, const float fStartedTime);
    std::string GetIPByHostName(const std::string& strHostName);

	NFMap<int, NFIMysqlDriver> mvMysql;
	NFMap<int, NFIMysqlDriver> mvInvalidMsyql;
	float mfLastCheckTime;
};

#endif // !_NFC_MYSQL_CONNECT_MANAGER_MODULE_H_