#include "NFCMysqlDriver.h"

const std::string NFCMysqlDriver::strDefaultKey = "ID";
const std::string NFCMysqlDriver::strDefaultTable = "RoleInfo";

NFCMysqlDriver::NFCMysqlDriver(const int nReconnectTime/* = 60*/, const int nReconnectCount /*= -1*/)
{
    mfCheckReconnect = 0.0f;
    mnDBPort        = 0;
    m_pMysqlConnect = NULL;
    mnReconnectTime = nReconnectTime;
    mnReconnectCount = nReconnectCount;
}

NFCMysqlDriver::NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
    mfCheckReconnect = 0.0f;
    mnDBPort        = 0;
    m_pMysqlConnect = NULL;

    mnReconnectTime = 60;
    mnReconnectCount = -1;

    Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
}

NFCMysqlDriver::~NFCMysqlDriver()
{
    CloseConnection();
}

bool NFCMysqlDriver::Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
    mstrDBName  = strDBName   ;
    mstrDBHost  = strDBHost   ;
    mnDBPort    = nDBPort     ;
    mstrDBUser  = strDBUser   ;
    mstrDBPwd   = strDBPwd    ;

    return Connect();
}

bool NFCMysqlDriver::Execute()
{
    if (IsNeedReconnect() && CanReconnect())
    {
        Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);
    }

    return true;
}

bool NFCMysqlDriver::Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult)
{
    mysqlpp::Connection* pConection = GetConnection();
    if (pConection)
    {
        NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConection->query(qstr);
        //query.execute();

        queryResult = query.store();
        query.reset();

        NFMYSQLTRYEND(qstr)
        return true;
    }

    return false;
}

bool NFCMysqlDriver::Query(const std::string& qstr)
{
    mysqlpp::Connection* pConection = GetConnection();
    if (pConection)
    {
        NFMYSQLTRYBEGIN
        mysqlpp::Query query = pConection->query(qstr);
        query.execute();

        query.reset();

        NFMYSQLTRYEND(qstr)
        return true;
    }

    return false;
}

mysqlpp::Connection* NFCMysqlDriver::GetConnection()
{
    return m_pMysqlConnect;
}

void NFCMysqlDriver::CloseConnection()
{
    delete m_pMysqlConnect;
    m_pMysqlConnect = NULL;
}

bool NFCMysqlDriver::Enable()
{
    return !IsNeedReconnect();
}

bool NFCMysqlDriver::CanReconnect()
{

    mfCheckReconnect += 0.1f;

    //30分钟检查断线重连
    if (mfCheckReconnect < mnReconnectTime)
    {
        return false;
    }

    if (mnReconnectCount == 0)
    {
        return false;
    }

    mfCheckReconnect = 0.0f;

    return true;
}

bool NFCMysqlDriver::Reconnect()
{
    CloseConnection();
    Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);

    if (mnReconnectCount > 0)
    {
        mnReconnectCount--;
    }

    return true;
}

bool NFCMysqlDriver::IsNeedReconnect()
{
    //没有配置表
    if (mstrDBHost.length() < 1 || mstrDBUser.length() < 1)
    {
        return false;
    }

    if (NULL == m_pMysqlConnect)
    {
        return true;
    }

    if (!m_pMysqlConnect->connected())
    {
        CloseConnection();
        return true;
    }

    if (!m_pMysqlConnect->ping())
    {
        CloseConnection();
        return true;
    }

    return false;
}

bool NFCMysqlDriver::Connect()
{
    m_pMysqlConnect = new mysqlpp::Connection();
    if (NULL == m_pMysqlConnect)
    {
        return false;
    }
    NFMYSQLTRYBEGIN
    m_pMysqlConnect->set_option(new mysqlpp::MultiStatementsOption(true));
    m_pMysqlConnect->set_option(new mysqlpp::SetCharsetNameOption("utf8mb4"));
    m_pMysqlConnect->set_option(new mysqlpp::ReconnectOption(true));
    m_pMysqlConnect->set_option(new mysqlpp::ConnectTimeoutOption(60));
    if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(), mnDBPort))
    {
        CloseConnection();
        // 连接失败
        return false;
    }

    // 设置超时时间为24小时
    mysqlpp::Query query = m_pMysqlConnect->query("set interactive_timeout = 24*3600");
    query.execute();
    query.reset();
    NFMYSQLTRYEND("Connect faild")
    return true;
}

bool NFCMysqlDriver::Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
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


bool NFCMysqlDriver::Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
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


bool NFCMysqlDriver::Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    return false;
}

bool NFCMysqlDriver::Delete(const std::string& strRecordName, const std::string& strKey)
{
    mysqlpp::Connection* pConnection = GetConnection();
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

bool NFCMysqlDriver::Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit)
{
    mysqlpp::Connection* pConnection = GetConnection();
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

bool NFCMysqlDriver::Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
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
