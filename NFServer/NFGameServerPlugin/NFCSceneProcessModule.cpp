// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#include "NFCSceneProcessModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFTimer.h"

bool NFCSceneProcessModule::Init()
{
    mnContainerLine = 10;
    mnLineMaxPlayer = 100;



    return true;
}

bool NFCSceneProcessModule::Shut()
{
    return true;
}

bool NFCSceneProcessModule::Execute()
{
    return true;
}

bool NFCSceneProcessModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>( "NFCLogicClassModule" );
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>( "NFCPropertyModule" );
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");

	assert( NULL != m_pKernelModule );
	assert( NULL != m_pElementInfoModule );
	assert( NULL != m_pLogicClassModule );
	assert( NULL != m_pPropertyModule );
	assert( NULL != m_pLogModule );

	m_pKernelModule->AddClassCallBack( NFrame::Player::ThisName(), this, &NFCSceneProcessModule::OnObjectClassEvent );
	//////////////////////////////////////////////////////////////////////////

    //初始化场景容器
// #ifdef NF_USE_ACTOR
// 	int nSelfActorID = pPluginManager->GetActorID();
// #endif
    NF_SHARE_PTR<NFILogicClass> pLogicClass =  m_pLogicClassModule->GetElement("Scene");
    if (pLogicClass.get())
    {
        NFList<std::string>& list = pLogicClass->GetConfigNameList();

        std::string strData;
        bool bRet = list.First(strData);
        while (bRet)
        {
            int nSceneID = boost::lexical_cast<int>(strData);

            const std::string& strFilePath = m_pElementInfoModule->GetPropertyString( strData, NFrame::Scene::FilePath());
            const int nActorID = m_pElementInfoModule->GetPropertyInt( strData, NFrame::Scene::ActorID());

			//if ( nActorID == nSelfActorID && nSceneID > 0 )
            {
                LoadInitFileResource( nSceneID );

                m_pKernelModule->CreateScene( nSceneID, strData );

                if ( E_SCENE_TYPE::SCENE_TYPE_NORMAL == GetCloneSceneType(nSceneID) )
                {
                    for (int i = 1; i <= mnContainerLine; ++i)
                    {
                        const int nTargetGroupID = m_pKernelModule->RequestGroupScene( nSceneID);
                        const int nCreateGroupID = CreateCloneScene( nSceneID, nTargetGroupID, "File.xml", NFCDataList() );
                        if ( nCreateGroupID == nTargetGroupID)
                        {
                            CreateContinerObjectByFile( nSceneID, nTargetGroupID, "File.xml" );
                        }
                    }
                }
            }

            bRet = list.Next(strData);
        }
    }

    return true;
}

bool NFCSceneProcessModule::CreateContinerObjectByFile( const int nContainerID, const int nGroupID, const std::string& strFileName )
{
    NF_SHARE_PTR<NFMapEx<std::string, SceneGroupResource>> pSceneResource = mtSceneResourceConfig.GetElement( nContainerID );
    if ( pSceneResource.get() )
    {
        NF_SHARE_PTR<SceneGroupResource> pResourceMap = pSceneResource->GetElement( strFileName );
        if ( pResourceMap.get() )
        {
            NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>> pNPCResourceList = pResourceMap->xSceneGroupResource.GetElement(NFrame::NPC::ThisName());
            if ( pNPCResourceList.get() )
            {
                NF_SHARE_PTR<SceneSeedResource> pResource = pNPCResourceList->First( );
                while ( pResource.get() )
                {
                    CreateContinerObject( nContainerID, nGroupID, strFileName, pResource->strSeedID );

                    pResource = pNPCResourceList->Next();
                }
            }
        }
    }
    

    return true;
}

