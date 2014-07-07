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
    mnRoleHallContainer = -3;
    mnContainerLine = 50;//默认30条线

    mnLineMaxPlayer = 30;//每条线最大30人

    
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>( pPluginManager->FindModule( "NFCGameLogicModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>( pPluginManager->FindModule( "NFCPropertyModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pGameLogicModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pPropertyModule );
    assert( NULL != m_pLogModule );

    //     m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECTROLE_ENTER, OnEnterWorldEvent);
    //     m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LEAVE_GAME, OnAcountDisConnectEvent);

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCSceneProcessModule::OnObjectClassEvent );
    m_pEventProcessModule->AddClassCallBack( "Scene", this, &NFCSceneProcessModule::OnContainerObjectEvent );

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
    int nSelfActorID = pPluginManager->GetAddress().AsInteger();
    NFILogicClass* pLogicClass =  m_pLogicClassModule->GetElement("Scene");
    if (pLogicClass)
    {
        NFList<std::string>& list = pLogicClass->GetConfigNameList();

        std::string strData;
        bool bRet = list.First(strData);
        while (bRet)
        {
            int nSceneID = boost::lexical_cast<int>(strData);

            const std::string& strFilePath = m_pElementInfoModule->QueryPropertyString( strData, "FilePath" );
            const int nActorID = m_pElementInfoModule->QueryPropertyInt( strData, "ActorID" );
            
            if ( nActorID == nSelfActorID && nSceneID > 0 )
            {
                m_pKernelModule->CreateContainer( nSceneID, strData );
            }


            bRet = list.Next(strData);
        }
    }

    return true;
}

int NFCSceneProcessModule::OnContainerObjectEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
    {
        //容器已经创建成功，应该加载NPC了
        const std::string& strSceneFilePath = m_pKernelModule->QueryPropertyString( self, "FilePath" );
        if ( !strSceneFilePath.empty() )
        {
            int nSceneID = m_pKernelModule->QueryPropertyInt( self, "SceneID" );
            //如果特意是克隆副本，则第一次创建容器时不创建NPC
            int nCanClone = m_pKernelModule->QueryPropertyInt( self, "CanClone" );
            //if (nCanClone <= 0)
            {
                for ( int i = 0; i < 20; i++ )
                {
                    //最多20套资源
                    NFConfig loadConfig;
                    std::string strFilePathName( strSceneFilePath );
                    char szFileName[MAX_PATH] = { 0 };
                    sprintf( szFileName, "File%d.cfg", i );
                    strFilePathName.append( szFileName );
                    bool bLoad =  loadConfig.Load( strFilePathName );
                    if ( bLoad )
                    {
                        SceneGroupResource* pResourceMap = mtSceneResourceConfig.GetElement( nSceneID );
                        if ( NULL == pResourceMap )
                        {
                            pResourceMap = new SceneGroupResource();
                            pResourceMap->bCanClone = (nCanClone > 0 ? true : false);
                            mtSceneResourceConfig.AddElement( nSceneID, pResourceMap );
                        }

                        NFList<std::string>* pNPCResourceList = pResourceMap->mtSceneGroupResource.GetElement( szFileName );
                        if ( NULL == pNPCResourceList )
                        {
                            pNPCResourceList = new NFList<std::string>();
                            pResourceMap->mtSceneGroupResource.AddElement( szFileName, pNPCResourceList );
                        }

                        std::vector<std::string>::iterator it = loadConfig.vec.begin();
                        for ( ; it != loadConfig.vec.end(); it++ )
                        {
                            std::string strObjectFileName( strSceneFilePath );
                            std::string strClassFileName = *it ;
                            strObjectFileName.append( strClassFileName );
                            strObjectFileName.append( ".xml" );

                            m_pElementInfoModule->LoadSceneInfo( strObjectFileName, "NPCSeed" );

                            rapidxml::file<> fdoc( strObjectFileName.c_str() );
                            rapidxml::xml_document<>  doc;
                            doc.parse<0>( fdoc.data() );

                            rapidxml::xml_node<>* root = doc.first_node();
                            for ( rapidxml::xml_node<>* attrNode = root->first_node(); attrNode; attrNode = attrNode->next_sibling() )
                            {
                                //内存中NPC ConfigID，并与场景ID以及资源ID关联起来
                                std::string strSeedConfigID = attrNode->first_attribute( "ID" )->value();
                                std::string strNormalConfigID = attrNode->first_attribute( "NPCConfigID" )->value();
                                pNPCResourceList->Add( strSeedConfigID );
                            }
                        }
                    }
                }

                if ( nCanClone <= 0 )
                {
                    CreateContinerNPCObjectByFile( nSceneID, 0, "File0.cfg" );

                    //城镇,一上线就给11条线(0线为NPC，1-10为玩家，尼玛肯定够了，否则游戏火爆了)
                    for ( int i = 0; i < mnContainerLine; i++ )
                    {
                        m_pKernelModule->RequestGroupScene( nSceneID, "" );
                    }
                }
            }
        }
    }

    return 0;
}

