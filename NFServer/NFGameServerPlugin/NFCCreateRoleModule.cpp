// -------------------------------------------------------------------------
//    @FileName         :    NFCCreateRoleModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-03-29
//    @Module               :    NFCCreateRoleModule
//    @Desc                 :
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "NFCCreateRoleModule.h"

//

int NFCCreateRoleModule::mnConnectContainer = -1;
int NFCCreateRoleModule::mnRoleHallContainer = -3;

NFIGameLogicModule* NFCCreateRoleModule::m_pGameLogicModule = NULL;
NFIKernelModule* NFCCreateRoleModule::m_pKernelModule = NULL;
//NFIDataBaseModule* NFCCreateRoleModule::m_pDataBaseModule = NULL;
NFIDataNoSqlModule* NFCCreateRoleModule::m_pNoSqlModule = NULL;
NFIEventProcessModule* NFCCreateRoleModule::m_pEventProcessModule = NULL;
NFIElementInfoModule* NFCCreateRoleModule::m_pElementInfoModule = NULL;
NFCCreateRoleModule* NFCCreateRoleModule::m_pThis = NULL;

bool NFCCreateRoleModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pNoSqlModule = dynamic_cast<NFIDataNoSqlModule*>( pPluginManager->FindModule( "NFCDataNoSqlModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>( pPluginManager->FindModule( "NFCGameLogicModule" ) );

    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pNoSqlModule );
    assert( NULL != m_pKernelModule );

    m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, OnLoadRoleBeginEvent );
    m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_DATABASE_SERVER_LOADROE_FINAL_RESULTS, OnLoadRoleFinalEvent );
    m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_CREATEROLE, OnCreateRoleEvent );
    m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_DELETEROLE, OnDeleteRoleEvent );
    m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_LEAVE_GAME, OnAcountDisConnectEvent );

    return true;
}

bool NFCCreateRoleModule::Shut()
{
    return true;
}

bool NFCCreateRoleModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

int NFCCreateRoleModule::OnLoadRoleFinalEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if ( 9 != var.GetCount() )
    {
        return -1;
    }

    //赋予属性
    const char* pstrAccount = var.StringVal( 0 );
    NFCValueList valueInfo;
    valueInfo << pstrAccount;

    int nCount = 0;
    for ( int i = 1; i <= 7; i += 2 )
    {
        const char* pstrRoleName = var.StringVal( i );
        if ( strlen( pstrRoleName ) > 0 )
        {
            //看容器中是否已经存在，存在则不创建
            NFCValueList varHallObjectList;
            NFCValueList varHalvalueInfo;
            varHalvalueInfo << pstrRoleName;
            int nHallObjectCount = m_pKernelModule->GetObjectByProperty( mnRoleHallContainer, "RoleName", varHalvalueInfo, varHallObjectList );
            if ( nHallObjectCount > 0 )
            {

                for ( int j = 0; j < varHallObjectList.GetCount(); j++ )
                {
                    m_pKernelModule->LogErrorObject( varHallObjectList.ObjectVal( j ) );
                    m_pKernelModule->DestroyObject( varHallObjectList.ObjectVal( j ) );
                }
            }

            GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager( "Scene0" )->GetElement( "RelivePos" );
            const char* pstrRelivePos = gtPproperty->QueryString();
            NFCValueList valueRelivePos( pstrRelivePos, "," );

            char szConfigIindex[MAX_PATH] = { 0 };
            sprintf( szConfigIindex, "%d", var.IntVal( i + 1 ) );

            NFCValueList arg;
            arg << "Account" << pstrAccount;
            arg << "RoleName" << pstrRoleName;
            arg << "SceneID" << mnRoleHallContainer;
            arg << "X" << atof( valueRelivePos.StringVal( 0 ) );
            arg << "Y" << atof( valueRelivePos.StringVal( 1 ) );
            arg << "Z" << atof( valueRelivePos.StringVal( 2 ) );
            m_pKernelModule->CreateObject( 0, mnRoleHallContainer, 0, "Player", szConfigIindex, arg );

            nCount++;
        }
    }

    char szInfo[MAX_PATH] = { 0 };
    sprintf( szInfo, "Load data final, %s: have %d role.", pstrAccount, nCount );
    m_pKernelModule->LogInfo( szInfo );

    return 0;
}

bool NFCCreateRoleModule::AfterInit()
{
    m_pKernelModule->CreateContainer( mnRoleHallContainer, "" );

    m_pKernelModule->LogInfo( " -3 RoleHallContainer " );

    return true;
}

