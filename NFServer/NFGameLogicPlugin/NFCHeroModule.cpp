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

	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pGameServerNet_ServerModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pElementInfoModule);

	if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGameMsgID::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg)) { return false; }

	bool isUnitTest = true;//单元测试开关

	if (isUnitTest)
	{
		m_pKernelModule->CreateScene(1);
		NFGUID xPlayerID = m_pUUIDModule->CreateGUID();
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(xPlayerID, 1, 0, NFrame::Player::ThisName(), "", NFCDataList());
		AddHero(xPlayerID, "Hero1");
		AddHero(xPlayerID, "Hero1");
		AddHero(xPlayerID, "Hero1");
		AddHero(xPlayerID, "Hero1");
		AddHero(xPlayerID, "Hero1");
		AddHero(xPlayerID, "Hero1");

		NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(NFrame::Player::R_PlayerHero());
		if (pRecord.get())
		{
			NFCDataList xRowData1;
			pRecord->QueryRow(0, xRowData1);
			const NFGUID xHeroID1 = xRowData1.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData2;
			pRecord->QueryRow(1, xRowData2);
			const NFGUID xHeroID2 = xRowData2.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData3;
			pRecord->QueryRow(2, xRowData3);
			const NFGUID xHeroID3 = xRowData3.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData4;
			pRecord->QueryRow(3, xRowData4);
			const NFGUID xHeroID4 = xRowData4.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData5;
			pRecord->QueryRow(4, xRowData5);
			const NFGUID xHeroID5 = xRowData5.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData6;
			pRecord->QueryRow(5, xRowData6);
			const NFGUID xHeroID6 = xRowData6.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData7;
			pRecord->QueryRow(6, xRowData7);
			const NFGUID xHeroID7 = xRowData7.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			NFCDataList xRowData8;
			pRecord->QueryRow(7, xRowData8);
			const NFGUID xHeroID8 = xRowData8.Object(NFrame::Player::PlayerHero::PlayerHero_GUID);

			AddHeroExp(xPlayerID, xHeroID1, 1000);//添加经验

			HeroStarUp(xPlayerID, xHeroID1);//升星

			HeroSkillUp(xPlayerID, xHeroID1, 0);
			HeroSkillUp(xPlayerID, xHeroID1, 0);
			HeroSkillUp(xPlayerID, xHeroID1, 1);
			HeroSkillUp(xPlayerID, xHeroID1, 2);
			HeroSkillUp(xPlayerID, xHeroID1, 3);
			HeroSkillUp(xPlayerID, xHeroID1, 4);

			HeroTalentUp(xPlayerID, xHeroID1, 0);
			HeroTalentUp(xPlayerID, xHeroID1, 0);
			HeroTalentUp(xPlayerID, xHeroID1, 1);
			HeroTalentUp(xPlayerID, xHeroID1, 2);
			HeroTalentUp(xPlayerID, xHeroID1, 3);
			HeroTalentUp(xPlayerID, xHeroID1, 4);
			HeroTalentUp(xPlayerID, xHeroID1, 5);
			HeroTalentUp(xPlayerID, xHeroID1, 6);
			HeroTalentUp(xPlayerID, xHeroID1, 7);
			HeroTalentUp(xPlayerID, xHeroID1, 8);


			bool isSetFightHero = false;
			isSetFightHero = SetFightHero(xPlayerID, 0, xHeroID1);
			isSetFightHero = SetFightHero(xPlayerID, 1, xHeroID2);
			isSetFightHero = SetFightHero(xPlayerID, 2, xHeroID3);
			isSetFightHero = SetFightHero(xPlayerID, 3, xHeroID4);
			isSetFightHero = SetFightHero(xPlayerID, 4, xHeroID5);
			isSetFightHero = SetFightHero(xPlayerID, 5, xHeroID6);
			isSetFightHero = UnSetFightHero(xPlayerID, 4, xHeroID5);
			isSetFightHero = SetFightHero(xPlayerID, 5, xHeroID6);
			isSetFightHero = SetFightHero(xPlayerID, 6, xHeroID7);
			isSetFightHero = SetFightHero(xPlayerID, 7, xHeroID8);
		}
	}

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
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	NFCDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout <= 0)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFCDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	if (nIndex > (NFrame::Player::PlayerHero::PlayerHero_Skill5 - NFrame::Player::PlayerHero::PlayerHero_Skill1)
		|| nIndex < 0)
	{
		return false;
	}

	int nSkillIDCol = NFrame::Player::PlayerHero::PlayerHero_Skill1 + nIndex;
	int nSkillLevleCol = NFrame::Player::PlayerHero::PlayerHero_SkillLevel1 + nIndex;

	const std::string& strHeroID = varRowData.String(NFrame::Player::PlayerHero::PlayerHero_ConfigID);
	const std::string& strSkillID = varRowData.String(nSkillIDCol);
	const int nLevel = varRowData.Int(nSkillLevleCol);

	std::string strNewSkillID = strSkillID;

	int nAfterLevel = nLevel + 1;
	if (strNewSkillID.empty())
	{
		const std::string& strSkillRef = m_pElementInfoModule->GetPropertyString(strHeroID, NFrame::Hero::SkillRef());

		switch (nSkillIDCol)
		{
		case NFrame::Player::PlayerHero::PlayerHero_Skill1:
			strNewSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL1());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Skill2:
			strNewSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL2());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Skill3:
			strNewSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL3());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Skill4:
			strNewSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL4());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Skill5:
			strNewSkillID = m_pElementInfoModule->GetPropertyString(strSkillRef, NFrame::SkillRef::SKILL5());
			break;
		default:
			return 1;
			break;
		}

		pHeroRecord->SetString(nRow, nSkillIDCol, strNewSkillID.c_str());
		nAfterLevel = 1;
	}

	if (strNewSkillID.empty())
	{
		return false;
	}

	pHeroRecord->SetInt(nRow, nSkillLevleCol, nAfterLevel);
	return true;
}

