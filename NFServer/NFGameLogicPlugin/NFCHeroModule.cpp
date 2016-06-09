// -------------------------------------------------------------------------
//    @FileName      :    NFCHeroModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCHeroModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCHeroModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCHeroModule::Init()
{
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
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>("NFCSceneProcessModule");

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pGameServerNet_ServerModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pSceneProcessModule);

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGameMsgID::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg)) { return false; }

	return true;
}

bool NFCHeroModule::BeforeShut()
{
	return true;
}

bool NFCHeroModule::AddHero(const NFGUID& self, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	NF_SHARE_PTR<NFIDataList> xRowData = pHeroRecord->GetInitData();

	NFGUID xHeroID = m_pUUIDModule->CreateGUID();
	xRowData->SetObject(NFrame::Player::PlayerHero::PlayerHero_GUID, xHeroID);
	xRowData->SetString(NFrame::Player::PlayerHero::PlayerHero_ConfigID, strID.c_str());


	if (pHeroRecord->AddRow(-1, *xRowData) < 0)
	{
		return false;
	}

	return true;
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

	NFCDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, varFind) <= 0)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	NFCDataList varRowData;
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

	NFCDataList varFind;
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

	NFCDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout != 1)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFCDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}
	
	const std::string& strSkillID = pHeroRecord->GetString(nRow, nIndex);
	const std::string& strNextSkillID = m_pElementInfoModule->GetPropertyString(strSkillID, NFrame::Skill::AfterUpID());
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

	NFCDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout != 1)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFCDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	const std::string& strTalentID = pHeroRecord->GetString(nRow, nIndex);
	const std::string& strNextTalentID = m_pElementInfoModule->GetPropertyString(strTalentID, NFrame::Talent::AfterUpID());
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

	NFCDataList varHeroID;
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

	NF_SHARE_PTR<NFIDataList> xRowData = pFightHeroRecord->GetInitData();

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
	if (!m_pSceneProcessModule->IsCloneScene(nSceneID))
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	NFCDataList xDataList;
	int nHeroCount = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xDataList);
	if (nHeroCount != 1)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero_ConfigID);

	m_pKernelModule->CreateObject(xHeroID, nSceneID, nGroupID, NFrame::NPC::ThisName(), strConfigID, NFCDataList());

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

	NFCDataList xDataList;
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

	NFCDataList xDataList;
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