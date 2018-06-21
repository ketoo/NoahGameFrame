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
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();


	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SWITCH_FIGHT_HERO, this, &NFCHeroModule::OnSwitchFightHeroMsg);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCHeroModule::OnPlayerClassEvent);

	m_pSceneAOIModule->AddBeforeEnterSceneGroupCallBack(this, &NFCHeroModule::BeforeEnterSceneGroupEvent);
	m_pSceneAOIModule->AddAfterEnterSceneGroupCallBack(this, &NFCHeroModule::AfterEnterSceneGroupEvent);
	m_pSceneAOIModule->AddBeforeLeaveSceneGroupCallBack(this, &NFCHeroModule::BeforeLeaveSceneGroupEvent);
	m_pSceneAOIModule->AddAfterLeaveSceneGroupCallBack(this, &NFCHeroModule::AfterLeaveSceneGroupEvent);

	return true;
}

bool NFCHeroModule::BeforeShut()
{
	return true;
}

NFGUID NFCHeroModule::AddHero(const NFGUID& self, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return NFGUID();
	}

	int nRow = pHeroRecord->FindString(NFrame::Player::PlayerHero::ConfigID, strID);
	if (nRow >= 0)
	{
		//up star
		NFGUID id = pHeroRecord->GetObject(nRow, NFrame::Player::PlayerHero::GUID);
		int nNowStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Star);

		if (nNowStar < ECONSTDEFINE_HERO_MAX_STAR)
		{
			//random
			float fHit = (ECONSTDEFINE_HERO_MAX_STAR - nNowStar) / (float)ECONSTDEFINE_HERO_MAX_STAR;
			float fRandom = m_pKernelModule->Random();

			if (fRandom < fHit)
			{
				//hit

				nNowStar++;
				pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::Star, nNowStar);


			}
			else
			{
				//badly
			}

		}

		return id;
	}
	
	//if you have't assign a model for main role, than you assign a hero model for main role
	const std::string& strCnfID = m_pKernelModule->GetPropertyString(self, NFrame::Player::ConfigID());
	if (strCnfID.empty())
	{
		m_pKernelModule->SetPropertyString(self, NFrame::Player::ConfigID(), strID);
	}


	NF_SHARE_PTR<NFDataList> xRowData = pHeroRecord->GetInitData();

	NFGUID xHeroID = m_pKernelModule->CreateGUID();
	xRowData->SetObject(NFrame::Player::PlayerHero::GUID, xHeroID);
	xRowData->SetString(NFrame::Player::PlayerHero::ConfigID, strID);
	xRowData->SetInt(NFrame::Player::PlayerHero::Activated, 1);
	xRowData->SetInt(NFrame::Player::PlayerHero::Level, 1);

	if (pHeroRecord->AddRow(-1, *xRowData) < 0)
	{
		return NFGUID();
	}

	
	return xHeroID;
}

bool NFCHeroModule::AddHeroExp(const NFGUID & self, const int64_t nExp)
{
	NFGUID xID;// = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	return AddHeroExp(self, xID, nExp);
}

