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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>( "NFCKernelModule" );
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>( "NFCSceneProcessModule" );
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>( "NFCElementInfoModule" );
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>("NFCPackModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>("NFCLevelModule");
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>("NFCCommonConfigModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pPackModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLevelModule);
	assert(NULL != m_pCommonConfigModule);

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCNPCRefreshModule::OnObjectClassEvent);
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCNPCRefreshModule::OnObjectClassEvent);

	std::string strPlayerPath = pPluginManager->GetConfigPath();
	strPlayerPath += "NFDataCfg/Ini/Common/PlayerLevelConfig.xml";

	m_pCommonConfigModule->LoadConfig(strPlayerPath);

    return true;
}

int NFCNPCRefreshModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var )
{
    NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(self);
    if (nullptr == pSelf)
    {
        return 1;
    }

    if (strClassName == NFrame::NPC::ThisName())
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
        {
            const std::string& strConfigIndex = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
            const std::string& strPropertyID = m_pElementInfoModule->GetPropertyString(strConfigIndex, NFrame::NPC::EffectData());
            NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strPropertyID);
            if (pConfigPropertyManager)
            {
                std::string strProperName;
                NF_SHARE_PTR<NFIPropertyManager> pSelfPropertyManager = pSelf->GetPropertyManager();
                for (NFIProperty* pProperty = pConfigPropertyManager->FirstNude(strProperName); pProperty != NULL; pProperty = pConfigPropertyManager->NextNude(strProperName))
                {
                    if (pSelfPropertyManager && strProperName != NFrame::NPC::ID())
                    {
                        pSelfPropertyManager->SetProperty(pProperty->GetKey(), pProperty->GetValue());
                    }
                }
            }
        }

        if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
            int nHPMax = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::NPC::MAXHP());
            m_pKernelModule->SetPropertyInt(self, NFrame::NPC::HP(), nHPMax);
            m_pKernelModule->AddPropertyCallBack( self, NFrame::NPC::HP(), this, &NFCNPCRefreshModule::OnObjectHPEvent );

			m_pKernelModule->AddEventCallBack( self, NFED_ON_OBJECT_BE_KILLED, this, &NFCNPCRefreshModule::OnObjectBeKilled );
        }
    }

	if ( strClassName == NFrame::Player::ThisName() && CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT == eClassEvent )
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCNPCRefreshModule::OnObjectLevelEvent);
	}
    return 0;
}

int NFCNPCRefreshModule::OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    if ( newVar.GetInt() <= 0 )
    {
        NFGUID identAttacker = m_pKernelModule->GetPropertyObject( self, NFrame::NPC::LastAttacker());
        if (!identAttacker.IsNull())
		{
            m_pKernelModule->DoEvent( self, NFED_ON_OBJECT_BE_KILLED, NFCDataList() << identAttacker );

            m_pKernelModule->AddHeartBeat( self, "OnDeadDestroyHeart", this, &NFCNPCRefreshModule::OnDeadDestroyHeart, 5.0f, 1 );
        }
    }

    return 0;
}

int NFCNPCRefreshModule::OnDeadDestroyHeart( const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{
    //and create new object
    const std::string& strClassName = m_pKernelModule->GetPropertyString( self, NFrame::NPC::ClassName());
    const std::string& strSeedID = m_pKernelModule->GetPropertyString( self, NFrame::NPC::SeedID());
    const std::string& strConfigID = m_pKernelModule->GetPropertyString( self, NFrame::NPC::ConfigID());
    int nSceneID = m_pKernelModule->GetPropertyInt( self, NFrame::NPC::SceneID());
    int nGroupID = m_pKernelModule->GetPropertyInt( self, NFrame::NPC::GroupID());

    //m_pSceneProcessModule->ClearAll( nSceneID, nGroupID, strSeendID );

	float fSeedX = m_pKernelModule->GetPropertyFloat( self, NFrame::NPC::X());
	float fSeedY = m_pKernelModule->GetPropertyFloat( self, NFrame::NPC::Y());
	float fSeedZ = m_pKernelModule->GetPropertyFloat( self, NFrame::NPC::Z());

    m_pKernelModule->DestroyObject( self );

    NFCDataList arg;
	arg << NFrame::NPC::X() << fSeedX;
    arg << NFrame::NPC::Y() << fSeedY;
    arg << NFrame::NPC::Z() << fSeedZ;
	arg << NFrame::NPC::SeedID() << strSeedID;

	m_pKernelModule->CreateObject( NFGUID(), nSceneID, nGroupID, strClassName, strConfigID, arg );

    return 0;
}

int NFCNPCRefreshModule::OnObjectBeKilled( const NFGUID& self, const int nEventID, const NFIDataList& var )
{
	if ( var.GetCount() == 1 && var.Type( 0 ) == TDATA_OBJECT )
	{
		NFGUID identKiller = var.Object( 0 );
		if ( m_pKernelModule->GetObject( identKiller ) )
		{
			const int nExp = m_pKernelModule->GetPropertyInt( self, NFrame::Player::EXP() );

			m_pLevelModule->AddExp( identKiller, nExp);

			// TODO:¼Ó¹ÖÎïµôÂä½ðÇ®
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, identKiller, "Add Exp for kill monster", nExp);
		}
		else
		{
			m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, identKiller, "There is no object", __FUNCTION__, __LINE__);
		}
	}

	return 0;
}

int NFCNPCRefreshModule::OnObjectLevelEvent( const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar )
{
	NFINT64 nLevel = newVar.GetInt();

	AddLevelUpAward(self, nLevel);

	return 0;
}

bool NFCNPCRefreshModule::AddLevelUpAward( const NFGUID& self, const int nLevel )
{
	const std::string& strAwardID = m_pCommonConfigModule->GetAttributeString("PlayerLevel", lexical_cast<std::string>(nLevel), "AwardPack");
	if (strAwardID.empty())
	{
		return true;
	}

	std::vector<std::string> xList;
	m_pCommonConfigModule->GetStructItemList(strAwardID, xList);

	for (int i = 0; i < xList.size(); ++i)
	{
		const std::string& strItemID = xList[i];
		const int nCout = m_pCommonConfigModule->GetAttributeInt("strAwardID", strItemID, "Count");
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
