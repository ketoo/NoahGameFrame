// -------------------------------------------------------------------------
//    @FileName			:    NFCNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFCNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_DATANOSQL_MODULE_H
#define NFC_DATANOSQL_MODULE_H

#include "NFCNoSqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCNoSqlModule
    : public NFINoSqlModule
{
public:

    NFCNoSqlModule(NFIPluginManager* p);
    virtual ~NFCNoSqlModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP);
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort);
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass);

	virtual NFList<std::string> GetDriverIdList();
	virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriver(const std::string& strID);
	virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuitRandom();
	virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuitConsistent();
	virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuit(const std::string& strHash);
	//virtual NF_SHARE_PTR<NFINoSqlDriver> GetDriverBySuit(const int nHash);
    virtual bool RemoveConnectSql(const std::string& strID);

protected:
	NFINT64 mLastCheckTime;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFILogModule* m_pLogModule;

	NFCConsistentHashMapEx<std::string, NFINoSqlDriver> mxNoSqlDriver;

};

#endif