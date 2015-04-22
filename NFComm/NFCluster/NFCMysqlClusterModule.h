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

#ifndef _NFC_DATABASE_MODULE_H_
#define _NFC_DATABASE_MODULE_H_

#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"

class NFCMysqlClusterModule
    : public NFIClusterModule
{
public:

    NFCMysqlClusterModule(NFIPluginManager* p);
    virtual ~NFCMysqlClusterModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

	virtual bool Updata(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
	virtual bool Query(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
	virtual bool Select(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

	virtual bool Delete(const std::string& strKey);
	virtual bool Exists(const std::string& strKey, bool& bExit);

	//////////////////////////////////////////////////////////////////////////
	virtual bool Updata(const std::string& strRecord, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
	virtual bool Query(const std::string& strRecord, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
	virtual bool Select(const std::string& strRecord, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

	virtual bool Delete(const std::string& strRecord, const std::string& strKey);
	virtual bool Exists(const std::string& strRecord, const std::string& strKey, bool& bExit);
private:

    NFCMysqlDriver* m_pDataBaseDriver;

    std::string mstrMasterName;
    std::string mstrUID;
    std::string mstrPWD;

};

#endif