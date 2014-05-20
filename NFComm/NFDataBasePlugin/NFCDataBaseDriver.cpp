// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseDriver.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2010-12-19 11:05
//    @Module           :    NFCDataBaseDriver
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCDataBaseDriver.h"

int NFCDataBaseDriver::OTLConnect(const std::string& strUID, const std::string& strPWD, const std::string& strDSN, otl_connect& otlDB)
{
    const char* pszUID = strUID.c_str();
    const char* pszPWD = strPWD.c_str();
    const char* pszDSN = strDSN.c_str();

    char pszConnStr[MAX_PATH] = { 0 };
    sprintf_s(pszConnStr, "UID=%s;PWD=%s;DSN=%s;", pszUID, pszPWD, pszDSN);

    try
    {
        otlDB.rlogon(pszConnStr, 1);
        printf("NFDataBase->CONNECT: OK!\n%s\n", pszConnStr);
    }
    catch (otl_exception& p)    // intercept OTL exceptions
    {
        "NFDataBase->Connect Error: (%s) (%s) (%s)\n",
        cerr << p.msg << endl;  // print out error message
        cerr << p.stm_text << endl; // print out SQL that caused the error
        cerr << p.sqlstate << endl; // print out SQLSTATE message
        cerr << p.var_info << endl; // print out the variable that caused the error
    }
    return 0;
}

int NFCDataBaseDriver::OTLDisconnect(otl_connect& otlDB)
{
    //关闭数据库之前需要提交事务，如果是自动提交，则无须要这里写
    //OtlDB.commit ( );
    otlDB.logoff();

    printf("NFDataBase->DISCONNECT: OK!\n");
    return 0;
}

int NFCDataBaseDriver::OTLExec(const char* pstrSqlInfo, otl_connect& otlDB)
{
    int nRet = 0;
    try
    {
        nRet = otl_cursor::direct_exec(otlDB, pstrSqlInfo);
        //printf( "OTLExec:%s\n", pstrSqlInfo );

    }
    catch (otl_exception& p)    // intercept OTL exceptions
    {
        "OTLExec Error: (%s) (%s) (%s)\n",
        cerr << p.msg << endl;  // print out error message
        cerr << p.stm_text << endl; // print out SQL that caused the error
        cerr << p.sqlstate << endl; // print out SQLSTATE message
        cerr << p.var_info << endl; // print out the variable that caused the error
    }
    return nRet;
}
