// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlClusterModule.h"

std::string NFCMysqlClusterModule::strDefaultKey = "id";
std::string NFCMysqlClusterModule::strDefaultTable = "role_info";

NFCMysqlClusterModule::NFCMysqlClusterModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCMysqlClusterModule::~NFCMysqlClusterModule()
{

}

bool NFCMysqlClusterModule::Init()
{
//     m_pShareMemoryModule = dynamic_cast<NFIShareMemoryModule*>(pPluginManager->FindModule("NFCShareMemoryModule"));
// 
//     otl_connect::otl_initialize();
// 
//     bool bLoad = mConfig.Load(mstrDataBaseConfigFile);
// 
//     mstrUID = mConfig["UID"].str();
//     mstrPWD = mConfig["PWD"].str();
//     mstrDSN = mConfig["DSN"].str();
//     mstrMasterName = mConfig["MST"].str();
// 
//     mstrAccountTableName = mConfig["AccountTBL"].str();
//     mstrPlayerTableName = mConfig["RoleTBL"].str();
//     mstrGlobalTableName = mConfig["GlobalTBL"].str();
// 
// 
//     m_pDataBaseDriver->OTLConnect(mstrUID, mstrPWD, mstrDSN, motlConnect);

    return true;
}

bool NFCMysqlClusterModule::Shut()
{
    //m_pDataBaseDriver->OTLDisconnect(motlConnect);

    return true;
}

bool NFCMysqlClusterModule::AfterInit()
{

    return true;
}

bool NFCMysqlClusterModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCMysqlClusterModule::Updata( const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec )
{
	return Updata(strDefaultTable, strKey, fieldVec, valueVec);
}

bool NFCMysqlClusterModule::Updata( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec )
{
    mysqlpp::Connection* pConnection = m_pDataBaseDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    bool bExist = false;
    if (!Exists(strRecordName, strKey, bExist))
    {
        return false;
    }


    if (fieldVec.size() != valueVec.size())
    {
        return false;
    }

    std::ostringstream strSQL;
    if (bExist)
    {
        // update
        strSQL << "UPDATE " << strRecordName << " ";
        for (int i = 0; i < fieldVec.size(); ++i)
        {
            if (i == 0)
            {
                strSQL << fieldVec[i] << " = " << valueVec[i];
            }
            else
            {
                strSQL << "," << fieldVec[i] << " = " << valueVec[i];
            }
        }
    }
    else
    {
        // insert
        strSQL << "INSERT INTO " << strRecordName << "(";
        for (int i = 0; i < fieldVec.size(); ++i)
        {
            if (i == 0)
            {
                strSQL << fieldVec[i];
            }
            else
            {
                strSQL << ", " << fieldVec[i];
            }
        }
        
        strSQL << ") VALUES(";
        for (int i = 0; i < valueVec.size(); ++i)
        {
            if (i == 0)
            {
                strSQL << mysqlpp::quote << valueVec[i];
            }
            else
            {
                strSQL << ", " << mysqlpp::quote << valueVec[i];
            }
        }

        strSQL << ")";
    }

    strSQL << ";";

    NFMYSQLTRYBEGIN
        std::ostringstream stream;
        stream << "DELETE FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";

        mysqlpp::Query query = pConnection->query(stream.str());
        query.reset();
    NFMYSQLTRYEND("update or insert error")

	return true;
}

bool NFCMysqlClusterModule::Query( const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
    return Query(strDefaultTable, strKey, fieldVec, valueVec);
}

bool NFCMysqlClusterModule::Query( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
    mysqlpp::Connection* pConnection = m_pDataBaseDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        std::ostringstream stream;
        stream << "SELECT ";
        for (std::vector<std::string>::const_iterator iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
        {
            if (iter == fieldVec.begin())
            {
                stream << *iter;
            }
            else
            {
                stream << "," << *iter;
            }            
        }
        stream << " FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";
        
        
        mysqlpp::Query query = pConnection->query(stream.str());
        mysqlpp::StoreQueryResult xResult = query.store();
        query.reset();

        if (xResult.empty() || !xResult)
        {
            return false;
        }

        // xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
        for (int i = 0; i < xResult.size(); ++i)
        {
            for (int j = 0; j < fieldVec.size(); ++j)
            {
                const std::string& strFieldName = fieldVec[j];
                std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
                valueVec.push_back(strValue);
            }
        }
    NFMYSQLTRYEND("query error")

	return true;
}

bool NFCMysqlClusterModule::Delete( const std::string& strKey )
{
    return Delete(strDefaultTable, strKey);
}

bool NFCMysqlClusterModule::Delete( const std::string& strRecordName, const std::string& strKey )
{
    mysqlpp::Connection* pConnection = m_pDataBaseDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        std::ostringstream stream;
        stream << "DELETE FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";

        mysqlpp::Query query = pConnection->query(stream.str());
        query.reset();
    NFMYSQLTRYEND("delete error")

	return true;
}

bool NFCMysqlClusterModule::Exists( const std::string& strKey, bool& bExit )
{
    //select 1 from table where col_name = col_value limit 1;
	return Exists(strDefaultTable, strKey, bExit);
}

bool NFCMysqlClusterModule::Exists( const std::string& strRecordName, const std::string& strKey, bool& bExit )
{
    mysqlpp::Connection* pConnection = m_pDataBaseDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        std::ostringstream stream;
        stream << "SELECT 1 FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << " LIMIT 1;";

        mysqlpp::Query query = pConnection->query(stream.str());
        mysqlpp::StoreQueryResult result = query.store();
        query.reset();

        if (!result || result.empty())
        {
            bExit = false;
            return false;
        }

    NFMYSQLTRYEND("exist error")

    bExit = true;
	return true;
}

bool NFCMysqlClusterModule::Select( const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
	return false;
}

bool NFCMysqlClusterModule::Select( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
	return false;
}