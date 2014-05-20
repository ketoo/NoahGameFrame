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

//     NFConfig config;
//     bool bLoad =  config.Load("NFLoginConfig.cfg");
//     if (!bLoad)
//     {
//         assert(0);
// 
//         return true;
//     }
// 
//     mnLoginID = config["LoginID"].toNumber<int>();

    return true;
}

bool NFCLoginLogicModule::Shut()
{
    return true;
}

int NFCLoginLogicModule::OnLoginEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (3 != var.GetCount()
        || !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////
    int nAddress = var.IntVal(0);
    const std::string& strAccount = var.StringVal(1);
    const std::string& strPassword = var.StringVal(2);
    //int nState = m_pDataBaseModule->ConfirmAccountInfo(strAccount, strPassword);
    //m_pNoSqlModule->AddAccountInfo(strAccount, strPassword);

    int nState = m_pNoSqlModule->ConfirmAccountInfo(strAccount, strPassword);
 
    NFCValueList valEventInfo;
    valEventInfo << nState << nAddress << strAccount;
    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGIN_RESULTS, valEventInfo);
  
    return 0;
}

int NFCLoginLogicModule::OnLogOutEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (2 != var.GetCount()
        || !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return -1;
    }

    int  nAddress = var.IntVal(0);
    const std::string& strAccount = var.StringVal(1);

    NFCValueList valEventInfo;
    valEventInfo << nAddress << strAccount.c_str();
    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGOUT_RESULTS, valEventInfo);

    return 0;
}

// int NFCLoginLogicModule::OnRequireServerListEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
// {
//  if ( 1 != var.GetCount())
//  {
//      return -1;
//  }
//
//  std::string strAddress = var.StringVal(0);
//  WorldServerInfo* pInfo = First();
//  while ( pInfo )
//  {
//      NFCValueList valWorldInfo;
//      valWorldInfo << strAddress << pInfo->nWorldID << pInfo->strWorldName << pInfo->nMaxOnline << pInfo->nOnlineCount;
//      m_pEventProcessModule->DoEvent( 0, NFED_ON_CLIENT_REQUIRE_SERVER_LIST_RESULTS, valWorldInfo );
//
//      pInfo = Next();
//  }
//
//  return 0;
// }

int NFCLoginLogicModule::OnDisconnectEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (2 != var.GetCount()
        || !var.TypeEx(VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return -1;
    }

    //std::string strAccount = var.StringVal(0);

    return 0;
}

bool NFCLoginLogicModule::Execute(const float fLasFrametime, const float fStartedTime)
{

    //////////////////////////////////////////////////////////////////////////
#ifdef NF_DEBUG_MODE
    //     char szContent[MAX_PATH] = { 0 };
    //     if (kbhit() && gets(szContent))
    //     {
    //         NFCValueList val(szContent, ",");
    //         if (val.GetCount() > 0)
    //         {
    //             const std::string& strCmd = val.StringVal(0);
    //             //if (0 == strcmp("listworldserver", pstrCmd))
    //             if ("listworldserver" == strCmd)
    //             {
    //                 NFCValueList valOobjectList;
    //                 int nCount = m_pKernelModule->GetContainerOnLineList(-2, valOobjectList);
    //                 for (int i = 0; i < nCount; i++)
    //                 {
    //                     NFIDENTID ident = valOobjectList.ObjectVal(i);
    //
    //                     char szInfo[MAX_PATH] = { 0 };
    //                     sprintf_s(szInfo, "WorldID:%d, WorldName:%s, WorldMaxOnline:%d, WorldOnlineCount:%d",
    //                               m_pKernelModule->QueryPropertyInt(ident, "WorldID"),
    //                               m_pKernelModule->QueryPropertyString(ident, "WorldName"),
    //                               m_pKernelModule->QueryPropertyInt(ident, "WorldMaxOnline"),
    //                               m_pKernelModule->QueryPropertyInt(ident, "WorldOnlineCount"));
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
    m_pNoSqlModule = dynamic_cast<NFIPlatformDataModule*>(pPluginManager->FindModule("NFCPlatformDataModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pNoSqlModule);
    assert(NULL != m_pKernelModule);

    //////////////////////////////////////////////////////////////////////////
    // register event calback

    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGIN, this, &NFCLoginLogicModule::OnLoginEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGOUT, this, &NFCLoginLogicModule::OnLogOutEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_DISCONNECT, this, &NFCLoginLogicModule::OnDisconnectEvent);

    //m_pKernelModule->SetIdentSerialID(mnLoginID);

    return true;
}
