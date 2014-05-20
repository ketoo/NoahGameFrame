// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCDataBaseModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

extern NFIPluginManager* pPluginManager;

NFIShareMemoryModule* NFCDataBaseModule::m_pShareMemoryModule = NULL;

NFCDataBaseModule::NFCDataBaseModule()
{
    mstrDataBaseConfigFile = "DBConfig.cfg";
    m_pDataBaseDriver = NULL;
    m_pDataBaseDriver = new NFCDataBaseDriver();
}

NFCDataBaseModule::~NFCDataBaseModule()
{
    delete m_pDataBaseDriver;
    m_pDataBaseDriver = NULL;
}

bool NFCDataBaseModule::Init()
{
    m_pShareMemoryModule = dynamic_cast<NFIShareMemoryModule*>(pPluginManager->FindModule("NFCShareMemoryModule"));

    otl_connect::otl_initialize();

    bool bLoad = mConfig.Load(mstrDataBaseConfigFile);

    mstrUID = mConfig["UID"].str();
    mstrPWD = mConfig["PWD"].str();
    mstrDSN = mConfig["DSN"].str();
    mstrMasterName = mConfig["MST"].str();

    mstrAccountTableName = mConfig["AccountTBL"].str();
    mstrPlayerTableName = mConfig["RoleTBL"].str();
    mstrGlobalTableName = mConfig["GlobalTBL"].str();


    m_pDataBaseDriver->OTLConnect(mstrUID, mstrPWD, mstrDSN, motlConnect);

    return true;
}

bool NFCDataBaseModule::Shut()
{
    m_pDataBaseDriver->OTLDisconnect(motlConnect);

    return true;
}

int NFCDataBaseModule::ConfirmAccountInfo(const std::string& strAccount, const std::string& strPassword)
{

    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT Count(*) FROM %s	 WHERE Account = '%s' and Password = '%s'", mstrAccountTableName.c_str(), strAccount.c_str(), strPassword.c_str());

    int nCount = 0;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> nCount;
        return nCount;
    }

    return 0;
}

int NFCDataBaseModule::CreateAccount(const std::string& strAccount, const std::string& strPassword)
{
    if (HasAccount(strAccount) <= 0)
    {
        char szExec[MAX_PATH] = { 0 };
        sprintf(szExec, "insert into %s ( Account, Password ) values ( '%s', '%s' )", mstrAccountTableName.c_str(), strAccount.c_str(), strPassword.c_str());

        m_pDataBaseDriver->OTLExec(szExec, motlConnect);
    }

    return -1;
}

int NFCDataBaseModule::HasAccount(const std::string& strAccount)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "select count(*) from %s where Account = '%s'", mstrAccountTableName.c_str(), strAccount.c_str());

    int nCount = 0;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> nCount;
        return nCount;
    }

    return 0;
}

int NFCDataBaseModule::HasRole(const std::string& strRoleName)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "select count(*) from %s where RoleName = '%s'", mstrPlayerTableName.c_str(), strRoleName.c_str());

    int nCount = 0;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> nCount;
        return nCount;
    }

    return -1;
}

int NFCDataBaseModule::CreateRole(const std::string& strAccount, const std::string& strRoleName)
{
    if (HasRole(strRoleName) <= 0)
    {
        NFCValueList valueRoleList;
        QueryAccountRoleList(strAccount, valueRoleList);

        if (valueRoleList.GetCount() < 4)
        {
            char szExecRole[MAX_PATH] = { 0 };
            sprintf(szExecRole, "insert into %s ( Account, RoleName ) values ( '%s', '%s' )", mstrPlayerTableName.c_str(), strAccount.c_str(), strRoleName.c_str());
            m_pDataBaseDriver->OTLExec(szExecRole, motlConnect);

            //////////////////////////////////////////////////////////////////////////
            //添加角色源
            valueRoleList.AddString(strRoleName.c_str());

            std::string strInfo;
            for (int i = 0; i < valueRoleList.GetCount(); i++)
            {
                strInfo.append(valueRoleList.StringVal(i));
                strInfo.append(";");
            }

            char szExecHead[MAX_PATH] = { 0 };
            sprintf(szExecHead, "UPDATE %s		SET RoleList = ", mstrAccountTableName.c_str());

            char szExecEnd[MAX_PATH] = { 0 };
            sprintf(szExecEnd, " WHERE Account = '%s'", strAccount.c_str());

            std::string strExec;
            strExec.append(szExecHead);
            strExec.append("'");
            strExec.append(strInfo);
            strExec.append("' ");
            strExec.append(szExecEnd);

            return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
        }
    }

    return 0;
}

