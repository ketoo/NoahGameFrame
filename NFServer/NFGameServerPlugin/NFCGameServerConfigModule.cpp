// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerConfigModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCGameServerConfigModule
//
// -------------------------------------------------------------------------

#include "NFCGameServerConfigModule.h"
#include "NFComm/Config/NFConfig.h"

bool NFCGameServerConfigModule::Init()
{

    

    return true;
}

bool NFCGameServerConfigModule::Shut()
{
    return true;
}

bool NFCGameServerConfigModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCGameServerConfigModule::AfterInit()
{
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogModule );

    Load();

    return true;
}

int NFCGameServerConfigModule::GetActorID( const int nSceneID )
{
    int* pInt = mtSceneConfig.GetElement( nSceneID );
    if ( pInt )
    {
        return *pInt;
    }

    return 0;
}

bool NFCGameServerConfigModule::AddSceneToActor( const int nSceneID, const int nGameServer )
{
    int* pInt = new int();
    mtSceneConfig.AddElement( nSceneID, pInt );
    //int* pInt = mtSceneConfig.CreateElement( nSceneID );
    *pInt = nGameServer;

    return true;
}

void NFCGameServerConfigModule::Load()
{
    rapidxml::file<> fdoc( "../../NFDataCfg/Ini/GSSceneConfig.xml" );
    rapidxml::xml_document<>  doc;
    doc.parse<0>( fdoc.data() );

    rapidxml::xml_node<>* root = doc.first_node();
    for ( rapidxml::xml_node<>* jobNode = root->first_node(); jobNode; jobNode = jobNode->next_sibling() )
    {
        const char* strSceneID = jobNode->first_attribute( "SceneID" )->value();
        const char* strActorID = jobNode->first_attribute( "ActorID" )->value();

        int nSceneID = boost::lexical_cast<int>( strSceneID );
        int nActorID = boost::lexical_cast<int>( strActorID );

        char szSceneIDName[MAX_PATH] = { 0 };
        sprintf( szSceneIDName, "%d", nSceneID );

        if (!m_pElementInfoModule->ExistElement(szSceneIDName))
        {
            m_pLogModule->LogElement(NFILogModule::NLL_ERROR_NORMAL, 0, szSceneIDName, "There is no element", __FUNCTION__, __LINE__);
            assert(0);
        }

        this->AddSceneToActor( nSceneID, nActorID );
    }
}
