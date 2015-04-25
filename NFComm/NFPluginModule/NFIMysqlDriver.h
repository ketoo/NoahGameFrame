// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseDriver.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCDataBaseDriver
//
// -------------------------------------------------------------------------

#ifndef _NFC_DATABASEDRIVER_H_
#define _NFC_DATABASEDRIVER_H_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <list>
#include "NFComm/NFPluginModule/NFPlatform.h"

#define OTL_ODBC_MYSQL // 编译 OTL 4.0/ODBC
#define OTL_BIGINT unsigned __int64
#include "otlv4.h" // 包含 OTL 4.0 头文件

using namespace std;

class  NFIMysqlDriver
{
public:

    //--------------------------------------------------------------------------
    // 函数名称：
    //      OTLConnect
    // 参数：
    //      const std::string& strUID
    //      const std::string& strPWD
    //      const std::string& strDSN
    //      otl_connect& otlDB
    // 返回：
    //      int                 : 目前没用
    // 说明：
    //      连接DB
    //---------------------------------------------------------------------------
    virtual int OTLConnect(const std::string& strUID, const std::string& strPWD, const std::string& strDSN, otl_connect& otlDB) = 0;

    //--------------------------------------------------------------------------
    // 函数名称：
    //      OTLDisconnect
    // 参数：
    //      otl_connect& otlDB
    // 返回：
    //      int                 : 目前没用
    // 说明：
    //      断开DB
    //---------------------------------------------------------------------------
    virtual int OTLDisconnect(otl_connect& otlDB) = 0;

    //--------------------------------------------------------------------------
    // 函数名称：
    //      OTLDisconnect
    // 参数：
    //      const char* pstrSqlInfo
    //      otl_connect& otlDB
    // 返回：
    //      int                     : 目前没用
    // 说明：
    //      执行SQL语句,只有命令性质的insert updata才能用这个
    //---------------------------------------------------------------------------
    virtual int OTLExec(const char* pstrSqlInfo, otl_connect& otlDB) = 0;
protected:

private:

};

#endif