bool NFCSceneProcessModule::CreateContinerObject( const int nContainerID, const int nGroupID, const std::string& strFileName, const std::string& strSeedID )
{
    NF_SHARE_PTR<NFMapEx<std::string, SceneGroupResource>> pSceneResource = mtSceneResourceConfig.GetElement( nContainerID );
    if ( pSceneResource.get() )
    {
        NF_SHARE_PTR<SceneGroupResource> pGroupResource = pSceneResource->GetElement( strFileName );
        if ( pSceneResource.get() )
        {
            NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>> pResourceList = pGroupResource->xSceneGroupResource.GetElement(NFrame::NPC::ThisName());
            if ( pResourceList.get() )
            {
                NF_SHARE_PTR<SceneSeedResource> pResourceObject = pResourceList->GetElement( strSeedID );
                if ( pResourceObject.get() )
                {
                    const std::string& strClassName = m_pElementInfoModule->GetPropertyString(pResourceObject->strConfigID, NFrame::NPC::ClassName());

                    NFCDataList arg;
                    arg << NFrame::NPC::X() << pResourceObject->fSeedX;
                    arg << NFrame::NPC::Y() << pResourceObject->fSeedY;
                    arg << NFrame::NPC::Z() << pResourceObject->fSeedZ;
                    arg << NFrame::NPC::SeedID() << strSeedID;

                    m_pKernelModule->CreateObject( NFGUID(), nContainerID, nGroupID, strClassName, pResourceObject->strConfigID, arg );
                }
            }
        }
    }

    return true;
}

int NFCSceneProcessModule::CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIDataList& arg )
{
    int nTargetGroupID = -1;
    const E_SCENE_TYPE eType = GetCloneSceneType( nContainerID );
    if ( eType == SCENE_TYPE_NORMAL)
    {
        //城镇
        if ( nGroupID <= 0 )
        {
            //随服务器自动安排线路
            for ( int i = 1; i <= mnContainerLine; i++ )
            {
                int nCount = m_pKernelModule->GetSceneOnLineCount( nContainerID, i );
                if ( nCount < mnLineMaxPlayer )
                {
                    nTargetGroupID = i;
                    break;
                }
            }
        }
        else
        {
            //想换指定的线，看人数是否满了
            int nCount = m_pKernelModule->GetSceneOnLineCount( nContainerID, nGroupID );
            if ( nCount < mnLineMaxPlayer )
            {
                nTargetGroupID = nGroupID;
            }
            else
            {
                //次线人数满
                return -1;
            }
        }
    }
    else if ( eType == SCENE_TYPE_MAINLINE_CLONE)
    {
        //副本，申请分层ID
        nTargetGroupID = m_pKernelModule->RequestGroupScene( nContainerID );
    }

    if ( nTargetGroupID > 0 && eType == SCENE_TYPE_MAINLINE_CLONE)
    {
        CreateContinerObjectByFile( nContainerID, nTargetGroupID, strResourceID );
    }

    return nTargetGroupID;
}

bool NFCSceneProcessModule::DestroyCloneScene( const int& nContainerID, const int& nGroupID, const NFIDataList& arg )
{
    m_pKernelModule->ReleaseGroupScene(nContainerID, nGroupID);

    return false;
}

