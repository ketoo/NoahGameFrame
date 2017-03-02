// -------------------------------------------------------------------------
//    @FileName         :    NFINoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFINoSqlModule
//
// -------------------------------------------------------------------------

#ifndef NFI_NOSQL_MODULE_H
#define NFI_NOSQL_MODULE_H

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFINoSqlDriver.h"


class NFINoSqlModule
	: public NFIModule
{
public:

	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP) = 0;
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort) = 0;
	virtual bool AddConnectSql(const std::string& strID, const std::string& strIP, const int nPort, const std::string& strPass) = 0;

	virtual NFList<std::string> GetDriverIdList() = 0;
	virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriver(const std::string& strID) = 0;
	virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuitRandom() = 0;
	virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuitConsistent() = 0;
	virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuit(const std::string& strHash) = 0;
	//virtual NF_SHARE_PTR<NFINoSqlDriver>  GetDriverBySuit(const int nHash) = 0;
	virtual bool RemoveConnectSql(const std::string& strID) = 0;
};

#endif