bool NFCHeroModule::AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int64_t nExp)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
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

	int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroID);
	if (nRow < 0)
	{
		return false;
	}

	NFDataList varRowData;
	if (!pHeroRecord->QueryRow(nRow, varRowData))
	{
		return false;
	}

	const int64_t nCurExp = varRowData.Int(NFrame::Player::PlayerHero::Exp);
	const int nBeforeLevel = varRowData.Int32(NFrame::Player::PlayerHero::Level);

	int64_t nLeftExp = nCurExp + nExp;
	int nAfterLevel = nBeforeLevel;
	for (int i = nBeforeLevel; i < ECONSTDEFINE_HERO_MAXLEVEL; i++)
	{
		const int64_t nNeedExp = (i + 1) * ECONSTDEFINE_HERO_ONCELEVEEXP;

		if (nLeftExp >= nNeedExp)
		{
			nAfterLevel += 1;
			nLeftExp -= nNeedExp;

			pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::Level, nAfterLevel);
		}
		else
		{
			break;
		}
	}

	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::Exp, nLeftExp);

	return true;
}

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xHeroID, const EConsHero_Pos nPos)
{
	if (nPos <= EConsHero_Pos::ECONSt_HERO_UNKNOW
		|| nPos >= EConsHero_Pos::ECONSt_HERO_MAX)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	if (xHeroID.IsNull())
	{
		return false;
	}

	int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroID);
	if (nRow < 0)
	{
		return false;
	}

	int nActivite = pHeroRecord->GetInt32(nRow, NFrame::Player::PlayerHero::Activated);
	if (nActivite <= 0)
	{
		return false;
	}

	const std::string& strCnfID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const int nStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Star);

	NFGUID xHero1 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1());
	NFGUID xHero2 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2());
	NFGUID xHero3 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3());
	if (xHero1 == xHeroID
		|| xHero2 == xHeroID
		|| xHero3 == xHeroID)
	{
		//this hero has exist in the fighting list
		return 0;
	}
	/*
	NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	int nHeroMAXHP = pHeroValueRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXHP);

	switch (nPos)
	{
	case EConsHero_Pos::ECONSt_HERO_POS1:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID1(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos1CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos1Star(), nStar);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero1HP(), nHeroMAXHP);
		//now the fighting hero is hero 2, we would change the fighting hero as a new one because the pos2 has been placed
		if (xFightHero == xHero1
			|| xFightHero.IsNull())
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	case EConsHero_Pos::ECONSt_HERO_POS2:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID2(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos2CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos2Star(), nStar);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero1HP(), nHeroMAXHP);

		//now the fighting hero is hero 2, we would change the fighting hero as a new one because the pos2 has been placed
		if (xFightHero == xHero2
			|| xFightHero.IsNull())
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	case EConsHero_Pos::ECONSt_HERO_POS3:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID3(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos3CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos3Star(), nStar);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero1HP(), nHeroMAXHP);
		//now the fighting hero is hero 2, we would change the fighting hero as a new one because the pos2 has been placed
		if (xFightHero == xHero3
			|| xFightHero.IsNull())
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	default:
		break;
	}
	*/

	return false;
}

bool NFCHeroModule::SwitchFightHero(const NFGUID & self, const NFGUID & xHeroID)
{
	/*
	NFGUID xLastFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	if (xLastFightingHero == xHeroID)
	{
		return false;
	}

	EConsHero_Pos nPos = GetFightPos(self, xHeroID);
	if (nPos <= EConsHero_Pos::ECONSt_HERO_UNKNOW
		|| nPos >= EConsHero_Pos::ECONSt_HERO_MAX)
	{
		return false;
	}


	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroID);
	if (nRow < 0)
	{
		return false;
	}

	int nHero1HP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Hero1HP());
	int nHero2HP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Hero2HP());
	int nHero3HP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::Hero3HP());
	int nHP = 0;
	if (nPos == EConsHero_Pos::ECONSt_HERO_POS1)
	{
		nHP = nHero1HP;
		if (nHero1HP <= 0)
		{
			return false;
		}
	}
	else if (nPos == EConsHero_Pos::ECONSt_HERO_POS2)
	{
		nHP = nHero2HP;
		if (nHero2HP <= 0)
		{
			return false;
		}
	}
	else if (nPos == EConsHero_Pos::ECONSt_HERO_POS3)
	{
		nHP = nHero3HP;
		if (nHero3HP <= 0)
		{
			return false;
		}
	}

	const std::string& strCnfID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const int nHeroLevel = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Level);
	
	m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightHero(), xHeroID);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::FightHeroCnfID(), strCnfID);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroLevel(), nHeroLevel);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHP);

	const std::string& strSkill1 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillNormal());
	const std::string& strSkill2 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillAttack());
	const std::string& strSkill3 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillTHUMP());

	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill1(), strSkill1);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill2(), strSkill2);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill3(), strSkill3);

	m_pHeroPropertyModule->CalFightintHeroProperty(self);
	*/
	return true;
}

NFGUID NFCHeroModule::GetHeroGUID(const NFGUID& self, const std::string& strID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return NFGUID();
	}

	int nRow = pHeroRecord->FindString(NFrame::Player::PlayerHero::ConfigID, strID);
	if (nRow < 0)
	{
		return NFGUID();
	}

	return pHeroRecord->GetObject(nRow, NFrame::Player::PlayerHero::GUID);
}