int NFCSceneProcessModule::OnEnterSceneEvent( const NFGUID& self, const int nEventID, const NFIDataList& var )
{
    if ( var.GetCount() != 4 
        || !var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, 
        TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return 0;
    }

    NFGUID ident = var.Object( 0 );
    int nType = var.Int( 1 );
    int nTargetScene = var.Int( 2 );
    int nTargetGroupID = var.Int( 3 );
    int nOldSceneID = m_pKernelModule->GetPropertyInt( self, NFrame::Player::SceneID());

    char szSceneID[MAX_PATH] = {0};
    sprintf(szSceneID, "%d", nTargetScene);
// #ifdef NF_USE_ACTOR
// 	int nActorID = m_pElementInfoModule->GetPropertyInt(szSceneID, "ActorID");
// 	int nSelfActorID = pPluginManager->GetActorID();
// 	if (nSelfActorID != nActorID)
// 	{
// 		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, ident, "target scene not runing in this server", nTargetScene);
// 		return 1;
// #endif

    if ( self != ident )
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, ident, "you are not you self, but you want to entry this scene", nTargetScene);
        return 1;
    }

    const int nNowContainerID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
    const int nNowGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
    if (nNowContainerID == nTargetScene
        && nNowGroupID == nTargetGroupID)
    {
        //本来就是这个层这个场景就别切换了
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "in same scene and group but it not a clone scene", nTargetScene);

        return 1;
    }

    nTargetGroupID = CreateCloneScene( nTargetScene, nTargetGroupID, "File.xml", NFCDataList() );
    if ( nTargetGroupID <= 0 )
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "CreateCloneScene failed", nTargetScene);
        return 0;
    }

    //得到坐标
    double fX = 0.0f;
    double fY = 0.0f;
    double fZ = 0.0f;

    const std::string& strRelivePosList = m_pElementInfoModule->GetPropertyString(szSceneID, NFrame::Scene::RelivePos());
    NFCDataList valueRelivePosList( strRelivePosList.c_str(), ";" );
    if ( valueRelivePosList.GetCount() >= 1 )
    {
        NFCDataList valueRelivePos( valueRelivePosList.String( 0 ).c_str(), "," );
        if ( valueRelivePos.GetCount() == 3 )
        {
            fX = boost::lexical_cast<double>( valueRelivePos.String( 0 ) );
            fY = boost::lexical_cast<double>( valueRelivePos.String( 1 ) );
            fZ = boost::lexical_cast<double>( valueRelivePos.String( 2 ) );
        }
    }

    NFCDataList xSceneResult( var );
    xSceneResult.Add( fX );
    xSceneResult.Add( fY );
    xSceneResult.Add( fZ );

    m_pKernelModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, xSceneResult );

    if(!m_pKernelModule->SwitchScene( self, nTargetScene, nTargetGroupID, fX, fY, fZ, 0.0f, var ))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "SwitchScene failed", nTargetScene);

        return 0;
    }

    xSceneResult.Set(3, NFINT64(nTargetGroupID));//spicial
    m_pKernelModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, xSceneResult );

    return 0;
}

int NFCSceneProcessModule::OnLeaveSceneEvent( const NFGUID& object, const int nEventID, const NFIDataList& var )
{
    if (1 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    NFINT32 nOldGroupID = var.Int(0);
    if (nOldGroupID > 0)
    {
        int nContainerID = m_pKernelModule->GetPropertyInt(object, NFrame::Player::SceneID());
        if (GetCloneSceneType(nContainerID) == SCENE_TYPE_MAINLINE_CLONE)
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, object, "DestroyCloneSceneGroup", nOldGroupID);
            DestroyCloneScene(nContainerID, nOldGroupID, var);
        }
    }

    return 0;
}

int NFCSceneProcessModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
        {
            //如果在副本中,则删除他的那个副本
            int nContainerID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
            if (GetCloneSceneType(nContainerID) == SCENE_TYPE_MAINLINE_CLONE)
            {
                int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
                DestroyCloneScene(nContainerID, nGroupID, NFCDataList());
            }
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            m_pKernelModule->AddEventCallBack( self, NFED_ON_CLIENT_ENTER_SCENE, this, &NFCSceneProcessModule::OnEnterSceneEvent );
            m_pKernelModule->AddEventCallBack( self, NFED_ON_CLIENT_LEAVE_SCENE, this, &NFCSceneProcessModule::OnLeaveSceneEvent );
        }
    }

    return 0;
}

E_SCENE_TYPE NFCSceneProcessModule::GetCloneSceneType( const int nContainerID )
{
    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf( szSceneIDName, "%d", nContainerID );
    if (m_pElementInfoModule->ExistElement(szSceneIDName))
    {
        return (E_SCENE_TYPE)m_pElementInfoModule->GetPropertyInt(szSceneIDName, NFrame::Scene::CanClone());
    }

    return SCENE_TYPE_ERROR;
}

bool NFCSceneProcessModule::IsCloneScene(const int nSceneID)
{
    return GetCloneSceneType(nSceneID) == 1;
}

