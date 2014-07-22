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

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>( pPluginManager->FindModule( "NFCPropertyModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCSceneProcessModule::OnObjectClassEvent );

    return true;
}

bool NFCSceneProcessModule::Shut()
{
    return true;
}

bool NFCSceneProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCSceneProcessModule::AfterInit()
{
    //初始化场景容器
    int nSelfActorID = pPluginManager->GetActorID();
    NFILogicClass* pLogicClass =  m_pLogicClassModule->GetElement("Scene");
    if (pLogicClass)
    {
        NFList<std::string>& list = pLogicClass->GetConfigNameList();

        std::string strData;
        bool bRet = list.First(strData);
        while (bRet)
        {
            int nSceneID = boost::lexical_cast<int>(strData);

            const std::string& strFilePath = m_pElementInfoModule->GetPropertyString( strData, "FilePath" );
            const int nActorID = m_pElementInfoModule->GetPropertyInt( strData, "ActorID" );

            if ( nActorID == nSelfActorID && nSceneID > 0 )
            {
                LoadInitFileResource( nSceneID );

                m_pKernelModule->CreateContainer( nSceneID, strData );

                if ( E_SCENE_TYPE::SCENE_TYPE_NORMAL == GetCloneSceneType(nSceneID) )
                {
                    for (int i = 1; i <= mnContainerLine; ++i)
                    {
                        const int nTargetGroupID = m_pKernelModule->RequestGroupScene( nSceneID);
                        const int nCreateGroupID = CreateCloneScene( nSceneID, nTargetGroupID, "File.xml", NFCValueList() );
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
    NFMap<std::string, SceneGroupResource>* pSceneResource = mtSceneResourceConfig.GetElement( nContainerID );
    if ( NULL != pSceneResource )
    {
        SceneGroupResource* pResourceMap = pSceneResource->GetElement( strFileName );
        if ( NULL != pResourceMap )
        {
            NFMap<std::string, SceneSeedResource>* pNPCResourceList = pResourceMap->xSceneGroupResource.GetElement( "NPC" );
            if ( NULL != pNPCResourceList )
            {
                SceneSeedResource* pResource = pNPCResourceList->First( );
                while ( pResource )
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
    NFMap<std::string, SceneGroupResource>* pSceneResource = mtSceneResourceConfig.GetElement( nContainerID );
    if ( NULL != pSceneResource )
    {
        SceneGroupResource* pGroupResource = pSceneResource->GetElement( strFileName );
        if ( NULL != pSceneResource )
        {
            NFMap<std::string, SceneSeedResource>* pResourceList = pGroupResource->xSceneGroupResource.GetElement( "NPC" );
            if ( NULL != pResourceList )
            {
                SceneSeedResource* pResourceObject = pResourceList->GetElement( strSeedID );
                if ( NULL != pResourceObject )
                {
                    const std::string& strClassName = m_pElementInfoModule->GetPropertyString(pResourceObject->strConfigID, "ClassName");

                    NFCValueList arg;
                    arg << "X" << pResourceObject->fSeedX;
                    arg << "Y" << pResourceObject->fSeedY;
                    arg << "Z" << pResourceObject->fSeedZ;
                    arg << "SeedID" << strSeedID;

                    m_pKernelModule->CreateObject( 0, nContainerID, nGroupID, strClassName, pResourceObject->strConfigID, arg );
                }
            }
        }
    }

    return true;
}

int NFCSceneProcessModule::CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIValueList& arg )
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
                int nCount = m_pKernelModule->GetContainerOnLineCount( nContainerID, i );
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
            int nCount = m_pKernelModule->GetContainerOnLineCount( nContainerID, nGroupID );
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

bool NFCSceneProcessModule::DestroyCloneScene( const int& nContainerID, const int& nGroupID, const NFIValueList& arg )
{
    //无条件删除    
    NFCValueList valueAllObjectList;
    m_pKernelModule->GetGroupObjectList( nContainerID, nGroupID, valueAllObjectList );
    for (int i = 0; i < valueAllObjectList.GetCount(); ++i)
    {
        NFIDENTID ident = valueAllObjectList.ObjectVal( i );
        std::string strObjClassName = m_pKernelModule->GetPropertyString( ident, "ClassName" );
        if ( "Player" != strObjClassName )
        {
            m_pKernelModule->DestroyObject(ident);
        }
    }

    m_pKernelModule->ReleaseGroupScene(nContainerID, nGroupID);

    return false;
}

int NFCSceneProcessModule::OnEnterSceneEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    if ( var.GetCount() != 4 
        || !var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, 
        TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return 0;
    }

    NFIDENTID ident = var.NumberVal<NFINT64>( 0 );
    int nType = var.NumberVal<int>( 1 );
    int nTargetScene = var.NumberVal<int>( 2 );
    int nTargetGroupID = var.NumberVal<int>( 3 );
    int nOldSceneID = m_pKernelModule->GetPropertyInt( self, "SceneID" );

    char szSceneID[MAX_PATH] = {0};
    sprintf(szSceneID, "%d", nTargetScene);
    int nActorID = m_pElementInfoModule->GetPropertyInt(szSceneID, "ActorID");
    int nSelfActorID = pPluginManager->GetActorID();
    if (nSelfActorID != nActorID)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, ident, "target scene not runing in this server", nTargetScene);
        return 1;
    }

    if ( self != ident )
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, ident, "you are not you self, but you want to entry this scene", nTargetScene);
        return 1;
    }

    const int nNowContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
    const int nNowGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");
    if (nNowContainerID == nTargetScene
        && nNowGroupID == nTargetGroupID)
    {
        //本来就是这个层这个场景就别切换了
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "in same scene and group but it not a clone scene", nTargetScene);

        return 1;
    }

    nTargetGroupID = CreateCloneScene( nTargetScene, nTargetGroupID, "File.xml", NFCValueList() );
    if ( nTargetGroupID <= 0 )
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "CreateCloneScene failed", nTargetScene);
        return 0;
    }

    //得到坐标
    float fX = 0.0f;
    float fY = 0.0f;
    float fZ = 0.0f;

    const std::string& strRelivePosList = m_pElementInfoModule->GetPropertyString(szSceneID, "RelivePos");
    NFCValueList valueRelivePosList( strRelivePosList.c_str(), ";" );
    if ( valueRelivePosList.GetCount() >= 1 )
    {
        NFCValueList valueRelivePos( valueRelivePosList.StringVal( 0 ).c_str(), "," );
        if ( valueRelivePos.GetCount() == 3 )
        {
            fX = boost::lexical_cast<float>( valueRelivePos.StringVal( 0 ) );
            fY = boost::lexical_cast<float>( valueRelivePos.StringVal( 1 ) );
            fZ = boost::lexical_cast<float>( valueRelivePos.StringVal( 2 ) );
        }
    }

    NFCValueList xSceneResult( var );
    xSceneResult.AddFloat( fX );
    xSceneResult.AddFloat( fY );
    xSceneResult.AddFloat( fZ );

    m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, xSceneResult );

    if(!m_pKernelModule->SwitchScene( self, nTargetScene, nTargetGroupID, fX, fY, fZ, 0.0f, var ))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "SwitchScene failed", nTargetScene);

        return 0;
    }

    xSceneResult.SetInt(3, nTargetGroupID);//spicial
    m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, xSceneResult );

    return 0;
}

