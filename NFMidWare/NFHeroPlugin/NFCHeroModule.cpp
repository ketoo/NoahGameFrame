// -------------------------------------------------------------------------
//    @FileName			:    NFCHeroModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-16
//    @Module           :    NFCHeroModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCHeroModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCHeroModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>();

	return true;
}

bool NFCHeroModule::Shut()
{
	return true;
}

bool NFCHeroModule::Execute()
{
	return true;
}

bool NFCHeroModule::AfterInit()
{

	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCHeroModule::OnObjectClassEvent);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg);

	return true;
}

bool NFCHeroModule::BeforeShut()
{
	return true;
}

NFGUID NFCHeroModule::AddHero(const NFGUID& self, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return NFGUID();
	}

	NF_SHARE_PTR<NFDataList> xRowData = pHeroRecord->GetInitData();

	NFGUID xHeroID = m_pKernelModule->CreateGUID();
	xRowData->SetObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xHeroID);
	xRowData->SetString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strID.c_str());


	if (pHeroRecord->AddRow(-1, *xRowData) < 0)
	{
		return xHeroID;
	}

	return NFGUID();
}

bool NFCHeroModule::AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int nExp)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	if (nExp <= 0)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <= 0)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	NFDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	const int nCurExp = varRowData.Int(NFrame::Player::PlayerHero::PlayerHero_Exp);
	const int nBeforeLevel = varRowData.Int(NFrame::Player::PlayerHero::PlayerHero_Level);

	int nLeftExp = nCurExp + nExp;
	int nAfterLevel = nBeforeLevel;
	for (int i = nBeforeLevel; i < ECONSTDEFINE_HERO_MAXLEVEL; i++)
	{
		const int nNeedExp = (i + 1) * ECONSTDEFINE_HERO_ONCELEVEEXP;

		if (nLeftExp >= nNeedExp)
		{
			nAfterLevel += 1;
			nLeftExp -= nNeedExp;
		}
		else
		{
			break;
		}
	}

	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Exp, nLeftExp);
	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Level, nAfterLevel);

	return true;
}

bool NFCHeroModule::HeroStarUp(const NFGUID& self, const NFGUID& xHeroID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <= 0)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	const int nBeforeStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Star);

	int nAfterStar = nBeforeStar + 1;
	if (nAfterStar > ECONSTDEFINE_HERO_MAXSTAR)
	{
		nAfterStar = ECONSTDEFINE_HERO_MAXSTAR;
	}

	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::PlayerHero_Star, nAfterStar);

	return true;
}

bool NFCHeroModule::HeroSkillUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex)
{
	if (xHeroID.IsNull())
	{
		return false;
	}

	if (nIndex > NFrame::Player::PlayerHero::PlayerHero_Skill5 && nIndex < NFrame::Player::PlayerHero::PlayerHero_Skill1)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout != 1)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}
	
	const std::string& strSkillID = pHeroRecord->GetString(nRow, nIndex);
	const std::string& strNextSkillID = m_pElementModule->GetPropertyString(strSkillID, NFrame::Skill::AfterUpID());
	if (strNextSkillID.empty())
	{
		//no next skill id, this skill is the best skill
		return false;
	}

	pHeroRecord->SetString(nRow, nIndex, strNextSkillID);

	return true;
}

bool NFCHeroModule::HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex)
{
	if (xHeroID.IsNull())
	{
		return false;
	}

	if (nIndex > NFrame::Player::PlayerHero::PlayerHero_Talent5 && nIndex < NFrame::Player::PlayerHero::PlayerHero_Talent1)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout != 1)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	const std::string& strTalentID = pHeroRecord->GetString(nRow, nIndex);
	const std::string& strNextTalentID = m_pElementModule->GetPropertyString(strTalentID, NFrame::Talent::AfterUpID());
	if (strNextTalentID.empty())
	{
		//no next talent id, this skill is the best talent
		return false;
	}

	pHeroRecord->SetString(nRow, nIndex, strNextTalentID);

	return true;
}

bool NFCHeroModule::SetFightHero(const NFGUID& self, const int nPos, const NFGUID& xHeroID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	NFDataList varHeroID;
	int nHeroCount = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varHeroID);
	if (nHeroCount != 1)
	{
		return false;
	}

	/////////////Add this hero to Record_PlayerFightHero
	NF_SHARE_PTR<NFIRecord> pFightHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerFightHero());
	if (nullptr == pFightHeroRecord)
	{
		return false;
	}

	if (pFightHeroRecord->GetRows() <= nPos || nPos < 0)
	{
		return false;
	}

	NF_SHARE_PTR<NFDataList> xRowData = pFightHeroRecord->GetInitData();

	xRowData->SetObject(NFrame::Player::PlayerFightHero::PlayerFightHero_GUID, xHeroID);
	if (pFightHeroRecord->AddRow(nPos, *xRowData) < 0)
	{
		return false;
	}

	return true;
}

