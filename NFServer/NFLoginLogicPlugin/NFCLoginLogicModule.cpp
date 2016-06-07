// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFLoginLogicPlugin.h"
#include "NFCLoginLogicModule.h"


bool NFCLoginLogicModule::Init()
{

    return true;
}

bool NFCLoginLogicModule::Shut()
{
    return true;
}

int NFCLoginLogicModule::OnLoginProcess(const NFGUID& object, const std::string& strAccount, const std::string& strPwd)
{
    int64_t nState = -1;

    std::vector<std::string> xFieldVec;
    std::vector<std::string> xValueVec;
    xFieldVec.push_back("Password");

    if (m_pClusterSqlModule->Query("AccountInfo", strAccount, xFieldVec, xValueVec)
        && xFieldVec.size() == xValueVec.size()
        && strPwd == xValueVec[0])
    {
        nState = 0;
    }

    return nState;
}

bool NFCLoginLogicModule::Execute()
{


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{
    m_pClusterSqlModule = pPluginManager->FindModule<NFIMysqlModule>("NFCMysqlModule");
    m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>("NFCLoginNet_ServerModule");

    assert(NULL != m_pClusterSqlModule);
    assert(NULL != m_pLoginNet_ServerModule);

    return true;
}
