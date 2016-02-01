// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//
//    数据格式
//      Property:   HP,1;Mp,2;Exp,3;
//          属性,属性值;属性,属性值;属性,属性值;属性,属性值;
//
//      Record:     Friends,20,5;1,2,3,4,5;11,22,33,44,55;
//          表名,表行,表列;行1的值(行列的个数);行2的值(行列的个数);
//
// -------------------------------------------------------------------------

#ifndef NFC_DATABASE_MODULE_H
#define NFC_DATABASE_MODULE_H

#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIMysqlConnectMgrModule.h"

class NFCMysqlClusterModule
    : public NFIClusterModule
{
public:

    NFCMysqlClusterModule(NFIPluginManager* p);
    virtual ~NFCMysqlClusterModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	//////////////////////////////////////////////////////////////////////////
	virtual bool Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
	virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
	virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

	virtual bool Delete(const std::string& strRecordName, const std::string& strKey);
	virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit);
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec);

    //////////////////////////////////////////////////////////////////////////
    virtual bool UpdataWithDriver(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, NFIMysqlDriver* pDriver);
    virtual bool QueryWithDriver(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, NFIMysqlDriver* pDriver);
    virtual bool SelectWithDriver(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, NFIMysqlDriver* pDriver);

    virtual bool DeleteWithDriver(const std::string& strRecordName, const std::string& strKey, NFIMysqlDriver* pDriver);
    virtual bool ExistsWithDriver(const std::string& strRecordName, const std::string& strKey, bool& bExit, NFIMysqlDriver* pDriver);
    virtual bool KeysWithDriver(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec, NFIMysqlDriver* pDriver);

private:
    NFIMysqlConnectMgrModule* m_pMysqlConnectMgrManager;

    const static std::string strDefaultKey;
    const static std::string strDefaultTable;


    // key默认为ID
    // RoleInfo为角色的Property和Record数据，有三列数据 ID, Property, Record
};

#endif