// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCDataBaseNet_ClientModule.h"
#include "NFDataBaseNet_ClientPlugin.h"

bool NFCDataBaseNet_ClientModule::Init()
{    
    return true;
}

bool NFCDataBaseNet_ClientModule::Shut()
{
    return true;
}

bool NFCDataBaseNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCDataBaseNet_ClientModule::AfterInit()
{
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pMysqlConnectMgrModule = dynamic_cast<NFIMysqlConnectMgrModule*>(pPluginManager->FindModule("NFCMysqlConnectMgrModule"));

    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pMysqlConnectMgrModule);

    NF_SHARE_PTR<NFILogicClass> pLogicClass = m_pLogicClassModule->GetElement("SqlServer");
    if (nullptr == pLogicClass)
    {
        return false;
    }

    NFList<std::string>& xNameList = pLogicClass->GetConfigNameList();
    std::string strConfigName; 
    if (!xNameList.Get(0, strConfigName))
    {
        return false;
    }

    const int nID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
    const std::string& mstrSQLIP = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlIP");
    const int mnSQLPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "SqlPort");
    const std::string& mstrSQLName = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlName");
    const std::string& mstrSQLUser = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlUser");
    const std::string& mstrSQLPWD = m_pElementInfoModule->GetPropertyString(strConfigName, "SqlPwd");

    bool bConnect = m_pMysqlConnectMgrModule->AddMysqlServer(nID, "", mstrSQLIP, mnSQLPort, mstrSQLName, mstrSQLUser, mstrSQLPWD);

    // Log

    return true;
}

bool NFCDataBaseNet_ClientModule::BeforeShut()
{
    return true;
}