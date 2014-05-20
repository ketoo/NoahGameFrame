// -------------------------------------------------------------------------
//    @FileName         :    NFIDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFIDataNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATANOSQL_MODULE_H_
#define _NFI_DATANOSQL_MODULE_H_

#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

/*
    A user's attributes and the length of the table is fixed in this module,
    and all of the data of the user only attribute data and table data.
*/
class NFIDataNoSqlModule
    : public NFILogicModule
{
public:

    virtual int ExistRole(const std::string& strRoleName) = 0;
    virtual int CreateRole(const std::string& strAccount, const std::string& strRoleName) = 0;
    virtual int DeleteRole(const std::string& strAccount, const std::string& strRoleName) = 0;

    virtual int QueryAccountProperty(const std::string& strAccount, NFIValueList& valueListKeys, NFIValueList& valueListValues) = 0;
    virtual int QueryAccountRoleList(const std::string& strAccount, NFIValueList& value) = 0;
    
	virtual int QueryRoleProperty(const std::string& strRoleName, NFIValueList& valueListKeys, NFIValueList& valueListValues) = 0;
    virtual int QueryRoleRecord(const std::string& strRoleName, const std::string& strRecordName,  NFIValueList& valueListKeys, NFIValueList& valueListValues) = 0;

    virtual int SetAccountProperty(const std::string& strAccount, const NFIValueList& valueListKeys, const NFIValueList& valueListValues) = 0;
    virtual int SetRoleProperty(const std::string& strRoleName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues) = 0;
    virtual int SetRoleRecord(const std::string& strRoleName, const std::string& strRecordName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues) = 0;

    virtual void SavePlayerData() = 0;

};

#endif