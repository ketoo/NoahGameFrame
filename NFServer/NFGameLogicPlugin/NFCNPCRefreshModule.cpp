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
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>("NFCHeroPropertyModule");
	
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pPackModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLevelModule);
	assert(NULL != m_pHeroPropertyModule);

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCNPCRefreshModule::OnObjectClassEvent);

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
			const int nNPCType = m_pElementInfoModule->GetPropertyInt(strConfigIndex, NFrame::NPC::NPCType());
			NF_SHARE_PTR<NFIPropertyManager> pSelfPropertyManager = pSelf->GetPropertyManager();

			if (nNPCType == 1)
			{
				//hero
				NFGUID xMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::Master();
				NFCDataList xHeroPropertyList;
				if (m_pHeroPropertyModule->CalHeroAllProperty(xMasterID, self, xHeroPropertyList))
				{
					NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(xMasterID, NFrame::Player::R_HeroPropertyValue());
					if (pHeroPropertyRecord)
					{
						for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
						{
							const std::string& strColTag = pHeroPropertyRecord->GetColTag(i);
							const int nValue = xHeroPropertyList.Int(i);
							pSelfPropertyManager->SetPropertyInt(strColTag, nValue);
						}
					}
				}
			}
			else
			{
				//normal npc
				NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strPropertyID);
				if (pConfigPropertyManager)
				{
					std::string strProperName;
					for (NFIProperty* pProperty = pConfigPropertyManager->FirstNude(strProperName); pProperty != NULL; pProperty = pConfigPropertyManager->NextNude(strProperName))
					{
						if (pSelfPropertyManager && strProperName != NFrame::NPC::ID())
						{
							pSelfPropertyManager->SetProperty(pProperty->GetKey(), pProperty->GetValue());
						}
					}
				}
			}
            
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
            const std::string& strConfigID = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
            int nHPMax = m_pElementInfoModule->GetPropertyInt(strConfigID, NFrame::NPC::MAXHP());

            m_pKernelModule->SetPropertyInt(self, NFrame::NPC::HP(), nHPMax);
            m_pKernelModule->AddPropertyCallBack( self, NFrame::NPC::HP(), this, &NFCNPCRefreshModule::OnObjectHPEvent );

			m_pKernelModule->AddEventCallBack( self, NFED_ON_OBJECT_BE_KILLED, this, &NFCNPCRefreshModule::OnObjectBeKilled );
        }
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