bool NFCDataBaseModule::AfterInit()
{

    //test or create table
    CheckTable();

    //  NFCValueList valueAccount;
    //  QueryAccountProperty( "11", valueAccount );
    //
    //  for ( int i = 0; i < valueAccount.GetCount(); i++ )
    //  {
    //      std::cout << valueAccount.StringVal( i ) << std::endl;
    //  }
    //
    //  NFCValueList valueProperty;
    //  QueryRoleProperty( "qq", valueProperty );
    //  for ( int i = 0; i < valueProperty.GetCount(); i++ )
    //  {
    //      std::cout << valueProperty.StringVal( i ) << std::endl;
    //  }
    //
    //  SetAccountProperty( "11", NFCValueList() << "111,33" << "222,444" );

    return true;
}

void NFCDataBaseModule::CheckTable()
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "show tables like '%s'", mstrAccountTableName.c_str());
    if (0 == m_pDataBaseDriver->OTLExec(szExec, motlConnect))
    {
        //create table
        char szExecAccount[MAX_PATH] = { 0 };
        sprintf(szExecAccount, "CREATE TABLE `%s` (			`Account` char(32) NOT NULL DEFAULT '',			`Password` char(32) NOT NULL DEFAULT '',	`Property` longtext,			`Record` longtext,		`RoleList` longtext,		PRIMARY KEY (`Account`) ) ENGINE=InnoDB DEFAULT CHARSET=utf8;", mstrAccountTableName.c_str());
        m_pDataBaseDriver->OTLExec(szExecAccount, motlConnect);
    }

    memset(szExec, 0, MAX_PATH);
    sprintf(szExec, "show tables like '%s'", mstrPlayerTableName.c_str());
    if (0 == m_pDataBaseDriver->OTLExec(szExec, motlConnect))
    {
        //create table
        char szExecRole[MAX_PATH] = { 0 };
        sprintf(szExecRole, "CREATE TABLE `%s` (			`Account` char(32) NOT NULL DEFAULT '',			`RoleName` char(32) NOT NULL DEFAULT '',	`Property` longtext,			`Record` longtext,		PRIMARY KEY (`Account`,`RoleName`)			) ENGINE=InnoDB DEFAULT CHARSET=utf8;", mstrPlayerTableName.c_str());
        m_pDataBaseDriver->OTLExec(szExecRole, motlConnect);
    }
}

void NFCDataBaseModule::AddPropertyToRole(const std::string& strProperty)
{
    char szExec[MAX_PATH] = { 0 };
    //sprintf( szExec, "alter table docdsp  add dspcode char(256)", mstrPlayerTableName.c_str(), strAccount.c_str(), strRoleName.c_str() );

    m_pDataBaseDriver->OTLExec(szExec, motlConnect);
}

void NFCDataBaseModule::AddPropertyToAccount(const std::string& strProperty)
{
    char szExec[MAX_PATH] = { 0 };
    //sprintf( szExec, "alter table docdsp  add dspcode char(256)", mstrPlayerTableName.c_str(), strAccount.c_str(), strRoleName.c_str() );

    m_pDataBaseDriver->OTLExec(szExec, motlConnect);
}

int NFCDataBaseModule::HasPropertyInRole(const std::string& strProperty)
{
    char szExec[MAX_PATH] = { 0 };
    //sprintf( szExec, "insert into %s ( Account, RoleName ) values ( '%s', '%s' )", mstrPlayerTableName.c_str(), strAccount.c_str(), strRoleName.c_str() );
    return 0;
    //return m_pDataBaseDriver->OTLExec( szExec, motlConnect );
}