void NFCHeroModule::OnSetFightHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	const int nSet = xMsg.set();

	SetFightHero(nPlayerID, xHero, (NFIHeroModule::EConsHero_Pos)nSet);
}

void NFCHeroModule::OnSwitchFightHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSwitchFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());

	SwitchFightHero(nPlayerID, xHero);
	
}

NFIHeroModule::EConsHero_Pos NFCHeroModule::GetFightPos(const NFGUID & self, const NFGUID & xHeroID)
{
	/*
	if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1()) == xHeroID)
	{
		return EConsHero_Pos::ECONSt_HERO_POS1;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2()) == xHeroID)
	{
		return EConsHero_Pos::ECONSt_HERO_POS2;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3()) == xHeroID)
	{
		return EConsHero_Pos::ECONSt_HERO_POS3;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID4()) == xHeroID)
	{
		return EConsHero_Pos::ECONSt_HERO_POS4;
	}
	*/
	return EConsHero_Pos::ECONSt_HERO_UNKNOW;
}

int NFCHeroModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFCHeroModule::OnPlayerHPEvent);
	}
	
	return 0;
}

int NFCHeroModule::OnPlayerHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	/*
	NFGUID xFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	EConsHero_Pos nPos = GetFightPos(self, xFightingHero);

	switch (nPos)
	{
	case NFIHeroModule::ECONSt_HERO_UNKNOW:
		break;
	case NFIHeroModule::ECONSt_HERO_POS1:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero1HP(), newVar.GetInt());
	}
		break;
	case NFIHeroModule::ECONSt_HERO_POS2:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero2HP(), newVar.GetInt());
	}
		break;
	case NFIHeroModule::ECONSt_HERO_POS3:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero3HP(), newVar.GetInt());
	}
		break;
	case NFIHeroModule::ECONSt_HERO_MAX:
		break;
	default:
		break;
	}
	*/
	return 0;
}

int NFCHeroModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCHeroModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	/*
	//full hp for all heroes
	NFGUID xFightingHeroID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	NFGUID xHero1ID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1());
	NFGUID xHero2ID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2());
	NFGUID xHero3ID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3());

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	int nRow1 = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHero1ID);
	int nRow2 = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHero2ID);
	int nRow3 = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHero3ID);
	if (nRow1 < 0 || nRow2 < 0 || nRow3 < 0)
	{
		return 0;
	}

	NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());

	int nHero1MAXHP = pHeroValueRecord->GetInt(nRow1, NFrame::Player::HeroValue::MAXHP);
	int nHero2MAXHP = pHeroValueRecord->GetInt(nRow2, NFrame::Player::HeroValue::MAXHP);
	int nHero3MAXHP = pHeroValueRecord->GetInt(nRow3, NFrame::Player::HeroValue::MAXHP);

	int nHero1MAXMP = pHeroValueRecord->GetInt(nRow1, NFrame::Player::HeroValue::MAXMP);
	int nHero2MAXMP = pHeroValueRecord->GetInt(nRow2, NFrame::Player::HeroValue::MAXMP);
	int nHero3MAXMP = pHeroValueRecord->GetInt(nRow3, NFrame::Player::HeroValue::MAXMP);
	
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero1HP(), nHero1MAXHP);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero2HP(), nHero2MAXHP);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::Hero3HP(), nHero3MAXHP);
	
	if (xFightingHeroID == xHero1ID)
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXHP(), nHero1MAXHP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHero1MAXHP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXMP(), nHero1MAXMP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nHero1MAXMP);
	}
	else if (xFightingHeroID == xHero2ID)
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXHP(), nHero2MAXHP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHero2MAXHP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXMP(), nHero2MAXMP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nHero2MAXMP);
	}
	else if (xFightingHeroID == xHero3ID)
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXHP(), nHero3MAXHP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHero3MAXHP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXMP(), nHero3MAXMP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nHero3MAXMP);
	}

	*/
	return 0;
}

int NFCHeroModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFCHeroModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}


