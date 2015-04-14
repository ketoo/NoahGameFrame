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

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pPackModule);

    m_pEventProcessModule->AddClassCallBack( "NPC", this, &NFCNPCRefreshModule::OnObjectClassEvent );

    return true;
}

int NFCNPCRefreshModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == "NPC" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, "ConfigID");
            int nHPMax = m_pElementInfoModule->GetPropertyInt(strConfigID, "MAXHP");
            m_pKernelModule->SetPropertyInt(self, "HP", nHPMax);
            m_pKernelModule->AddPropertyCallBack( self, "HP", this, &NFCNPCRefreshModule::OnObjectHPEvent );
        }
    }
    return 0;
}

int NFCNPCRefreshModule::OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{
    if ( newVar.Int( 0 ) <= 0 )
    {
        NFIDENTID identAttacker = m_pKernelModule->GetPropertyObject( self, "LastAttacker" );
        if (!identAttacker.IsNull())
		{
            m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_BE_KILLED, NFCDataList() << identAttacker );

            m_pKernelModule->AddHeartBeat( self, "OnDeadDestroyHeart", this, &NFCNPCRefreshModule::OnDeadDestroyHeart, NFCDataList(), 5.0f, 1 );
        }        
    }

    return 0;
}

int NFCNPCRefreshModule::OnDeadDestroyHeart( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIDataList& var )
{
    //and create new object
    const std::string& strClassName = m_pKernelModule->GetPropertyString( self, "ClassName" );
    const std::string& strSeendID = m_pKernelModule->GetPropertyString( self, "NPCConfigID" );
    const std::string& strConfigID = m_pKernelModule->GetPropertyString( self, "ConfigID" );
    int nContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
    int nGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

    //m_pSceneProcessModule->ClearAll( nContainerID, nGroupID, strSeendID );

    m_pKernelModule->DestroyObject( self );
    return 0;
}
