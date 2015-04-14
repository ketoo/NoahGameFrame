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
#include "NFComm/NFCore/NFPlatform.h"

#define OTL_ODBC_MYSQL // ���� OTL 4.0/ODBC
#define OTL_BIGINT unsigned __int64
#include "otlv4.h" // ���� OTL 4.0 ͷ�ļ�

using namespace std;

class  NFCMysqlDriver
{
public:

    //--------------------------------------------------------------------------
    // �������ƣ�
    //      OTLConnect
    // ������
    //      const std::string& strUID
    //      const std::string& strPWD
    //      const std::string& strDSN
    //      otl_connect& otlDB
    // ���أ�
    //      int                 : Ŀǰû��
    // ˵����
    //      ����DB
    //---------------------------------------------------------------------------
    int OTLConnect(const std::string& strUID, const std::string& strPWD, const std::string& strDSN, otl_connect& otlDB);

    //--------------------------------------------------------------------------
    // �������ƣ�
    //      OTLDisconnect
    // ������
    //      otl_connect& otlDB
    // ���أ�
    //      int                 : Ŀǰû��
    // ˵����
    //      �Ͽ�DB
    //---------------------------------------------------------------------------
    int OTLDisconnect(otl_connect& otlDB);

    //--------------------------------------------------------------------------
    // �������ƣ�
    //      OTLDisconnect
    // ������
    //      const char* pstrSqlInfo
    //      otl_connect& otlDB
    // ���أ�
    //      int                     : Ŀǰû��
    // ˵����
    //      ִ��SQL���,ֻ���������ʵ�insert updata���������
    //---------------------------------------------------------------------------
    int OTLExec(const char* pstrSqlInfo, otl_connect& otlDB);
protected:

private:

};

#endif