int NFCDataBaseModule::HasPropertyInAccount(const std::string& strProperty)
{
    char szExec[MAX_PATH] = { 0 };
    //sprintf( szExec, "insert into %s ( Account, RoleName ) values ( '%s', '%s' )", mstrPlayerTableName.c_str(), strAccount.c_str(), strRoleName.c_str() );
    return 0;
    //return m_pDataBaseDriver->OTLExec( szExec, motlConnect );
}

void NFCDataBaseModule::AddRecordToRole(const std::string& strProperty)
{

}

void NFCDataBaseModule::AddRecordToAccount(const std::string& strProperty)
{

}

int NFCDataBaseModule::QueryAccountProperty(const std::string& strAccount, NFIValueList& value)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT Property FROM %s	 WHERE Account = '%s'", mstrAccountTableName.c_str(), strAccount.c_str());

    otl_long_string strInfo;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> strInfo;
    }

    if (strInfo.len() > 0)
    {
        stringstream strStream;
        strStream << strInfo.v;

        string strProperty = strStream.str();
        value.Split(strProperty.c_str(), ";");

        return value.GetCount();
    }

    return 0;
}

int NFCDataBaseModule::QueryRoleProperty(const std::string& strRoleName, NFIValueList& value)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT Property FROM %s	 WHERE RoleName = '%s'", mstrPlayerTableName.c_str(), strRoleName.c_str());

    otl_long_string strInfo;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> strInfo;
    }

    if (strInfo.len() > 0)
    {
        stringstream strStream;
        strStream << strInfo.v;

        std::string strProperty = strStream.str();
        value.Split(strProperty.c_str(), ";");

        return value.GetCount();
    }

    return 0;
}

int NFCDataBaseModule::QueryRoleRecord(const std::string& strRoleName, NFIValueList& value)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT Record FROM %s	 WHERE RoleName = '%s'", mstrPlayerTableName.c_str(), strRoleName.c_str());

    otl_long_string strInfo;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> strInfo;
    }

    if (strInfo.len() > 0)
    {
        stringstream strStream;
        strStream << strInfo.v;

        string strProperty = strStream.str();
        value.Split(strProperty.c_str(), ";");

        return value.GetCount();
    }

    return 0;
}

int NFCDataBaseModule::QueryAccountRecord(const std::string& strAccount, NFIValueList& value)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT Record FROM %s	 WHERE Account = '%s'", mstrAccountTableName.c_str(), strAccount.c_str());

    otl_long_string strInfo;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> strInfo;
    }

    if (strInfo.len() > 0)
    {
        stringstream strStream;
        strStream << strInfo.v;

        string strProperty = strStream.str();
        value.Split(strProperty.c_str(), ";");

        return value.GetCount();
    }

    return 0;
}

int NFCDataBaseModule::SetAccountProperty(const std::string& strAccount, const NFIValueList& value)
{
    std::string strInfo;
    for (int i = 0; i < value.GetCount(); i++)
    {
        strInfo.append(value.StringVal(i));
        strInfo.append(";");
    }

    char szExecHead[MAX_PATH] = { 0 };
    sprintf(szExecHead, "UPDATE %s		SET Property = ", mstrAccountTableName.c_str());

    char szExecEnd[MAX_PATH] = { 0 };
    sprintf(szExecEnd, " WHERE Account = '%s'", strAccount.c_str());

    std::string strExec;
    strExec.append(szExecHead);
    strExec.append("'");
    strExec.append(strInfo);
    strExec.append("' ");
    strExec.append(szExecEnd);

    return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
}

int NFCDataBaseModule::SetRoleProperty(const std::string& strRoleName, const NFIValueList& value)
{
    if (value.GetCount() <= 0)
    {
        return -1;
    }

    std::string strInfo;
    for (int i = 0; i < value.GetCount(); i++)
    {
        strInfo.append(value.StringVal(i));
        strInfo.append(";");
    }

    char szExecHead[MAX_PATH] = { 0 };
    sprintf(szExecHead, "UPDATE %s		SET Property = ", mstrPlayerTableName.c_str());

    char szExecEnd[MAX_PATH] = { 0 };
    sprintf(szExecEnd, " WHERE RoleName = '%s'", strRoleName.c_str());

    std::string strExec;
    strExec.append(szExecHead);
    strExec.append("'");
    strExec.append(strInfo);
    strExec.append("' ");
    strExec.append(szExecEnd);

    return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
}

