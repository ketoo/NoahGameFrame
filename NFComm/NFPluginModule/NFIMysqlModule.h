// -------------------------------------------------------------------------
//    @FileName      	:    NFIMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIMysqlModule
//
//
// -------------------------------------------------------------------------

#ifndef NFI_MYSQL_MODULE_H
#define NFI_MYSQL_MODULE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFIMysqlModule
    : public NFIModule
{
public:


    //  virtual bool Updata(const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
    //  virtual bool Query(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
    //  virtual bool Select(const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
    //
    //  virtual bool Delete(const std::string& strKey) = 0;
    //  virtual bool Exists(const std::string& strKey, bool& bExit) = 0;
    //     virtual bool Keys(const std::string& strKeyName, std::vector<std::string>& valueVec) = 0;

    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;
    virtual bool Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
    virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
    virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;

    virtual bool Delete(const std::string& strRecordName, const std::string& strKey) = 0;
    virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit) = 0;
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec) = 0;

private:

};
#endif