int NFCCreateRoleModule::OnCreateRoleEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if ( 6 != var.GetCount() )
    {
        return 0;
    }

    //如果有4个玩家则不让创建
    const char* pstrAccountName = var.StringVal( 0 );
    const char* pstrRoleName = var.StringVal( 1 );
    int nRoleSex = var.IntVal( 2 );
    int nRoleJob = var.IntVal( 3 );
    int nRoleRace = var.IntVal( 4 );
    int nRoleCamp = var.IntVal( 5 );

    NFCValueList roleLlist;
    if ( m_pNoSqlModule->QueryAccountRoleList( pstrAccountName, roleLlist ) >= 4 )
        //if (m_pDataBaseModule->QueryAccountRoleList(pstrAccountName, roleLlist) >= 4)
    {
        return 0;
    }
    //////////////////////////////////////////////////////////////////////////
    //创建人物直接走了数据库
    //////////////////////////////////////////////////////////////////////////
    if ( m_pNoSqlModule->CreateRole( pstrAccountName, pstrRoleName ) <= 0 )
        //if (m_pDataBaseModule->CreateRole(pstrAccountName, pstrRoleName) <= 0)
    {
        return 0;
    }

    NFCValueList varPropertyKeyList;
    NFCValueList varPropertyValueList;

    varPropertyKeyList << "RoleName"
                       << "Sex"
                       << "Job"
                       << "Race"
                       << "Camp"
                       << "SceneID"
                       << "LastContainerID"
                       << "ClassName";

    varPropertyValueList << pstrRoleName
                         << nRoleSex
                         << nRoleJob
                         << nRoleRace
                         << nRoleCamp
                         << mnRoleHallContainer
                         << 1
                         << "Player";

    int nConfigName = nRoleJob + nRoleSex * 10;
    char szConfigName[MAX_PATH] = { 0 };
    sprintf( szConfigName, "%d", nConfigName );

    //弄到所有的属性
    GTPropertyManager* pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager( szConfigName );
    if ( pConfigPropertyManager )
    {
        GTProperty* pConfigPropertyInfo = pConfigPropertyManager->First();
        while ( pConfigPropertyInfo )
        {
            //看属性是否需要保存,不需要保存的就别村
            if ( !pConfigPropertyInfo->GetSave() )
            {
                pConfigPropertyInfo = pConfigPropertyManager->Next();
                continue;
            }
            //这个属性RoleName是玩家的，因此，这里不能有RoleName
            const NFIValueList& valueList = pConfigPropertyInfo->GetValue();


            if ( 0 != strcmp( pConfigPropertyInfo->GetKey(), "RoleName" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "Sex" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "Job" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "Race" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "Camp" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "Account" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "SceneID" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "LastContainerID" )
                 && 0 != strcmp( pConfigPropertyInfo->GetKey(), "ClassName" ) )
            {
                varPropertyKeyList << pConfigPropertyInfo->GetKey();
                varPropertyValueList.Append( valueList, 0, 1 );
            }

            pConfigPropertyInfo = pConfigPropertyManager->Next();
        }
    }
    varPropertyKeyList << "RoleName";
    varPropertyValueList << pstrRoleName;
    varPropertyKeyList << "Sex";
    varPropertyValueList << nRoleSex;
    varPropertyKeyList << "Job";
    varPropertyValueList << nRoleJob;
    varPropertyKeyList << "Race";
    varPropertyValueList << nRoleRace;
    varPropertyKeyList << "Camp";
    varPropertyValueList << nRoleCamp;
    varPropertyKeyList << "SceneID";
    varPropertyValueList << mnRoleHallContainer;
    varPropertyKeyList << "LastContainerID";
    varPropertyValueList << 1;//1号场景为新手村
    varPropertyKeyList << "ClassName";
    varPropertyValueList << "Player";
    varPropertyKeyList << "Level";
    varPropertyValueList << 1;

    m_pNoSqlModule->SetRoleProperty( pstrRoleName, varPropertyKeyList, varPropertyValueList );

    //////////////////////////////////////////////////////////////////////////
    GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager( "Scene0" )->GetElement( "RelivePos" );
    const char* pstrRelivePos = gtPproperty->QueryString();
    NFCValueList valueRelivePos( pstrRelivePos, "," );

    NFCValueList arg;
    arg << "Account" << pstrAccountName;
    arg << "RoleName" << pstrRoleName;
    arg << "SceneID" << mnRoleHallContainer;
    arg << "X" << atof( valueRelivePos.StringVal( 0 ) );
    arg << "Y" << atof( valueRelivePos.StringVal( 1 ) );
    arg << "Z" << atof( valueRelivePos.StringVal( 2 ) );
    m_pKernelModule->CreateObject( 0, mnRoleHallContainer, 0, "Player", szConfigName, arg );
    //nosql 则不需要这样了
    //m_pEventProcessModule->DoEvent(0, NFED_ON_DATABASE_SERVER_LOADROE_BEGIN, NFCValueList() << pstrAccountName);

    return 0;
}

