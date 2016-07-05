// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlModule.h"
#include "NFCMysqlDriverManager.h"

NFCMysqlModule::NFCMysqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mnLastCheckTime = 0;
}

NFCMysqlModule::~NFCMysqlModule()
{

}

bool NFCMysqlModule::Init()
{
    m_pMysqlDriverManager = NF_SHARE_PTR<NFIMysqlDriverManager> (NF_NEW NFCMysqlDriverManager());
    return true;
}

bool NFCMysqlModule::Shut()
{
    return true;
}

bool NFCMysqlModule::AfterInit()
{
    return true;
}

bool NFCMysqlModule::Execute()
{
    if (mnLastCheckTime + 10 > GetPluginManager()->GetNowTime())
    {
        return false;
    }

    mnLastCheckTime = GetPluginManager()->GetNowTime();

    if (m_pMysqlDriverManager.get())
    {
        m_pMysqlDriverManager->CheckMysql();
    }

    return true;
}

bool NFCMysqlModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
    if (!m_pMysqlDriverManager.get())
    {
        return false;
    }

    return m_pMysqlDriverManager->AddMysqlServer(nServerID, strDns, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
}

bool NFCMysqlModule::Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Updata(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool NFCMysqlModule::Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Query(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool NFCMysqlModule::Delete(const std::string& strRecordName, const std::string& strKey)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Delete(strRecordName, strKey);
    }

    return false;
}

bool NFCMysqlModule::Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Exists(strRecordName, strKey, bExit);
    }

    return false;
}

bool NFCMysqlModule::Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Select(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool NFCMysqlModule::Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (pDriver)
    {
        return pDriver->Keys(strRecordName, strKeyName, valueVec);
    }

    return false;
}
