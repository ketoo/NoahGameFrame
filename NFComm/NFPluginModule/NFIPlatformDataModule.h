// -------------------------------------------------------------------------
//    @FileName         :    NFIDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFIDataNoSqlModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PLATFORM_DATA_MODULE_H_
#define _NFI_PLATFORM_DATA_MODULE_H_

#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"

/*
    A user's attributes and the length of the table is fixed in this module,
    and all of the data of the user only attribute data and table data.
*/
class NFIPlatformDataModule
    : public NFILogicModule
{
public:

    //account center
    virtual int AddAccountInfo(const std::string& strAccount, const std::string& strPassword) = 0;

    //int& nState:0为正确可登录，>0为其他各种错误,比如封号等
    virtual int ConfirmAccountInfo(const std::string& strAccount, const std::string& strPassword) = 0;

	virtual int SetAccountState(const std::string& strAccount, const int nState) = 0;

};

#endif