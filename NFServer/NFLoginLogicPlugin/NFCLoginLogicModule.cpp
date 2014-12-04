// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
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

int NFCLoginLogicModule::OnLoginEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    if (3 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////
    int nAddress = var.Int(0);
    const std::string& strAccount = var.String(1);
    const std::string& strPassword = var.String(2);
    //int nState = m_pDataBaseModule->ConfirmAccountInfo(strAccount, strPassword);
    //m_pNoSqlModule->AddAccountInfo(strAccount, strPassword);

    int nState = 0;//= m_pNoSqlModule->ConfirmAccountInfo(strAccount, strPassword);
 
    NFCDataList valEventInfo;
    valEventInfo << nState << nAddress << strAccount;
    m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_CLIENT_LOGIN_RESULTS, valEventInfo);
  
    return 0;
}

int NFCLoginLogicModule::OnDisconnectEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    if (2 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    //std::string strAccount = var.String(0);

    return 0;
}

bool NFCLoginLogicModule::Execute(const float fLasFrametime, const float fStartedTime)
{

    //////////////////////////////////////////////////////////////////////////
#ifdef NF_DEBUG_MODE
    //     char szContent[MAX_PATH] = { 0 };
    //     if (kbhit() && gets(szContent))
    //     {
    //         NFCDataList val(szContent, ",");
    //         if (val.GetCount() > 0)
    //         {
    //             const std::string& strCmd = val.String(0);
    //             //if (0 == strcmp("listworldserver", pstrCmd))
    //             if ("listworldserver" == strCmd)
    //             {
    //                 NFCDataList valOobjectList;
    //                 int nCount = m_pKernelModule->GetContainerOnLineList(-2, valOobjectList);
    //                 for (int i = 0; i < nCount; i++)
    //                 {
    //                     NFIDENTID ident = valOobjectList.Object(i);
    //
    //                     char szInfo[MAX_PATH] = { 0 };
    //                     sprintf_s(szInfo, "WorldID:%d, WorldName:%s, WorldMaxOnline:%d, WorldOnlineCount:%d",
    //                               m_pKernelModule->GetPropertyInt(ident, "WorldID"),
    //                               m_pKernelModule->GetPropertyString(ident, "WorldName"),
    //                               m_pKernelModule->GetPropertyInt(ident, "WorldMaxOnline"),
    //                               m_pKernelModule->GetPropertyInt(ident, "WorldOnlineCount"));
    //
    //                     m_pKernelModule->LogInfo(szInfo);
    //                 }
    //             }
    //             else if ("listclient" == strCmd)
    //                 //else if (0 == strcmp("listclient", pstrCmd))
    //             {
    //             }
    //             else
    //             {
    //                 m_pKernelModule->Command(val);
    //             }
    //         }
    //     }
#endif


    return true;
}


bool NFCLoginLogicModule::AfterInit()
{



    //////////////////////////////////////////////////////////////////////////
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    //m_pNoSqlModule = dynamic_cast<NFIPlatformDataModule*>(pPluginManager->FindModule("NFCPlatformDataModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));


    assert(NULL != m_pEventProcessModule);
    //assert(NULL != m_pNoSqlModule);
    assert(NULL != m_pKernelModule);

    //////////////////////////////////////////////////////////////////////////
    // register event calback
    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_CLIENT_LOGIN, this, &NFCLoginLogicModule::OnLoginEvent);

    return true;
}
