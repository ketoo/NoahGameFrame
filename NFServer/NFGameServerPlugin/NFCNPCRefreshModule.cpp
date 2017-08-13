// -------------------------------------------------------------------------
//    @FileName         :   NFCNPCRefreshModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-10-17
//    @Module               :    NFCNPCRefreshModule

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
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
	
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCNPCRefreshModule::OnObjectClassEvent);

    return true;
}

int NFCNPCRefreshModule::OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
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
			const std::string& strEffectPropertyID = m_pElementModule->GetPropertyString(strConfigIndex, NFrame::NPC::EffectData());
			const int nNPCType = m_pElementModule->GetPropertyInt32(strConfigIndex, NFrame::NPC::NPCType());
			NF_SHARE_PTR<NFIPropertyManager> pSelfPropertyManager = pSelf->GetPropertyManager();

			if (nNPCType == NFMsg::ENPCType::ENPCTYPE_NORMAL
				|| nNPCType == NFMsg::ENPCType::ENPCTYPE_TURRET)
			{
				//normal npc
				NF_SHARE_PTR<NFIPropertyManager> pConfigPropertyManager = m_pElementModule->GetPropertyManager(strEffectPropertyID);
				if (pConfigPropertyManager)
				{
					std::string strProperName;
					for (NFIProperty* pProperty = pConfigPropertyManager->FirstNude(strProperName); pProperty != NULL; pProperty = pConfigPropertyManager->NextNude(strProperName))
					{
						if (pSelfPropertyManager && pProperty->Changed()
							&& strProperName != NFrame::IObject::ID()
							&& strProperName != NFrame::IObject::ConfigID()
							&& strProperName != NFrame::IObject::ClassName()
							&& strProperName != NFrame::IObject::SceneID()
							&& strProperName != NFrame::IObject::GroupID())
						{
							pSelfPropertyManager->SetProperty(pProperty->GetKey(), pProperty->GetValue());
						}
					}
				}
			}
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent )
        {
			int nHPMax = m_pKernelModule->GetPropertyInt32(self, NFrame::NPC::MAXHP());
            m_pKernelModule->SetPropertyInt(self, NFrame::NPC::HP(), nHPMax);

            m_pKernelModule->AddPropertyCallBack( self, NFrame::NPC::HP(), this, &NFCNPCRefreshModule::OnObjectHPEvent );
			m_pEventModule->AddEventCallBack( self, NFED_ON_OBJECT_BE_KILLED, this, &NFCNPCRefreshModule::OnObjectBeKilled );
        }
    }

    return 0;
}

int NFCNPCRefreshModule::OnObjectHPEvent( const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    if ( newVar.GetInt() <= 0 )
    {
        NFGUID identAttacker = m_pKernelModule->GetPropertyObject( self, NFrame::NPC::LastAttacker());
        if (!identAttacker.IsNull())
		{
			m_pEventModule->DoEvent( self, NFED_ON_OBJECT_BE_KILLED, NFDataList() << identAttacker );

			m_pScheduleModule->AddSchedule( self, "OnDeadDestroyHeart", this, &NFCNPCRefreshModule::OnDeadDestroyHeart, 5.0f, 1 );
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
    int nSceneID = m_pKernelModule->GetPropertyInt32( self, NFrame::NPC::SceneID());
    int nGroupID = m_pKernelModule->GetPropertyInt32( self, NFrame::NPC::GroupID());

	NFVector3 fSeedPos = m_pKernelModule->GetPropertyVector3( self, NFrame::NPC::Position());

    m_pKernelModule->DestroySelf( self );

    NFDataList arg;
	arg << NFrame::NPC::Position() << fSeedPos;
	arg << NFrame::NPC::SeedID() << strSeedID;

	m_pKernelModule->CreateObject( NFGUID(), nSceneID, nGroupID, strClassName, strConfigID, arg );

    return 0;
}

int NFCNPCRefreshModule::OnObjectBeKilled( const NFGUID& self, const NFEventDefine nEventID, const NFDataList& var )
{
	if ( var.GetCount() == 1 && var.Type( 0 ) == TDATA_OBJECT )
	{
		NFGUID identKiller = var.Object( 0 );
		if ( m_pKernelModule->GetObject( identKiller ) )
		{
			const int64_t nExp = m_pKernelModule->GetPropertyInt32( self, NFrame::Player::EXP() );

			m_pLevelModule->AddExp( identKiller, nExp);

			
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, identKiller, "Add Exp for kill monster", nExp);
		}
		else
		{
			m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, identKiller, "There is no object", __FUNCTION__, __LINE__);
		}
	}

	return 0;
}