bool NFCSceneProcessModule::LoadInitFileResource( const int& nContainerID )
{
    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf( szSceneIDName, "%d", nContainerID );

    const std::string& strSceneFilePath = m_pElementInfoModule->GetPropertyString( szSceneIDName, NFrame::Scene::FilePath() );
    const int nCanClone = m_pElementInfoModule->GetPropertyInt( szSceneIDName, NFrame::Scene::CanClone() );

    //场景对应资源
    NF_SHARE_PTR<NFMapEx<std::string, SceneGroupResource>> pSceneResourceMap = mtSceneResourceConfig.GetElement( nContainerID );
    if ( !pSceneResourceMap.get() )
    {
        pSceneResourceMap = NF_SHARE_PTR<NFMapEx<std::string, SceneGroupResource>>(NF_NEW NFMapEx<std::string, SceneGroupResource>());
        mtSceneResourceConfig.AddElement( nContainerID, pSceneResourceMap );
    }

    if ( !strSceneFilePath.empty() )
    {
        std::string strFilePathName( strSceneFilePath );
        strFilePathName.append( "File.xml" );

        NF_SHARE_PTR<SceneGroupResource> pSceneGroupResource = pSceneResourceMap->GetElement( "File.xml" );
        if ( !pSceneGroupResource.get() )
        {
            pSceneGroupResource = NF_SHARE_PTR<SceneGroupResource>(NF_NEW SceneGroupResource());
            pSceneGroupResource->bCanClone = (nCanClone > 0 ? true : false);
            pSceneResourceMap->AddElement( "File.xml", pSceneGroupResource );
        }

        rapidxml::file<> xFileSource( strFilePathName.c_str() );
        rapidxml::xml_document<>  xFileDoc;
        xFileDoc.parse<0>( xFileSource.data() );

        //资源文件列表
        rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
        for ( rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling() )
        {
            std::string strSeedFileName = pSeedFileNode->first_attribute( "ID" )->value();
            
            //资源对应种子列表
            NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>> pSeedResourceList = pSceneGroupResource->xSceneGroupResource.GetElement(strSeedFileName);
            if ( !pSeedResourceList.get() )
            {
                pSeedResourceList = NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>>(NF_NEW NFMapEx<std::string, SceneSeedResource>());
                pSceneGroupResource->xSceneGroupResource.AddElement( strSeedFileName, pSeedResourceList );
            }

            std::string strObjectFileName( strSceneFilePath );
            strObjectFileName.append( strSeedFileName );
            strObjectFileName.append( ".xml" );


            rapidxml::file<> xSeedListSource( strObjectFileName.c_str() );
            rapidxml::xml_document<>  xFileDoc;
            xFileDoc.parse<0>( xSeedListSource.data() );
            rapidxml::xml_node<>* pSeedObjectRoot = xFileDoc.first_node();
            for ( rapidxml::xml_node<>* pSeedObjectNode = pSeedObjectRoot->first_node(); pSeedObjectNode; pSeedObjectNode = pSeedObjectNode->next_sibling() )
            {
                //种子具体信息
                std::string strSeedName = pSeedObjectNode->first_attribute( "ID" )->value();
                std::string strConfigID = pSeedObjectNode->first_attribute( "NPCConfigID" )->value();

                if (!m_pElementInfoModule->ExistElement(strConfigID))
                {
                    assert(0);
                }

                NF_SHARE_PTR<SceneSeedResource> pSeedResource = pSeedResourceList->GetElement(strSeedName);
                if ( !pSeedResource.get() )
                {
                    pSeedResource = NF_SHARE_PTR<SceneSeedResource>(NF_NEW SceneSeedResource());
                    pSeedResourceList->AddElement( strSeedName, pSeedResource );
                }

                pSeedResource->strSeedID = strSeedName;
                pSeedResource->strConfigID = strConfigID;
                pSeedResource->fSeedX = boost::lexical_cast<float>(pSeedObjectNode->first_attribute( "SeedX" )->value());
                pSeedResource->fSeedY = boost::lexical_cast<float>(pSeedObjectNode->first_attribute( "SeedY" )->value());
                pSeedResource->fSeedZ = boost::lexical_cast<float>(pSeedObjectNode->first_attribute( "SeedZ" )->value());
            }
        }
    }
    
    return true;
}