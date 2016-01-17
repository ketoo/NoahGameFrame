// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlClusterModule.h"

std::string NFCMysqlClusterModule::strDefaultKey = "ID";
std::string NFCMysqlClusterModule::strDefaultTable = "RoleInfo";

NFCMysqlClusterModule::NFCMysqlClusterModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCMysqlClusterModule::~NFCMysqlClusterModule()
{

}

bool NFCMysqlClusterModule::Init()
{


    return true;
}

bool NFCMysqlClusterModule::Shut()
{

    return true;
}

bool NFCMysqlClusterModule::AfterInit()
{
    m_pMysqlConnectMgrManager = pPluginManager->FindModule<NFIMysqlConnectMgrModule>("NFCMysqlConnectMgrModule");

    assert(NULL != m_pMysqlConnectMgrManager);

    return true;
}

bool NFCMysqlClusterModule::Execute()
{
    return true;
}

bool NFCMysqlClusterModule::Updata( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec )
{
    NFIMysqlDriver* pDriver = m_pMysqlConnectMgrManager->GetMysqlDriver();
    if (!pDriver)
    {
        return false;
    }

    mysqlpp::Connection* pConnection = pDriver->GetConnection();
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

    NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConnection->query();
        if (bExist)
        {
            // update
            query << "UPDATE " << strRecordName << " SET ";
            for (int i = 0; i < fieldVec.size(); ++i)
            {
                if (i == 0)
                {
                    query << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
                }
                else
                {
                    query << "," << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
                }
            }

            query << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";
        }
        else
        {
            // insert
            query << "INSERT INTO " << strRecordName << "(" << strDefaultKey << ",";
            for (int i = 0; i < fieldVec.size(); ++i)
            {
                if (i == 0)
                {
                    query << fieldVec[i];
                }
                else
                {
                    query << ", " << fieldVec[i];
                }
            }

            query << ") VALUES(" << mysqlpp::quote << strKey << ",";
            for (int i = 0; i < valueVec.size(); ++i)
            {
                if (i == 0)
                {
                    query << mysqlpp::quote << valueVec[i];
                }
                else
                {
                    query << ", " << mysqlpp::quote << valueVec[i];
                }
            }

            query << ");";
        }

        query.execute();
        query.reset();
    NFMYSQLTRYEND("update or insert error")

	return true;
}

bool NFCMysqlClusterModule::Query( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
    NFIMysqlDriver* pDriver = m_pMysqlConnectMgrManager->GetMysqlDriver();
    if (!pDriver)
    {
        return false;
    }

    mysqlpp::Connection* pConnection = pDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConnection->query();
        query << "SELECT ";
        for (std::vector<std::string>::const_iterator iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
        {
            if (iter == fieldVec.begin())
            {
                query << *iter;
            }
            else
            {
                query << "," << *iter;
            }            
        }
        query << " FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";
        //query.execute(); // 官网例子不需要execute
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

bool NFCMysqlClusterModule::Delete( const std::string& strRecordName, const std::string& strKey )
{
    NFIMysqlDriver* pDriver = m_pMysqlConnectMgrManager->GetMysqlDriver();
    if (!pDriver)
    {
        return false;
    }

    mysqlpp::Connection* pConnection = pDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConnection->query();
        query << "DELETE FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";

        query.execute();
        query.reset();
    NFMYSQLTRYEND("delete error")

	return true;
}

bool NFCMysqlClusterModule::Exists( const std::string& strRecordName, const std::string& strKey, bool& bExit )
{
    NFIMysqlDriver* pDriver = m_pMysqlConnectMgrManager->GetMysqlDriver();
    if (!pDriver)
    {
        return false;
    }

    mysqlpp::Connection* pConnection = pDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConnection->query();
        query << "SELECT 1 FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << " LIMIT 1;";

        //query.execute();
        mysqlpp::StoreQueryResult result = query.store();
        query.reset();

        if (!result || result.empty())
        {
            bExit = false;
            return true;
        }

    NFMYSQLTRYEND("exist error")

    bExit = true;
	return true;
}

bool NFCMysqlClusterModule::Select( const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec )
{
	return false;
}

bool NFCMysqlClusterModule::Keys( const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec )
{
    NFIMysqlDriver* pDriver = m_pMysqlConnectMgrManager->GetMysqlDriver();
    if (!pDriver)
    {
        return false;
    }

    mysqlpp::Connection* pConnection = pDriver->GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }
    
    const std::string strLikeKey = "%" + strKeyName + "%";

    NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConnection->query();
    query << "SELECT " << strDefaultKey << " FROM " << strRecordName << " WHERE " << strDefaultKey << " LIKE " << mysqlpp::quote << strLikeKey << " LIMIT 100;";

    mysqlpp::StoreQueryResult xResult = query.store();
    query.reset();

    if (xResult.empty() || !xResult)
    {
        return false;
    }

    // xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
    for (int i = 0; i < xResult.size(); ++i)
    {
        std::string strValue(xResult[i][strDefaultKey.data()].data(), xResult[i][strDefaultKey.data()].length());
        valueVec.push_back(strValue);
    }

    NFMYSQLTRYEND("exist error")

   return true;
}