int NFCCreateRoleModule::OnDeleteRoleEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if ( 2 != var.GetCount() )
    {
        return 0;
    }

    const char* pstrAccountName = var.StringVal( 0 );
    const char* pstrRoleName = var.StringVal( 1 );

    NFCValueList valObjctList;
    m_pKernelModule->GetObjectByProperty( mnRoleHallContainer, "RoleName", NFCValueList() << pstrRoleName, valObjctList );
    if ( valObjctList.GetCount() == 1 )
    {
        m_pNoSqlModule->DeleteRole( pstrAccountName, pstrRoleName );
        //m_pDataBaseModule->DeleteRole(pstrAccountName, pstrRoleName);
        m_pKernelModule->DestroyObject( valObjctList.ObjectVal( 0 ) );
        //m_pEventProcessModule->DoEvent( 0, NFED_ON_CLIENT_DELETEROLE_RESULTS, NFCValueList() << pstrAccountName << pstrRoleName );
    }

    return 0;
}

int NFCCreateRoleModule::OnAcountDisConnectEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if ( var.GetCount() == 3 )
    {
        const char* pstrAccount = var.StringVal( 0 );
        const char* pstrRoleName = var.StringVal( 1 );
        NFIDENTID ident = var.ObjectVal( 2 );

        if ( strlen( pstrAccount ) > 0 )
        {
            if ( m_pGameLogicModule->GetGameID() == 1 )
            {

                if ( !ident.IsNull() )
                {
                    m_pKernelModule->DestroyObject( ident );
                }
                else
                {
                    NFCValueList varHallObjectList;
                    NFCValueList varHalvalueInfo;
                    varHalvalueInfo << pstrAccount;

                    int nHallObjectCount = m_pKernelModule->GetObjectByProperty( mnRoleHallContainer, "Account", varHalvalueInfo, varHallObjectList );
                    for ( int i  = 0; i < varHallObjectList.GetCount(); i++ )
                    {
                        NFIDENTID identHall = varHallObjectList.ObjectVal( i );
                        if ( ident.nSerial == m_pGameLogicModule->GetGameID()
                             && m_pKernelModule->GetObject( identHall ) )
                        {
                            m_pKernelModule->DestroyObject( identHall );
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int NFCCreateRoleModule::OnLoadRoleBeginEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    //直接从NOSQL数据库拉
    const char* pstrAccount = var.StringVal( 0 );
    NFCValueList roleLlist;
    if ( m_pNoSqlModule->QueryAccountRoleList( pstrAccount, roleLlist ) > 0 )
    {
        for ( int i = 0; i < roleLlist.GetCount(); i++ )
        {
            const char* pstrRoleName = roleLlist.StringVal( i );
            if ( strlen( pstrRoleName ) > 0 )
            {
                //看容器中是否已经存在，存在则不创建
                NFCValueList varHallObjectList;
                NFCValueList varHalvalueInfo;
                varHalvalueInfo << pstrRoleName;
                int nHallObjectCount = m_pKernelModule->GetObjectByProperty( mnRoleHallContainer, "RoleName", varHalvalueInfo, varHallObjectList );
                if ( nHallObjectCount > 0 )
                {
                    for ( int j = 0; j < varHallObjectList.GetCount(); j++ )
                    {
                        m_pKernelModule->LogErrorObject( varHallObjectList.ObjectVal( j ) );
                        m_pKernelModule->DestroyObject( varHallObjectList.ObjectVal( j ) );
                    }
                }

                GTProperty* gtPproperty = m_pElementInfoModule->GetPropertyManager( "Scene0" )->GetElement( "RelivePos" );
                const char* pstrRelivePos = gtPproperty->QueryString();
                NFCValueList valueRelivePos( pstrRelivePos, "," );

                char szConfigIindex[MAX_PATH] = { 0 };
                sprintf( szConfigIindex, "%d", var.IntVal( i + 1 ) );

                NFCValueList arg;
                arg << "Account" << pstrAccount;
                arg << "RoleName" << pstrRoleName;
                arg << "SceneID" << mnRoleHallContainer;
                arg << "X" << atof( valueRelivePos.StringVal( 0 ) );
                arg << "Y" << atof( valueRelivePos.StringVal( 1 ) );
                arg << "Z" << atof( valueRelivePos.StringVal( 2 ) );
                m_pKernelModule->CreateObject( 0, mnRoleHallContainer, 0, "Player", szConfigIindex, arg );
            }
        }
    }

    return 0;
}
