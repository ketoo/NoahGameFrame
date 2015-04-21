// -------------------------------------------------------------------------
//    @FileName      :    NFIClusterModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIClusterModule
//
//
// -------------------------------------------------------------------------

#ifndef _NFI_CLUSTER_MODULE_H_
#define _NFI_CLUSTER_MODULE_H_

#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIClusterModule
    : public NFILogicModule
{
public:


	virtual bool Updata(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
	virtual bool Query(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
	//virtual bool Select(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

	virtual bool Delete(const std::string& strKey);
	virtual bool Exists(const std::string& strKey, bool& bExit);

private:

    NFCMysqlDriver* m_pDataBaseDriver;

  
    otl_connect motlConnect;

    std::string mstrMasterName;
    std::string mstrUID;
    std::string mstrPWD;

};

#endif