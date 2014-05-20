// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelConfigModule
//
// -------------------------------------------------------------------------

#include "NFCLevelConfigModule.h"
#include "NFComm/Config/NFConfig.h"

bool NFCLevelConfigModule::Init()
{

    return true;
}


bool NFCLevelConfigModule::Shut()
{
    return true;
}

bool NFCLevelConfigModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCLevelConfigModule::AfterInit()
{
    Load();
    //m_pEventProcessModule->AddClassCallBack("Player", OnClassObjectEvent);

    return true;
}

int NFCLevelConfigModule::Load()
{
    rapidxml::file<> fdoc( "../../NFDataCfg/Ini/NFZoneServer/LevelUpExp.xml" );
    rapidxml::xml_document<>  doc;
    doc.parse<0>( fdoc.data() );

    rapidxml::xml_node<>* root = doc.first_node();
    for ( rapidxml::xml_node<>* jobNode = root->first_node(); jobNode; jobNode = jobNode->next_sibling() )
    {
        const char* strLevel = jobNode->first_attribute( "Level" )->value();
        const char* strExp = jobNode->first_attribute( "Exp" )->value();

        int nLevel = boost::lexical_cast<int>( strLevel );
        int nExp = boost::lexical_cast<int>( strExp );

        int* pInt = new int();
        *pInt = nExp;
        this->AddElement( nLevel, pInt );
    }

    return 0;
}

int NFCLevelConfigModule::GetNeedExp( int nLevel )
{
    int* pInt = GetElement( nLevel );
    if ( pInt )
    {
        return *pInt;
    }

    return 0;
}