int NFCDataBaseModule::SetRoleRecord(const std::string& strRoleName, const NFIValueList& value)
{
    std::string strInfo;
    for (int i = 0; i < value.GetCount(); i++)
    {
        strInfo.append(value.StringVal(i));
        strInfo.append(";");
    }

    char szExecHead[MAX_PATH] = { 0 };
    sprintf(szExecHead, "UPDATE %s		SET Record = ", mstrPlayerTableName.c_str());

    char szExecEnd[MAX_PATH] = { 0 };
    sprintf(szExecEnd, " WHERE RoleName = '%s'", strRoleName.c_str());

    std::string strExec;
    strExec.append(szExecHead);
    strExec.append("'");
    strExec.append(strInfo);
    strExec.append("' ");
    strExec.append(szExecEnd);

    return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
}

int NFCDataBaseModule::SetAccountRecord(const std::string& strAccount, const NFIValueList& value)
{
    std::string strInfo;
    for (int i = 0; i < value.GetCount(); i++)
    {
        // n 个 table
        strInfo.append(value.StringVal(i));
        strInfo.append(";");
    }

    char szExecHead[MAX_PATH] = { 0 };
    sprintf(szExecHead, "UPDATE %s		SET Record = ", mstrAccountTableName.c_str());

    char szExecEnd[MAX_PATH] = { 0 };
    sprintf(szExecEnd, " WHERE Account = '%s'", strAccount.c_str());

    std::string strExec;
    strExec.append(szExecHead);
    strExec.append("'");
    strExec.append(strInfo);
    strExec.append("' ");
    strExec.append(szExecEnd);

    return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
}

int NFCDataBaseModule::QueryAccountRoleList(const std::string& strAccount, NFIValueList& value)
{
    char szExec[MAX_PATH] = { 0 };
    sprintf(szExec, "SELECT RoleList FROM %s	 WHERE Account = '%s'", mstrAccountTableName.c_str(), strAccount.c_str());

    otl_long_string strInfo;
    otl_stream oStream(1, szExec, motlConnect);

    while (!oStream.eof())
    {
        oStream >> strInfo;
    }

    if (strInfo.len() > 0)
    {
        stringstream strStream;
        strStream << strInfo.v;

        string strProperty = strStream.str();
        value.Split(strProperty.c_str(), ";");

        return value.GetCount();
    }

    return 0;
}

bool NFCDataBaseModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

int NFCDataBaseModule::DeleteRole(const std::string& strAccount, const std::string& strRoleName)
{
    if (HasRole(strRoleName) > 0)
    {
        NFCValueList valueRoleList;
        QueryAccountRoleList(strAccount, valueRoleList);

        //if ( valueRoleList.GetCount() < 5 )
        {
            std::string strInfo;
            for (int i = 0; i < valueRoleList.GetCount(); i++)
            {
                std::string strName = valueRoleList.StringVal(i);
                if (strRoleName != strName)
                {
                    strInfo.append(valueRoleList.StringVal(i));
                    strInfo.append(";");
                }
            }

            char szExecHead[MAX_PATH] = { 0 };
            sprintf(szExecHead, "UPDATE %s		SET RoleList = ", mstrAccountTableName.c_str());

            char szExecEnd[MAX_PATH] = { 0 };
            sprintf(szExecEnd, " WHERE Account = '%s'", strAccount.c_str());

            std::string strExec;
            strExec.append(szExecHead);
            strExec.append("'");
            strExec.append(strInfo);
            strExec.append("' ");
            strExec.append(szExecEnd);

            return m_pDataBaseDriver->OTLExec(strExec.c_str(), motlConnect);
        }
    }
    return 0;
}
