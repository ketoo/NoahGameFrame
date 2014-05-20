// -------------------------------------------------------------------------
//    @FileName      :    NFCPropertyConfigModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-30
//    @Module           :    NFCPropertyConfigModule
//
// -------------------------------------------------------------------------

#include "NFCPropertyConfigModule.h"

bool NFCPropertyConfigModule::Init()
{
    
    return true;
}


bool NFCPropertyConfigModule::Shut()
{
    return true;
}

bool NFCPropertyConfigModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCPropertyConfigModule::AfterInit()
{
    Load();

    InitViewPropertyList();
    return true;
}

void NFCPropertyConfigModule::InitViewPropertyList()
{
    mViewPropertyList.Add("MAXHP");
    mViewPropertyList.Add("HPREGEN");
    mViewPropertyList.Add("MAXSP");
    mViewPropertyList.Add("SPREGEN");
    mViewPropertyList.Add("ATK_VALUE");
    mViewPropertyList.Add("DEF_VALUE");
    mViewPropertyList.Add("DODGE_VALUE");
    mViewPropertyList.Add("CRITICAL_VALUE");
    mViewPropertyList.Add("CRITICAL_RATIO_VALUE");
    mViewPropertyList.Add("DEF_CRITICAL_RATIO_VALUE");
    mViewPropertyList.Add("MOVE_SPEED");
    mViewPropertyList.Add("PHYSICAL_BREAK");
    mViewPropertyList.Add("Level");
    mViewPropertyList.Add("Sex");
    mViewPropertyList.Add("FightValue");
}

bool NFCPropertyConfigModule::NeedView(const std::string& strProperty)
{
    return mViewPropertyList.Find(strProperty);
}

// int NFCPropertyConfigModule::CalculateBaseValue( const NFJobType nJob, const int nLevel, NFRumTimeColIndex eProperty)
// {
//     if (nJob >= 0 && nJob < NFJobType::NJT_MAX)
//     {
// //         if (eProperty >= 0 && eProperty < NFRumTimeColIndex::NFRTC_END)
// //         {
// //             Coefficien* pCoefficien = mhtCoefficienData[nJob].GetElement(nLevel);
// //             if (pCoefficien)
// //             {
// //                 return pCoefficien->nCoefficienValue * nLevel + pCoefficien->nConstantValue;
// //             }
// //         }
//     }
//
//     return 0;
// }

int NFCPropertyConfigModule::CalculateBaseValue( const NFJobType nJob, const int nLevel, const std::string& strProperty )
{
    if ( nJob >= 0 && nJob < NFJobType::NJT_MAX )
    {
        JOB_PROPERTY_STRUCT* pCoefficien = mhtCoefficienData[nJob].GetElement( nLevel );
        if ( pCoefficien )
        {
            JOB_PROPERTY_STRUCT::iterator it = pCoefficien->find(strProperty);
            if (it != pCoefficien->end())
            {
                return it->second;
            }
        }
    }
    return 0;
}

void NFCPropertyConfigModule::Load()
{
    rapidxml::file<> fdoc( "../../NFDataCfg/Ini/NFZoneServer/InitBaseCoefficien.xml" );
    rapidxml::xml_document<>  doc;
    doc.parse<0>( fdoc.data() );

    rapidxml::xml_node<>* root = doc.first_node();
    for ( rapidxml::xml_node<>* jobNode = root->first_node(); jobNode; jobNode = jobNode->next_sibling() )
    {
        const char* strJob = jobNode->first_attribute( "Job" )->value();
        const char* strLevel = jobNode->first_attribute( "Level" )->value();
        int nJob = boost::lexical_cast<int>( strJob );
        int nLevel = boost::lexical_cast<int>( strLevel );
        if ( nJob >= 0 && nJob < NFJobType::NJT_MAX )
        {
            JOB_PROPERTY_STRUCT* structProperty = new JOB_PROPERTY_STRUCT();
            for ( rapidxml::xml_attribute<>* levelAttribute = jobNode->first_attribute(); levelAttribute; levelAttribute = levelAttribute->next_attribute())
            {
                if (0 == strcmp( levelAttribute->name(), "Job" )
                    || 0 == strcmp( levelAttribute->name(), "Level" ))
                {
                    continue;
                }

                int nPropertyValue = boost::lexical_cast<int>( levelAttribute->value() );
                structProperty->insert(std::make_pair( levelAttribute->name(), nPropertyValue ));
            }

            mhtCoefficienData[nJob].AddElement( nLevel, structProperty );
        }
    }
}
