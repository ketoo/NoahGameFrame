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
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SET_FIGHT_HERO, this, &NFCHeroModule::OnSetFightHeroMsg);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SWITCH_FIGHT_HERO, this, &NFCHeroModule::OnSwitchFightHeroMsg);

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

	AddToFightList(self, xHeroID, strID);

	return xHeroID;
}

bool NFCHeroModule::AddHeroExp(const NFGUID & self, const int64_t nExp)
{
	NFGUID xID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
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

bool NFCHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xHeroID, const int nPos)
{
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

	//AddToFightList(self, xHeroID, nPos);
	NFGUID xHero1 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1());
	NFGUID xHero2 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2());
	NFGUID xHero3 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3());
	NFGUID xFightHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	if (xHero1 == xHeroID
		|| xHero2 == xHeroID
		|| xHero3 == xHeroID)
	{
		return 0;
	}

	switch (nPos)
	{
	case 1:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID1(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos1CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos1Star(), nStar);

		if (xFightHero == xHero1)
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	case 2:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID2(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos2CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos2Star(), nStar);
		if (xFightHero == xHero2)
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	case 3:
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID3(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos3CnfID(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroPos3Star(), nStar);
		if (xFightHero == xHero3)
		{
			SwitchFightHero(self, xHeroID);
		}
	}
	break;

	default:
		break;
	}

	return false;
}

bool NFCHeroModule::SwitchFightHero(const NFGUID & self, const NFGUID & xHeroID)
{
	NFGUID xLastFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	if (xLastFightingHero == xHeroID)
	{
		return false;
	}

	int nPos = GetFightPos(self, xHeroID);
	if (nPos <= 0 || nPos > ECONSTDEFINE_HERO_MAXFIGHT_POS)
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

	const std::string& strCnfID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const int nHeroLevel = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Level);

	m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightHero(), xHeroID);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::FightHeroCnfID(), strCnfID);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroLevel(), nHeroLevel);

	const std::string& strSkill1 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillNormal());
	const std::string& strSkill2 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillAttack());
	const std::string& strSkill3 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillTHUMP());

	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill1(), strSkill1);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill2(), strSkill2);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill3(), strSkill3);

	return true;
}

bool NFCHeroModule::CreateHero(const NFGUID& self, const NFGUID& xHeroID)
{
	if (xHeroID.IsNull())
	{
		return false;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());
	E_SCENE_TYPE eSceneType = (E_SCENE_TYPE)m_pElementModule->GetPropertyInt(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == E_SCENE_TYPE::SCENE_TYPE_ERROR)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord.get())
	{
		return false;
	}

	int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroID);
	if (nRow < 0)
	{
		return false;
	}

	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);

    NFDataList varList;

    int nCamp = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Camp());
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

	m_pKernelModule->DestroyObject(xHeroID);
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

	SetFightHero(nPlayerID, xHero, nSet);
}

void NFCHeroModule::OnSwitchFightHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSwitchFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());

	SwitchFightHero(nPlayerID, xHero);
	
}

int NFCHeroModule::AddToFightList(const NFGUID & self, const NFGUID & xHeroID, const std::string & strCnfID)
{
	if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1()) == NFGUID())
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID1(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos1CnfID(), strCnfID);

		if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero()) == NFGUID())
		{
			m_pKernelModule->SetPropertyString(self, NFrame::Player::ConfigID(), strCnfID);

			SwitchFightHero(self, xHeroID);
		}

		return 1;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2()) == NFGUID())
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID2(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos2CnfID(), strCnfID);

		if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero()) == NFGUID())
		{
			SwitchFightHero(self, xHeroID);
		}

		return 2;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3()) == NFGUID())
	{
		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID3(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroPos3CnfID(), strCnfID);

		if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero()) == NFGUID())
		{
			SwitchFightHero(self, xHeroID);
		}

		return 4;
	}

	return -1;
}

int NFCHeroModule::AddToFightList(const NFGUID & self, const NFGUID & xHeroID, const int nPos)
{
	NFGUID xHero1 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1());
	NFGUID xHero2 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2());
	NFGUID xHero3 = m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3());
	NFGUID xFightHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHero());
	if (xHero1 == xHeroID
		|| xHero2 == xHeroID
		|| xHero3 == xHeroID)
	{
		return 0;
	}

	switch (nPos)
	{
		case 1:
		{
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID1(), xHeroID);
			if (xFightHero == xHero1)
			{
				SwitchFightHero(self, xHeroID);
			}
		}
		break;

		case 2:
		{
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID2(), xHeroID);
			if (xFightHero == xHero2)
			{
				SwitchFightHero(self, xHeroID);
			}
		}
		break;

		case 3:
		{
			m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID3(), xHeroID);
			if (xFightHero == xHero3)
			{
				SwitchFightHero(self, xHeroID);
			}
		}
		break;

	default:
		break;
	}

	return 0;
}

int NFCHeroModule::GetFightPos(const NFGUID & self, const NFGUID & xHeroID)
{
	if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1()) == xHeroID)
	{
		return 1;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2()) == xHeroID)
	{
		return 2;
	}
	else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3()) == xHeroID)
	{
		return 3;
	}

	return -1;
}