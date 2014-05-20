// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerPublicProxyModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-27
//    @Module           :    NFCGameServerPublicProxyModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameServerPublicProxyModule.h"

bool NFCGameServerPublicProxyModule::Init()
{
    //     
    //     m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    //     m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>(pPluginManager->FindModule("NFCGameLogicModule"));
    //     m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    //     m_pGameServerModule = dynamic_cast<NFIGameServerNet_ServerModule*>(pPluginManager->FindModule("NFCGameServerNet_ServerModule"));
    //
    //     assert(NULL != m_pEventProcessModule);
    //     assert(NULL != m_pGameLogicModule);
    //     assert(NULL != m_pKernelModule);
    //     assert(NULL != m_pGameServerModule);

    return true;
}

bool NFCGameServerPublicProxyModule::Shut()
{
    //    Final();

    return true;
}

bool NFCGameServerPublicProxyModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}


// int NFCGameServerPublicProxyModule::OnClassCommonEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
// {
//  if (strClassName == "Player")
//  {
//      if (0 == nClassEvent
//          && m_pKernelModule->QueryPropertyInt( self, "SceneID" ) > 0 )
//      {
//          //被删除，也有可能是大厅角色，因此场景ID一定大于0才行
//          //删除他的连接和连接信息,包括世界服务器
//          _tagLTW_CLIENT_EXIT_WORLD exitWorld;
//          exitWorld.nWorldID = 0;
//          strcpy(exitWorld._szAccount, m_pKernelModule->QueryPropertyString(self, "Account") );
//
//          RakNet::BitStream bitStream;
//          exitWorld.EnCode( bitStream );
//
//          SendBitStream(&bitStream);
//      }
//
//  }
//
//  return 0;
// }