int NFCSceneProcessModule::CreateContinerNPCObjectByFile( const int nContainerID, const int nGroupID, const std::string& strFileName )
{
    SceneGroupResource* pResourceMap = mtSceneResourceConfig.GetElement( nContainerID );
    if ( NULL != pResourceMap )
    {
        //std::string为NPC列表
        NFList<std::string>* pNPCResourceList = pResourceMap->mtSceneGroupResource.GetElement( strFileName );
        if ( NULL != pNPCResourceList )
        {
            std::string str;
            bool bRet = pNPCResourceList->First( str );
            while ( bRet )
            {
                CreateContinerNPCObject( nContainerID, nGroupID, str );

                bRet = pNPCResourceList->Next( str );
            }
        }
    }

    return 0;
}

bool NFCSceneProcessModule::CreateContinerNPCObject( const int nContainerID, const int nGroupID, const std::string& strSeedID )
{
    NFIPropertyManager* pSeedPropertyManager = m_pElementInfoModule->GetPropertyManager( strSeedID );
    NFIPropertyManager* pSeedClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager( "NPCSeed" );

    if ( pSeedPropertyManager && pSeedClassPropertyManager )
    {
        NFIProperty* pProperty = pSeedPropertyManager->GetElement( "NPCConfigID" );
        if ( pProperty )
        {
           const std::string& strNormalConfigID = pProperty->QueryString();
            NFIPropertyManager* pNormalPropertyManager = m_pElementInfoModule->GetPropertyManager( strNormalConfigID );
            if ( pNormalPropertyManager )
            {
               const std::string& strClassName = pNormalPropertyManager ->GetElement( "ClassName" )->QueryString();

                NFCValueList arg;
                NFIProperty* pProperty = pSeedPropertyManager->First();
                while ( pProperty )
                {
                    const std::string& strPropertyName = pProperty->GetKey();
//                     if ( "SeedX" == strPropertyName )
//                     {
//                         arg << "X";
//                         arg.Append( pProperty->GetValue() );
//                     }
//                     if ( "SeedY" == strPropertyName )
//                     {
//                         arg << "Y";
//                         arg.Append( pProperty->GetValue() );
//                     }
//                     if ( "SeedZ" == strPropertyName )
//                     {
//                         arg << "Z";
//                         arg.Append( pProperty->GetValue() );
//                     }

//                     if ( "Bacth" == strPropertyName )
//                     {
//                         arg << "Bacth";
//                         arg.Append( pProperty->GetValue() );
//                     }

//                     arg << pProperty->GetKey();
//                     arg.Append( pProperty->GetValue() );

                    pProperty = pSeedPropertyManager->Next();
                }

                arg << "NPCConfigID" << strSeedID;
                m_pKernelModule->CreateObject( 0, nContainerID, nGroupID, strClassName, strNormalConfigID, arg );
            }

        }


    }

    return true;
}

int NFCSceneProcessModule::CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIValueList& arg )
{
    int nTargetGroupID = 0;
    if ( GetCloneSceneType( nContainerID ) == SCENE_TYPE_NORMAL)
    {
        //城镇，想换指定的线
        if ( nGroupID <= 0 )
        {
            //看人数是否满了,自动分线
            for ( int i = 1; i <= 10; i++ )
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
            //随机换线
            int nCount = m_pKernelModule->GetContainerOnLineCount( nContainerID, nGroupID );
            if ( nCount < mnLineMaxPlayer )
            {
                nTargetGroupID = nGroupID;
            }
            else
            {
                //次线人数满
                return 0;
            }
        }
    }
    else
    {
        //副本，申请分层ID
        nTargetGroupID = m_pKernelModule->RequestGroupScene( nContainerID, "" );
    }

    if ( nTargetGroupID > 0
		&& GetCloneSceneType( nContainerID ) == SCENE_TYPE_MAINLINE_CLONE)
    {
       CreateContinerNPCObjectByFile( nContainerID, nTargetGroupID, strResourceID );
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
        std::string strObjClassName = m_pKernelModule->QueryPropertyString( ident, "ClassName" );
        if ( "Player" != strObjClassName )
        {
            m_pKernelModule->DestroyObject(ident);
        }
//         else
//         {
//             m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "There is no object",  __FUNCTION__, __LINE__);
//         }
    }

    m_pKernelModule->ReleaseGroupScene(nContainerID, nGroupID);

    return false;
}

