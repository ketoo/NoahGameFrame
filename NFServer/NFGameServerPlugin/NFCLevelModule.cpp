// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#include "NFCLevelModule.h"
#include "NFComm/NFCore/NFCCommonConfig.h"

bool NFCLevelModule::Init()
{
    return true;
}


bool NFCLevelModule::Shut()
{
    return true;
}

bool NFCLevelModule::Execute()
{
    //位置呢
    return true;
}

bool NFCLevelModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pLogModule = pPluginManager->FindModule<NFILogModule>( "NFCLogModule" );
	m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>( "NFCPropertyConfigModule" );
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>( "NFCPackModule" );
	
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );
	assert( NULL != m_pPropertyConfigModule );
	assert( NULL != m_pElementInfoModule );
	assert( NULL != m_pPackModule );

    m_pKernelModule->AddClassCallBack( NFrame::Player::ThisName(), this, &NFCLevelModule::OnObjectClassEvent );
    m_pKernelModule->AddClassCallBack( NFrame::NPC::ThisName(), this, &NFCLevelModule::OnObjectClassEvent );

    std::string strPlayerPath = pPluginManager->GetConfigPath();
    strPlayerPath += "NFDataCfg/Ini/Common/PlayerLevelConfig.xml";
    NFCCommonConfig::GetSingletonPtr()->LoadConfig(strPlayerPath);

    return true;
}

int NFCLevelModule::AddExp( const NFGUID& self, const int nExp)
{
    NFJobType eJobType = ( NFJobType )m_pKernelModule->GetPropertyInt( self, NFrame::Player::Job());
    int nCurExp = m_pKernelModule->GetPropertyInt( self, NFrame::Player::EXP() );
    int nLevel = m_pKernelModule->GetPropertyInt( self, NFrame::Player::Level() );
    int nMaxExp = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());

    nCurExp += nExp;

    int nRemainExp = nCurExp - nMaxExp;
    while ( nRemainExp >= 0 )
    {
        //升级
        nLevel++;
        //防止越级BUG
        m_pKernelModule->SetPropertyInt( self, NFrame::Player::Level(), nLevel );

        nCurExp = nRemainExp;

        nMaxExp = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, NFrame::Player::MAXEXP());
        if (nMaxExp <= 0)
        {
            break;
        }

        nRemainExp -= nMaxExp;
    }

    m_pKernelModule->SetPropertyInt( self, NFrame::Player::EXP(), nCurExp );
    
    return 0;
}

int NFCLevelModule::OnKillObject( const NFGUID& self, const NFGUID& other )
{
    return 0;
}

int NFCLevelModule::OnDead( const NFGUID& self, const NFGUID& other )
{
    //降级掉经验？
    return 0;
}

int NFCLevelModule::OnObjectExpEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar )
{

    return 0;
}

int NFCLevelModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    if ( strClassName == NFrame::Player::ThisName() && CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {

    }

    if ( strClassName == NFrame::Player::ThisName() && CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent )
    {
        m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCLevelModule::OnObjectLevelEvent);
    }
    if ( strClassName == NFrame::NPC::ThisName() && CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        m_pKernelModule->AddEventCallBack( self, NFED_ON_OBJECT_BE_KILLED, this, &NFCLevelModule::OnObjectBeKilled );
    }

    return 0;
}

int NFCLevelModule::OnObjectLevelEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
    NFINT64 nLevel = newVar.GetInt();

    AddLevelUpAward(self, nLevel);
    return 0;
}

int NFCLevelModule::OnObjectBeKilled( const NFGUID& object, const int nEventID, const NFIDataList& var )
{
    if ( var.GetCount() == 1 && var.Type( 0 ) == TDATA_OBJECT )
    {
        NFGUID identKiller = var.Object( 0 );
        if ( m_pKernelModule->GetObject( identKiller ) )
        {
            int nExp = m_pKernelModule->GetPropertyInt( object, NFrame::Player::EXP() );
            AddExp( identKiller, nExp);
            // TODO:加怪物掉落金钱
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, identKiller, "Add Exp for kill monster", nExp);
        }
        else
        {
            m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, identKiller, "There is no object", __FUNCTION__, __LINE__);
        }
    }

    return 0;
}


bool NFCLevelModule::AddLevelUpAward( const NFGUID& self, const int nLevel )
{
    const std::string& strAwardID = NFCCommonConfig::GetSingletonPtr()->GetAttributeString("PlayerLevel", boost::lexical_cast<std::string>(nLevel), "AwardPack");
    if (strAwardID.empty())
    {
        return true;
    }

    std::vector<std::string> xList;
    NFCCommonConfig::GetSingletonPtr()->GetStructItemList(strAwardID, xList);

    for (int i = 0; i < xList.size(); ++i)
    {
        const std::string& strItemID = xList[i];
        const int nCout = NFCCommonConfig::GetSingletonPtr()->GetAttributeInt("strAwardID", strItemID, "Count");
        if (m_pElementInfoModule->ExistElement(strItemID))
        {
            const int nItemType = m_pElementInfoModule->GetPropertyInt(strItemID, NFrame::Item::ItemType());
            switch (nItemType)
            {
            case NFMsg::EIT_EQUIP:
                {
                    m_pPackModule->CreateEquip(self, strItemID);
                }
                break;
            default:
                m_pPackModule->CreateItem(self, strItemID, nCout);
                break;
            }
        }
    }

    return true;
}