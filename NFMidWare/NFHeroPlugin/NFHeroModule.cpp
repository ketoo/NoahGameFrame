/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "NFHeroModule.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFHeroModule::Init()
{

	return true;
}

bool NFHeroModule::Shut()
{
	return true;
}

bool NFHeroModule::Execute()
{
	return true;
}

bool NFHeroModule::AfterInit()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pHeroPropertyModule = pPluginManager->FindModule<NFIHeroPropertyModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SET_FIGHT_HERO, this, &NFHeroModule::OnSetFightHeroMsg);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SWITCH_FIGHT_HERO, this, &NFHeroModule::OnSwitchFightHeroMsg);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_RELIVE_HERO, this, &NFHeroModule::OnReliveHeroMsg);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFHeroModule::OnPlayerClassEvent);

	m_pSceneModule->AddBeforeEnterSceneGroupCallBack(this, &NFHeroModule::BeforeEnterSceneGroupEvent);
	m_pSceneModule->AddAfterEnterSceneGroupCallBack(this, &NFHeroModule::AfterEnterSceneGroupEvent);
	m_pSceneModule->AddBeforeLeaveSceneGroupCallBack(this, &NFHeroModule::BeforeLeaveSceneGroupEvent);
	m_pSceneModule->AddAfterLeaveSceneGroupCallBack(this, &NFHeroModule::AfterLeaveSceneGroupEvent);

	return true;
}

bool NFHeroModule::BeforeShut()
{
	return true;
}

NFGUID NFHeroModule::AddHero(const NFGUID& self, const std::string& strID)
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
		nNowStar++;

		if (nNowStar > ECONSTDEFINE_HERO_MAX_STAR)
		{
			nNowStar = ECONSTDEFINE_HERO_MAX_STAR;
		}

		pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::Star, nNowStar);

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

	std::cout << xRowData->ToString() << std::endl;

	nRow = pHeroRecord->AddRow(-1, *xRowData);
	if (nRow < 0)
	{
		return NFGUID();
	}

	std::cout << pHeroRecord->ToString() << std::endl;
	return xHeroID;
}

bool NFHeroModule::AddHeroExp(const NFGUID & self, const int64_t nExp)
{
	NFGUID xID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	return AddHeroExp(self, xID, nExp);
}

bool NFHeroModule::AddHeroExp(const NFGUID& self, const NFGUID& xHeroID, const int64_t nExp)
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

bool NFHeroModule::SetFightHero(const NFGUID& self, const NFGUID& xHeroID, const EConsHero_Pos nPos)
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

	int hp = pHeroRecord->GetInt32(nRow, NFrame::Player::PlayerHero::HP);
	if (hp <= 0)
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

	NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	NFGUID xFightHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());

	switch (nPos)
	{
	case EConsHero_Pos::ECONSt_HERO_POS1:
	{
		const std::string& strEffectData = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::EffectData());
		const int nMAXHP = m_pElementModule->GetPropertyInt(strEffectData, NFrame::NPC::MAXHP());

		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID1(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroCnfID1(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar1(), nStar);
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
		const std::string& strEffectData = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::EffectData());
		const int nMAXHP = m_pElementModule->GetPropertyInt(strEffectData, NFrame::NPC::MAXHP());

		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID2(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroCnfID2(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar2(), nStar);
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
		const std::string& strEffectData = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::EffectData());
		const int nMAXHP = m_pElementModule->GetPropertyInt(strEffectData, NFrame::NPC::MAXHP());

		m_pKernelModule->SetPropertyObject(self, NFrame::Player::HeroID3(), xHeroID);
		m_pKernelModule->SetPropertyString(self, NFrame::Player::HeroCnfID3(), strCnfID);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar3(), nStar);
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

	return false;
}

bool NFHeroModule::SwitchFightHero(const NFGUID & self, const NFGUID & xHeroID)
{
	NFGUID xLastFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	if (xLastFightingHero == xHeroID)
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
	NFGUID xFightHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	if (xFightHero == xHeroID)
	{
		return 0;
	}

	int hp = pHeroRecord->GetInt32(nRow, NFrame::Player::PlayerHero::HP);
	if (hp <= 0)
	{
		return false;
	}


	//depende the scene type
	//if now the player fighting in a clone scene, pos must be restricted between ECONSt_HERO_UNKNOW and ECONSt_HERO_MAX
	//if now the player fighting in a town or suburb scene, we allow player switch any hero who still alive to fight again
	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());

	int nHP = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::HP);
	const std::string& strCnfID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const int nHeroLevel = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Level);
	
	m_pKernelModule->SetPropertyObject(self, NFrame::Player::FightHeroID(), xHeroID);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::FightHeroCnfID(), strCnfID);
	m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHP);

	const std::string& strSkill1 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillNormal());
	const std::string& strSkill2 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillAttack());
	const std::string& strSkill3 = m_pElementModule->GetPropertyString(strCnfID, NFrame::NPC::SkillTHUMP());

	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill1(), strSkill1);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill2(), strSkill2);
	m_pKernelModule->SetPropertyString(self, NFrame::Player::Skill3(), strSkill3);

	m_pHeroPropertyModule->CalFightintHeroProperty(self);

	return true;
}

