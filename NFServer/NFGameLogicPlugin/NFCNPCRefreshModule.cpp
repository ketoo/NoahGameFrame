// -------------------------------------------------------------------------
//    @FileName         :   NFCNPCRefreshModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule
//    @Desc                 :    NPCÀ¿ÕˆÀ¢–¬
// -------------------------------------------------------------------------

#include "NFCNPCRefreshModule.h"

bool NFCNPCRefreshModule::Init()
{

    return true;
}


bool NFCNPCRefreshModule::Shut()
{
    return true;
}

bool NFCNPCRefreshModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //Œª÷√ƒÿ
    return true;
}

bool NFCNPCRefreshModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>( pPluginManager->FindModule( "NFCSceneProcessModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pPackModule = dynamic_cast<NFIPackModule*>( pPluginManager->FindModule( "NFCPackModule" ) );

    m_pEventProcessModule->AddClassCallBack( "AttackNPC", this, &NFCNPCRefreshModule::OnObjectClassEvent );

    return true;
}

int NFCNPCRefreshModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    if ( strClassName == "AttackNPC" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            int nHPMax = m_pKernelModule->QueryPropertyInt(self, "MAXHP");
            m_pKernelModule->SetPropertyInt(self, "HP", nHPMax);
            m_pKernelModule->AddPropertyCallBack( self, "HP", this, &NFCNPCRefreshModule::OnObjectHPEvent );
        }
    }
    return 0;
}

int NFCNPCRefreshModule::OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    if ( newVar.IntVal( 0 ) <= 0 )
    {
        NFIDENTID identAttacker = m_pKernelModule->QueryPropertyObject( self, "LastAttacker" );
        if (!identAttacker.IsNull())
		{
            m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_BE_KILLED, NFCValueList() << identAttacker );

            m_pKernelModule->AddHeartBeat( self, "OnDeadDestroyHeart", this, &NFCNPCRefreshModule::OnDeadDestroyHeart, NFCValueList(), 5.0f, 1 );
        }        
    }

    return 0;
}

int NFCNPCRefreshModule::OnDeadDestroyHeart( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    //and create new object
    const std::string& strClassName = m_pKernelModule->QueryPropertyString( self, "ClassName" );
    const std::string& strSeendID = m_pKernelModule->QueryPropertyString( self, "NPCConfigID" );
    const std::string& strConfigID = m_pKernelModule->QueryPropertyString( self, "ConfigID" );
    int nContainerID = m_pKernelModule->QueryPropertyInt( self, "SceneID" );
    int nGroupID = m_pKernelModule->QueryPropertyInt( self, "GroupID" );

    //m_pSceneProcessModule->ClearAll( nContainerID, nGroupID, strSeendID );

    m_pKernelModule->DestroyObject( self );
    return 0;
}