int NFCSceneProcessModule::OnEnterSceneEvent( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    if ( var.GetCount() != 5 )
    {
        return 0;
    }

    NFIDENTID ident = var.NumberVal<NFINT64>( 0 );
    int nType = var.NumberVal<int>( 1 );
    int nTargetScene = var.NumberVal<int>( 2 );
    int nTargetGroupID = var.NumberVal<int>( 3 );
    int nSceneType = var.NumberVal<int>( 4 );
    int nOldSceneID = m_pKernelModule->QueryPropertyInt( self, "SceneID" );

    int nSelfActorID = pPluginManager->GetActor();
    int nActorID = m_pGameServerConfigModule->GetActorID( nTargetScene );
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


    std::string strResource = "";
    if ( GetCloneSceneType(nTargetScene) == SCENE_TYPE_MAINLINE_CLONE)
    {
        strResource = "File0.cfg";
    }
    else
    {
        //不是副本，那么是否是同一场景同层
        const int nNowContainerID = m_pKernelModule->QueryPropertyInt(self, "SceneID");
        const int nNowGroupID = m_pKernelModule->QueryPropertyInt(self, "GroupID");
        if (nNowContainerID == nTargetScene
            && nNowGroupID == nTargetGroupID)
        {
            //本来就是这个层这个场景就别切换了
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "in same scene and group but it not a clone scene", nTargetScene);

             return 1;
        }
    }

    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf( szSceneIDName, "%d", nTargetScene );
    NFIPropertyManager* propertyManager = m_pElementInfoModule->GetPropertyManager( szSceneIDName );
    if ( propertyManager )
    {
        NFIProperty* pProperty = propertyManager->GetElement( "RelivePos" );
        if ( pProperty )
        {
            bool bCanEnter = true;
            if ( bCanEnter )
            {
                nTargetGroupID = CreateCloneScene( nTargetScene, nTargetGroupID, strResource, NFCValueList() );
                if ( nTargetGroupID <= 0 )
                {
                    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "CreateCloneScene failed", nTargetScene);
					return 0;
				}
                {
                    //得到坐标
                    float fX = 0.0f;
                    float fY = 0.0f;
                    float fZ = 0.0f;
                    const std::string& strRelivePosList = pProperty->QueryString();
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

                    NFCValueList sceneResult( var );
                    sceneResult.AddFloat( fX );
                    sceneResult.AddFloat( fY );
                    sceneResult.AddFloat( fZ );

                    m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, sceneResult );

                    if(!m_pKernelModule->SwitchScene( self, nTargetScene, nTargetGroupID, fX, fY, fZ, 0.0f, var ))
                    {
                        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "SwitchScene failed", nTargetScene);

                        return 0;
                    }

                    sceneResult.SetInt(3, nTargetGroupID);
                    m_pEventProcessModule->DoEvent( self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, sceneResult );


                    if ( GetCloneSceneType( nTargetScene ) == SCENE_TYPE_MAINLINE_CLONE )
                    {
                        m_pKernelModule->SetPropertyInt(self, "LastCloneSceneID", nTargetScene);
                        m_pKernelModule->SetPropertyInt(self, "LastCloneSceneMode", nSceneType);

                        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "CreateCloneSceneGroup", nTargetGroupID);
                    }
                    else
                    {
                        m_pKernelModule->SetPropertyInt(self, "LastContainerID", nTargetScene);
                    }
                }
            }
        }
    }

    return 0;
}

int NFCSceneProcessModule::OnLeaveSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if (1 != var.GetCount()
        || !var.TypeEx(VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return -1;
    }

    NFINT32 nOldGroupID = var.IntVal(0);

    NFIObject* pObject = m_pKernelModule->GetObject(object);
    if (pObject && nOldGroupID > 0)
    {
        int nContainerID = pObject->QueryPropertyInt("SceneID");
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
            int nContainerID = m_pKernelModule->QueryPropertyInt(self, "SceneID");
            if (GetCloneSceneType(nContainerID) == SCENE_TYPE_MAINLINE_CLONE)
            {
                int nGroupID = m_pKernelModule->QueryPropertyInt(self, "GroupID");

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);
                DestroyCloneScene(nContainerID, nGroupID, NFCValueList());
            }
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            m_pEventProcessModule->AddEventCallBack( self, NFED_ON_OBJECT_ENTER_SCENE, this, &NFCSceneProcessModule::OnEnterSceneEvent );
            m_pEventProcessModule->AddEventCallBack( self, NFED_ON_OBJECT_LEAVE_SCENE, this, &NFCSceneProcessModule::OnLeaveSceneEvent );
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
		return (E_SCENE_TYPE)m_pElementInfoModule->QueryPropertyInt(szSceneIDName, "SceneType");
	}

    return SCENE_TYPE_ERROR;
}