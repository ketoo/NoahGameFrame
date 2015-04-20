// -------------------------------------------------------------------------
//    @FileName         :    NFIDataProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIDataProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATABASE_MODULE_H_
#define _NFI_DATABASE_MODULE_H_

#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

/*
    A user's attributes and the length of the table is fixed in this module,
    and all of the data of the user only attribute data and table data.
*/
class NFIDataBaseModule
    : public NFILogicModule
{
public:

    //int& nState:0为正确可登录，>0为其他各种错误,比如封号等
    virtual int ConfirmAccountInfo(const std::string& strAccount, const std::string& strPassword) = 0;

    virtual int HasAccount(const std::string& strAccount) = 0;

    virtual int CreateAccount(const std::string& strAccount, const std::string& strPassword) = 0;

    virtual int HasRole(const std::string& strRoleName) = 0;

    virtual int CreateRole(const std::string& strAccount, const std::string& strRoleName) = 0;
    virtual int DeleteRole(const std::string& strAccount, const std::string& strRoleName) = 0;

    virtual int QueryAccountProperty(const std::string& strAccount, NFIValueList& value) = 0;
    virtual int QueryAccountRecord(const std::string& strAccount, NFIValueList& value) = 0;
    virtual int QueryAccountRoleList(const std::string& strAccount, NFIValueList& value) = 0;
    virtual int QueryRoleProperty(const std::string& strRoleName, NFIValueList& value) = 0;
    virtual int QueryRoleRecord(const std::string& strRoleName, NFIValueList& value) = 0;

    virtual int SetAccountProperty(const std::string& strAccount, const NFIValueList& value) = 0;
    virtual int SetAccountRecord(const std::string& strAccount, const NFIValueList& value) = 0;
    virtual int SetRoleProperty(const std::string& strRoleName, const NFIValueList& value) = 0;
    virtual int SetRoleRecord(const std::string& strRoleName, const NFIValueList& value) = 0;
};

#endif