int NFCSceneProcessModule::OnLeaveSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if (1 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    NFINT32 nOldGroupID = var.IntVal(0);

    NFIObject* pObject = m_pKernelModule->GetObject(object);
    if (pObject && nOldGroupID > 0)
    {
        int nContainerID = pObject->GetPropertyInt("SceneID");
        if (GetCloneSceneType(nContainerID) == SCENE_TYPE_MAINLINE_CLONE)
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, object, "DestroyCloneSceneGroup", nOldGroupID);
            DestroyCloneScene(nContainerID, nOldGroupID, var);
        }
    }

    return 0;
}

int NFCSceneProcessModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
        {
            //如果在副本中,则删除他的那个副本
            int nContainerID = m_pKernelModule->GetPropertyInt(self, "SceneID");
            if (GetCloneSceneType(nContainerID) == SCENE_TYPE_MAINLINE_CLONE)
            {
                int nGroupID = m_pKernelModule->GetPropertyInt(self, "GroupID");

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
                DestroyCloneScene(nContainerID, nGroupID, NFCValueList());
            }
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_ENTER_SCENE, this, &NFCSceneProcessModule::OnEnterSceneEvent );
            m_pEventProcessModule->AddEventCallBack( self, NFED_ON_CLIENT_LEAVE_SCENE, this, &NFCSceneProcessModule::OnLeaveSceneEvent );
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
        return (E_SCENE_TYPE)m_pElementInfoModule->GetPropertyInt(szSceneIDName, "SceneType");
    }

    return SCENE_TYPE_ERROR;
}

bool NFCSceneProcessModule::LoadInitFileResource( const int& nContainerID )
{
    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf( szSceneIDName, "%d", nContainerID );

    const std::string& strSceneFilePath = m_pElementInfoModule->GetPropertyString( szSceneIDName, "FilePath" );
    const int nCanClone = m_pElementInfoModule->GetPropertyInt( szSceneIDName, "CanClone" );

    //场景对应资源
    NFMap<std::string, SceneGroupResource>* pSceneResourceMap = mtSceneResourceConfig.GetElement( nContainerID );
    if ( NULL == pSceneResourceMap )
    {
        pSceneResourceMap = new NFMap<std::string, SceneGroupResource>();
        mtSceneResourceConfig.AddElement( nContainerID, pSceneResourceMap );
    }

    if ( !strSceneFilePath.empty() )
    {
        std::string strFilePathName( strSceneFilePath );
        strFilePathName.append( "File.xml" );

        SceneGroupResource* pSceneGroupResource = pSceneResourceMap->GetElement( "File.xml" );
        if ( NULL == pSceneGroupResource )
        {
            pSceneGroupResource = new SceneGroupResource();
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
            NFMap<std::string, SceneSeedResource>* pSeedResourceList = pSceneGroupResource->xSceneGroupResource.GetElement(strSeedFileName);
            if ( NULL == pSeedResourceList )
            {
                pSeedResourceList = new NFMap<std::string, SceneSeedResource>;
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

                SceneSeedResource* pSeedResource = pSeedResourceList->GetElement(strSeedName);
                if ( NULL == pSeedResource )
                {
                    pSeedResource = new SceneSeedResource();
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