bool NFHeroModule::ReliveHero(const NFGUID & self, const NFGUID & xHeroID, const int diamond)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}
	NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroValueRecord)
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

	const int heroHP = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::HP);
	if (heroHP > 0)
	{
		return false;
	}

	const int nMaxHP = pHeroValueRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXHP);
	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::HP, nMaxHP);

	const NFGUID& fighter = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	if (fighter == xHeroID)
	{
		m_pPropertyModule->FullHPMP(self);
	}

	return true;
}

bool NFHeroModule::ReliveAllHero(const NFGUID & self)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroValueRecord)
	{
		return false;
	}

	for (int i = 0; i < pHeroRecord->GetRows(); ++i)
	{
		const int nMaxHP = pHeroValueRecord->GetInt(i, NFrame::Player::HeroValue::MAXHP);
		pHeroRecord->SetInt(i, NFrame::Player::PlayerHero::HP, nMaxHP);
	}

	m_pPropertyModule->FullHPMP(self);

	return true;
}

NFGUID NFHeroModule::GetHeroGUID(const NFGUID& self, const std::string& strID)
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

void NFHeroModule::OnSetFightHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSetFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());
	const int nSet = xMsg.set();

	SetFightHero(nPlayerID, xHero, (NFIHeroModule::EConsHero_Pos)nSet);
}

void NFHeroModule::OnSwitchFightHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqSwitchFightHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.heroid());

	SwitchFightHero(nPlayerID, xHero);
	
}

void NFHeroModule::OnReliveHeroMsg(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckReliveHero);

	const NFGUID xHero = NFINetModule::PBToNF(xMsg.hero_id());
	const int diamond = xMsg.diamond();

	ReliveHero(nPlayerID, xHero, diamond);
}

NFIHeroModule::EConsHero_Pos NFHeroModule::GetFightPos(const NFGUID & self, const NFGUID & xHeroID)
{
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
	
	return EConsHero_Pos::ECONSt_HERO_UNKNOW;
}

int NFHeroModule::OnPlayerClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		NFGUID xLastFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
		NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
		NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
		
		int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xLastFightingHero);
		if (nRow < 0)
		{
			return false;
		}

		int nMAXHP = pHeroValueRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXHP);
		int nHP = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::HP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXHP(), nMAXHP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHP);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::NPC::HP(), this, &NFHeroModule::OnPlayerHPEvent);
		// we need add a schedule to relive the heroes that dead
	}
	
	return 0;
}

int NFHeroModule::OnPlayerHeroHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	// we need add a schedule to relive the heroes that dead

	return 0;
}

int NFHeroModule::OnPlayerHPEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	NFGUID xFightingHero = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xFightingHero);
	if (nRow < 0)
	{
		return 0;
	}

	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::HP, newVar.GetInt());

	return 0;
}

int NFHeroModule::BeforeEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFHeroModule::AfterEnterSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	//full hp for all fight heroes when entered a clone scene
	NFMsg::ESceneType eSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt32(std::to_string(nSceneID), NFrame::Scene::Type());
	if (eSceneType == NFMsg::ESceneType::SCENE_SINGLE_CLONE
		|| eSceneType == NFMsg::ESceneType::SCENE_MULTI_CLONE
		|| eSceneType == NFMsg::ESceneType::SCENE_HOME)
	{

		NFGUID xFightingHeroID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
		NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
		int nRow = pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xFightingHeroID);
		if (nRow < 0)
		{
			return 0;
		}

		NF_SHARE_PTR<NFIRecord> pHeroValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());

		int nHeroMAXHP = pHeroValueRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXHP);
		int nHeroMAXMP = pHeroValueRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXMP);

		pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::HP, nHeroMAXHP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXHP(), nHeroMAXHP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nHeroMAXHP);

		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MAXMP(), nHeroMAXMP);
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::MP(), nHeroMAXMP);

	}

	return 0;
}

int NFHeroModule::BeforeLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}

int NFHeroModule::AfterLeaveSceneGroupEvent(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFDataList & argList)
{
	return 0;
}