bool NFCHeroModule::HeroTalentUp(const NFGUID& self, const NFGUID& xHeroID, const int nIndex)
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

	NFCDataList xMatchList;
	int nMatchCout = pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroID, xMatchList);
	if (nMatchCout <= 0)
	{
		return false;
	}

	const int nRow = xMatchList.Int(0);
	NFCDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	if (nIndex > (NFrame::Player::PlayerHero::PlayerHero_Talent8 - NFrame::Player::PlayerHero::PlayerHero_Talent1)
		|| nIndex < 0)
	{
		return false;
	}

	int nTalentIDCol = NFrame::Player::PlayerHero::PlayerHero_Talent1 + nIndex;
	int nTalentLevelCol = NFrame::Player::PlayerHero::PlayerHero_TalentLevel1 + nIndex;

	const std::string& strHeroID = varRowData.String(NFrame::Player::PlayerHero::PlayerHero_ConfigID);
	const std::string& strTalentID = varRowData.String(nTalentIDCol);
	const int nLevel = varRowData.Int(nTalentLevelCol);

	std::string strNewTalentID = strTalentID;

	int nAfterLevel = nLevel + 1;
	if (strNewTalentID.empty())
	{
		const std::string& strTalentRef = m_pElementInfoModule->GetPropertyString(strHeroID, NFrame::Hero::TalentRef());

		switch (nTalentIDCol)
		{
		case NFrame::Player::PlayerHero::PlayerHero_Talent1:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT1());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent2:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT2());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent3:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT3());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent4:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT4());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent5:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT5());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent6:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT6());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent7:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT7());
			break;
		case NFrame::Player::PlayerHero::PlayerHero_Talent8:
			strNewTalentID = m_pElementInfoModule->GetPropertyString(strTalentRef, NFrame::TalentRef::TALENT8());
			break;
		default:
			return 1;
			break;
		}

		pHeroRecord->SetString(nRow, nTalentIDCol, strNewTalentID.c_str());
		nAfterLevel = 1;
	}

	if (strNewTalentID.empty())
	{
		return false;
	}

	pHeroRecord->SetInt(nRow, nTalentLevelCol, nAfterLevel);
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
	if (nHeroCount == -1 || nHeroCount > 1)
	{
		return false;
	}

	int nCount = pHeroRecord->FindInt(NFrame::Player::PlayerHero_FightState, 1, NFCDataList());
	if (nCount >= ECONSTDEFINE_HERO_MAX_FIGHT_COUNT)
	{
		return false;
	}

	/////////////Set FightState to 1
	pHeroRecord->SetInt(varHeroID.Int(0), NFrame::Player::PlayerHero_FightState, 1);

	/////////////Add this hero to Record_PlayerFightHero
	NF_SHARE_PTR<NFIRecord> pFightHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerFightHero());
	if (nullptr == pFightHeroRecord.get())
	{
		return false;
	}

	NF_SHARE_PTR<NFIDataList> xRowData = pFightHeroRecord->GetInitData();

	xRowData->SetObject(NFrame::Player::PlayerFightHero::PlayerFightHero_GUID, xHeroID);
	xRowData->SetInt(NFrame::Player::PlayerFightHero::PlayerFightHero_FightPos, nPos);

	if (pFightHeroRecord->AddRow(-1, *xRowData) < 0)
	{
		return false;
	}

	return true;
}

bool NFCHeroModule::UnSetFightHero(const NFGUID& self, const int nPos, const NFGUID& xHeroID)
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
	if (nHeroCount == -1 || nHeroCount > 1)
	{
		return false;
	}

	int nCount = pHeroRecord->FindInt(NFrame::Player::PlayerHero_FightState, 1, NFCDataList());
	if (nCount <= 0)
	{
		return false;
	}

	/////////////Set FightState to 0
	pHeroRecord->SetInt(varHeroID.Int(0), NFrame::Player::PlayerHero_FightState, 0);

	/////////////Remove this hero to Record_PlayerFightHero
	NF_SHARE_PTR<NFIRecord> pFightHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerFightHero());
	if (nullptr == pFightHeroRecord.get())
	{
		return false;
	}

	NFCDataList varFightHeroID;
	pFightHeroRecord->FindObject(NFrame::Player::PlayerFightHero::PlayerFightHero_GUID, xHeroID, varHeroID);
	if (varHeroID.GetCount() >= 1)
	{
		pFightHeroRecord->Remove(varHeroID);
	}

	return true;
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
	int nFightState = pHeroRecord->FindObject(NFrame::Player::PlayerHero_FightState, xHeroID, xDataList);
	if (nFightState <= 0)
	{
		return false;
	}

	const int nRow = xDataList.Int(0);
	pHeroRecord->SetString(nRow, NFrame::Player::PlayerHero_FightSkill, xSkillID.c_str());

	return false;
}

bool NFCHeroModule::HeroUnSkill(const NFGUID& self, const NFGUID& xHeroID, const std::string& xSkillID)
{
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