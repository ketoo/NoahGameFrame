// -------------------------------------------------------------------------
//    @FileName         :   NFCNPCRefreshModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule
//    @Desc                 :    NPCËÀÍöË¢ÐÂ
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

bool NFCNPCRefreshModule::Execute()
{
    return true;
}

bool NFCNPCRefreshModule::AfterInit()
{
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>( "NFCEventProcessModule" );
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
    m_pPackModule = dynamic_cast<NFIPackModule*>( pPluginManager->FindModule( "NFCPackModule" ) );

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pPackModule);

    m_pEventProcessModule->AddClassCallBack( "NPC", this, &NFCNPCRefreshModule::OnObjectClassEvent );

    return true;
}

int NFCNPCRefreshModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
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

int NFCNPCRefreshModule::OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    if ( newVar.GetInt() <= 0 )
    {
        NFGUID identAttacker = m_pKernelModule->GetPropertyObject( self, "LastAttacker" );
        if (!identAttacker.IsNull())
		{
            m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_BE_KILLED, NFCDataList() << identAttacker );

            m_pKernelModule->AddHeartBeat( self, "OnDeadDestroyHeart", this, &NFCNPCRefreshModule::OnDeadDestroyHeart, 5.0f, 1 );
        }
    }

    return 0;
}

int NFCNPCRefreshModule::OnDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    //and create new object
    const std::string& strClassName = m_pKernelModule->GetPropertyString( self, "ClassName" );
    const std::string& strSeedID = m_pKernelModule->GetPropertyString( self, "NPCConfigID" );
    const std::string& strConfigID = m_pKernelModule->GetPropertyString( self, "ConfigID" );
    int nContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
    int nGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

    //m_pSceneProcessModule->ClearAll( nContainerID, nGroupID, strSeendID );

	float fSeedX = m_pKernelModule->GetPropertyFloat( self, "X" );
	float fSeedY = m_pKernelModule->GetPropertyFloat( self, "Y" );
	float fSeedZ = m_pKernelModule->GetPropertyFloat( self, "Z" );
    
    m_pKernelModule->DestroyObject( self );

    NFCDataList arg;
	arg << "X" << fSeedX;
	arg << "Y" << fSeedY;
	arg << "Z" << fSeedZ;
	arg << "SeedID" << strSeedID;

	m_pKernelModule->CreateObject( NFGUID(), nContainerID, nGroupID, strClassName, strConfigID, arg );

    return 0;
}
