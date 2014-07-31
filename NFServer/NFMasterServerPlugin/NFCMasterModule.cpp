// -------------------------------------------------------------------------
//    @FileName      :    NFCMasterModle.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterModle
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <conio.h>
#include "NFCMasterModule.h"
#include "NFMasterServerPlugin.h"

bool NFCMasterModule::Init()
{

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));


    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);




    return true;
}

bool NFCMasterModule::Shut()
{
    return true;
}

bool NFCMasterModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    //////////////////////////////////////////////////////////////////////////
//     char szContent[MAX_PATH] = { 0 };
//     if (kbhit() && gets(szContent))
//     {
//         NFCDataList val(szContent, ",");
//         if (val.GetCount() > 0)
//         {
//             const std::string& strCmd = val.StringVal(0);
//             if ("listworldserver" == strCmd)
//             {
//                 NFCDataList valOobjectList;
//                 int nCount = m_pKernelModule->GetContainerOnLineList(-2, valOobjectList);
//                 for (int i = 0; i < nCount; i++)
//                 {
//                     NFIDENTID ident = valOobjectList.ObjectVal(i);
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
//             else if ("listloginserver" == strCmd)
//             {
//                 NFCDataList valOobjectList;
//                 int nCount = m_pKernelModule->GetContainerOnLineList(-1, valOobjectList);
//                 for (int i = 0; i < nCount; i++)
//                 {
//                     NFIDENTID ident = valOobjectList.ObjectVal(i);
// 
//                     char szInfo[MAX_PATH] = { 0 };
//                     sprintf_s(szInfo, "LoginID:%d, LoginName:%s, LoginMaxOnline:%d, LoginOnlineCount:%d",
//                               m_pKernelModule->GetPropertyInt(ident, "LoginID"),
//                               m_pKernelModule->GetPropertyString(ident, "LoginName"),
//                               m_pKernelModule->GetPropertyInt(ident, "LoginMaxOnline"),
//                               m_pKernelModule->GetPropertyInt(ident, "LoginOnlineCount"));
// 
//                     m_pKernelModule->LogInfo(szInfo);
//                 }
//             }
//             else if ("listcontainer" == strCmd)
//             {
//                 //              NFCDataList valOobjectList;
//                 //              int nCount= m_pKernelModule->GetContainerOnLineList( mnLoginContranerID, valOobjectList );
//                 //              for ( int i = 0; i < nCount; i++ )
//                 //              {
//                 //                  NFIDENTID ident = valOobjectList.ObjectVal(i);
//                 //
//                 //                  char szInfo[MAX_PATH] = { 0 };
//                 //                  sprintf_s( szInfo, "LoginID:%d, LoginName:%s, LoginMaxOnline:%d, LoginOnlineCount:%d",
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginID" ),
//                 //                      m_pKernelModule->GetPropertyString( ident, "LoginName" ),
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginMaxOnline" ),
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginOnlineCount" ) );
//                 //
//                 //                  m_pKernelModule->PrintInfo( szInfo );
//                 //              }
//             }
//             else if ("listobject" == strCmd)
//             {
//                 //              NFCDataList valOobjectList;
//                 //              int nCount= m_pKernelModule->GetContainerOnLineList( mnLoginContranerID, valOobjectList );
//                 //              for ( int i = 0; i < nCount; i++ )
//                 //              {
//                 //                  NFIDENTID ident = valOobjectList.ObjectVal(i);
//                 //
//                 //                  char szInfo[MAX_PATH] = { 0 };
//                 //                  sprintf_s( szInfo, "LoginID:%d, LoginName:%s, LoginMaxOnline:%d, LoginOnlineCount:%d",
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginID" ),
//                 //                      m_pKernelModule->GetPropertyString( ident, "LoginName" ),
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginMaxOnline" ),
//                 //                      m_pKernelModule->GetPropertyInt( ident, "LoginOnlineCount" ) );
//                 //
//                 //                  m_pKernelModule->PrintInfo( szInfo );
//                 //              }
//             }
//             else
//             {
//                 m_pKernelModule->Command(val);
//             }
//         }
//     }

    return true;
}