bool NFCHeroModule::CreateHero(const NFGUID& self, const NFGUID& xHeroID)
{
	if (xHeroID.IsNull())
	{
		return false;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
	if (!m_pSceneProcessModule->GetCloneSceneType(nSceneID) == E_SCENE_TYPE::SCENE_TYPE_ERROR)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	NFDataList xDataList;
	int nHeroCount = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xDataList);
	if (nHeroCount != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero_ConfigID);

    NFDataList varList;

    int nCamp = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Camp());
    varList << NFrame::NPC::Camp() << nCamp;
    varList << NFrame::NPC::MasterID() << self;

	NF_SHARE_PTR<NFIObject> pHero = m_pKernelModule->CreateObject(xHeroID, nSceneID, nGroupID, NFrame::NPC::ThisName(), strConfigID, NFDataList());
    if (!pHero)
    {
        return false;
    }
	return true;
}

bool NFCHeroModule::DestroyHero(const NFGUID& self, const NFGUID& xHeroID)
{
	if (xHeroID.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	NFDataList xDataList;
	int nHeroCount = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xDataList);
	if (nHeroCount != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	if (xHeroID == pHeroRecord->GetObject(nRow, NFrame::Player::PlayerHero_GUID))
	{
		m_pKernelModule->DestroyObject(xHeroID);
		return true;
	}


	return false;
}

NFGUID NFCHeroModule::GetHeroGUID(const NFGUID& self, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return NFGUID();
	}

	NFDataList xDataList;
	int nHeroCount = pHeroRecord->FindString(NFrame::Player::PlayerHero_ConfigID, strID.c_str(), xDataList);
	if (nHeroCount != 1)
	{
		return NFGUID();
	}

	const int nRow = xDataList.Int(0);
	return pHeroRecord->GetObject(nRow, NFrame::Player::PlayerHero_GUID);
}

bool NFCHeroModule::HeroWearSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xSkillID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	NFDataList xDataList;
	int nHeroCount = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xDataList);
	if (nHeroCount != 1)
	{
		return false;
	}

	bool bOwner = false;
	const int nRow = xDataList.Int(0);
	for (int i = NFrame::Player::PlayerHero_Skill1; i < NFrame::Player::PlayerHero_Skill5; ++i)
	{
		const std::string& strOwnerSkillID = pHeroRecord->GetString(nRow, i);
		if (strOwnerSkillID == xSkillID)
		{
			bOwner = true;
			break;
		}
	}

	if (bOwner)
	{
		pHeroRecord->SetString(nRow, NFrame::Player::PlayerHero_FightSkill, xSkillID);
		return true;
	}

	return false;
}

void NFCHeroModule::OnSetFightHeroMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	const int nFightPos = xMsg.fightpos();

	NF_SHARE_PTR<NFIRecord> pHeroList = m_pKernelModule->FindRecord(nPlayerID, NFrame::Player::R_PlayerHero());
	if (!pHeroList.get())
	{
		return;
	}

	SetFightHero(nPlayerID, nFightPos, xHero);	
}

int NFCHeroModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{

	NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(self);
	if (nullptr == pSelf)
	{
		return 1;
	}

	if (strClassName == NFrame::NPC::ThisName())
	{
		if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
		{
			const std::string& strConfigIndex = m_pKernelModule->GetPropertyString(self, NFrame::NPC::ConfigID());
			const std::string& strEffectPropertyID = m_pElementModule->GetPropertyString(strConfigIndex, NFrame::NPC::EffectData());
			const int nNPCType = m_pElementModule->GetPropertyInt(strConfigIndex, NFrame::NPC::NPCType());
			NF_SHARE_PTR<NFIPropertyManager> pSelfPropertyManager = pSelf->GetPropertyManager();

			if (nNPCType == NFMsg::ENPCType::ENPCTYPE_HERO)
			{
				//hero
				NFGUID xMasterID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::MasterID());
				NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(xMasterID, NFrame::Player::R_HeroPropertyValue());
				if (pHeroPropertyRecord)
				{
					NFDataList xHeroPropertyList;
					if (m_pHeroPropertyModule->CalHeroAllProperty(xMasterID, self, xHeroPropertyList))
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
		}
	}
}