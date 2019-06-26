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


#include "NFHeroPropertyModule.h"

bool NFHeroPropertyModule::Init()
{
	return true;
}

bool NFHeroPropertyModule::Shut()
{
	return true;
}

bool NFHeroPropertyModule::Execute()
{
	return true;
}

bool NFHeroPropertyModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEquipPropertyModule = pPluginManager->FindModule<NFIEquipPropertyModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pPropertyConfigModule = pPluginManager->FindModule<NFIPropertyConfigModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFHeroPropertyModule::OnPlayerClassEvent);

	return true;

}

bool NFHeroPropertyModule::BeforeShut()
{
	return true;
}

int NFHeroPropertyModule::OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	switch (eClassEvent)
	{
	case CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT:
	{
		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::PlayerHero::ThisName(), this, &NFHeroPropertyModule::OnObjectHeroRecordEvent);
		CalFightintHeroProperty(self);
	}
	break;
	case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
	{
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFHeroPropertyModule::OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
	std::ostringstream stream;
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
	if (nullptr == pHeroRecord)
	{
		return 0;
	}

	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	{
		const NFGUID xHeroID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::GUID);
		AddHeroProperty(self, xHeroID);

		if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID1()) == NFGUID())
		{
			//must setting props before we set props for fighting hero
			m_pHeroModule->SetFightHero(self, xHeroID, NFIHeroModule::EConsHero_Pos::ECONSt_HERO_POS1);
		}
		else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID2()) == NFGUID())
		{
			m_pHeroModule->SetFightHero(self, xHeroID, NFIHeroModule::EConsHero_Pos::ECONSt_HERO_POS2);
		}
		else if (m_pKernelModule->GetPropertyObject(self, NFrame::Player::HeroID3()) == NFGUID())
		{
			m_pHeroModule->SetFightHero(self, xHeroID, NFIHeroModule::EConsHero_Pos::ECONSt_HERO_POS3);
		}

		const NFGUID xFightingHeroID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
		if (xFightingHeroID == xHeroID)
		{
			CalFightintHeroProperty(self);
		}
	}
	break;
	case RECORD_EVENT_DATA::Del:
	{

	}
	break;
	case RECORD_EVENT_DATA::Update:
	{
		switch (xEventData.nCol)
		{
		case NFrame::Player::PlayerHero::Level:
		case NFrame::Player::PlayerHero::Star:
		{
			const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::GUID);
			OnHeroPropertyUpdate(self, xHeroGUID);
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}

	return 0;
}

bool NFHeroPropertyModule::OnHeroPropertyUpdate(const NFGUID & self, const NFGUID & xHeroGUID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);

	NFDataList xHeroAllValue;
	bool bRet = CalHeroAllProperty(self, xHeroGUID, xHeroAllValue);
	if (bRet)
	{
		if (pHeroPropertyRecord->IsUsed(nRow))
		{
			pHeroPropertyRecord->SetRow(nRow, xHeroAllValue);
		}
		else
		{
			pHeroPropertyRecord->AddRow(nRow, xHeroAllValue);
		}
	}

	int nStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Star);
	NFIHeroModule::EConsHero_Pos ePos = m_pHeroModule->GetFightPos(self, xHeroGUID);
	switch (ePos)
	{
	case NFIHeroModule::ECONSt_HERO_UNKNOW:
		break;
	case NFIHeroModule::ECONSt_HERO_POS1:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar1(), nStar);
	}
		break;
	case NFIHeroModule::ECONSt_HERO_POS2:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar2(), nStar);
	}
		break;
	case NFIHeroModule::ECONSt_HERO_POS3:
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HeroStar3(), nStar);
	}
		break;
	case NFIHeroModule::ECONSt_HERO_MAX:
		break;
	default:
		break;
	}

	NFGUID xFightingID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	if (xFightingID == xHeroGUID)
	{
		CalFightintHeroProperty(self);
	}

	return true;
}

bool NFHeroPropertyModule::AddHeroProperty(const NFGUID & self, const NFGUID & xHeroGUID)
{
	//we would add a row to record the hero props when the player got this hero first time
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);

	NFDataList xHeroAllValue;
	bool bRet = CalHeroAllProperty(self, xHeroGUID, xHeroAllValue);
	if (bRet)
	{
		if (pHeroPropertyRecord->IsUsed(nRow))
		{
			pHeroPropertyRecord->SetRow(nRow, xHeroAllValue);
		}
		else
		{
			pHeroPropertyRecord->AddRow(nRow, xHeroAllValue);
		}
	}

	//update hero's current HP
	int nMAXHP = pHeroPropertyRecord->GetInt(nRow, NFrame::Player::HeroValue::MAXHP);
	pHeroRecord->SetInt(nRow, NFrame::Player::PlayerHero::HP, nMAXHP);

	std::cout << pHeroRecord->ToString() << std::endl;
	return true;
}

bool NFHeroPropertyModule::CalHeroAllProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	//const int nRow = varFind.Int32(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	NFDataList xHeroPropertyValue;
	CalHeroBaseProperty(self, xHeroGUID, xHeroPropertyValue);

	NFDataList xHeroTalentValue;
	CalHeroTalentProperty(self, xHeroGUID, xHeroTalentValue);

	NFDataList xHeroEquipValue;
	CalHeroEquipProperty(self, xHeroGUID, xHeroEquipValue);

	if (xHeroPropertyValue.GetCount() != pHeroPropertyRecord->GetCols()
		|| xHeroTalentValue.GetCount() != pHeroPropertyRecord->GetCols()
		|| xHeroEquipValue.GetCount() != pHeroPropertyRecord->GetCols())
	{
		std::ostringstream os;
		os << "CRITICAL ERROR!!!!! SOME THINGS WRONG AS THE PROPERTY COUNT NOW DIFFERENT!!!";
		m_pLogModule->LogError(os);
		return false;
	}

	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		int nPropertyValue = 0;
		int nTalentValue = 0;
		int nEquipValue = 0;

		if (xHeroPropertyValue.GetCount() == pHeroPropertyRecord->GetCols())
		{
			nPropertyValue = xHeroPropertyValue.Int32(i);
		}
		if (xHeroTalentValue.GetCount() == pHeroPropertyRecord->GetCols())
		{
			nTalentValue = xHeroTalentValue.Int32(i);
		}
		if (xHeroEquipValue.GetCount() == pHeroPropertyRecord->GetCols())
		{
			nEquipValue = xHeroEquipValue.Int32(i);
		}

		int nAllValue = nPropertyValue + nTalentValue + nEquipValue;

		xDataList.AddInt(nAllValue);
	}

	return true;
}

bool NFHeroPropertyModule::CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	/////////////PropertyBase/////////////////////////////////////////
	xDataList.Clear();
	
	const int nHeroStar = pHeroRecord->GetInt(nRow, NFrame::Player::PlayerHero::Star);
	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::NPC::EffectData());

	int nLevel = nHeroStar * 4;
	if (nLevel > 99)
	{
		nLevel = 99;
	}

	if (nLevel <= 0)
	{
		nLevel = 1;
	}

	int eJobType = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::Job());
	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		const std::string& strColTag = pHeroPropertyRecord->GetColTag(i);
		int64_t nValue = m_pPropertyConfigModule->CalculateBaseValue(eJobType, nLevel, strColTag);
		xDataList.AddInt(nValue);
	}

	return true;
}

bool NFHeroPropertyModule::CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	////////////////////Talent//////////////////////////////////////////////////////
	xDataList.Clear();

	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}
	/*
	for (int i = NFrame::Player::PlayerHero::Talent1; i <= NFrame::Player::PlayerHero::Talent5; ++i)
	{
		const std::string& strTalentID = pHeroRecord->GetString(nRow, i);
		const std::string& strTalentEffectData = m_pElementModule->GetPropertyString(strTalentID, NFrame::Talent::EffectData());
		if (!strTalentEffectData.empty())
		{
			//one talent
			for (int j = 0; j < pHeroPropertyRecord->GetCols(); ++j)
			{
				const std::string& strColTag = pHeroPropertyRecord->GetColTag(j);
				int64_t nValue = m_pElementModule->GetPropertyInt(strTalentEffectData, strColTag);
				int64_t nOldValue = xDataList.Int(j);

				xDataList.SetInt(j, nOldValue + nValue);
			}
		}
	}
	*/
	return true;
}

bool NFHeroPropertyModule::CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	////////////////////Equip//////////////////////////////////////////////////////
	xDataList.Clear();

	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}
	/*
	for (int i = NFrame::Player::PlayerHero::Equip1; i <= NFrame::Player::PlayerHero::Equip6; ++i)
	{
		NFDataList EquipDataList;
		const NFGUID xEquipID = pHeroRecord->GetObject(nRow, i);
		if (!xEquipID.IsNull() && m_pEquipPropertyModule->CalEquipProperty(self, xEquipID, EquipDataList))
		{
			//one equip
			for (int j = 0; j < pHeroPropertyRecord->GetCols(); ++j)
			{
				int64_t nOldValue = xDataList.Int(j);
				int64_t nEquipValue = EquipDataList.Int(j);

				xDataList.SetInt(j, nOldValue + nEquipValue);
			}
		}
	}
	*/
	return true;
}

bool NFHeroPropertyModule::CalFightintHeroProperty(const NFGUID & self)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());
	if (nullptr == pHeroRecord)
	{
		return false;
	}
	NFGUID xHeroGUID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::FightHeroID());
	if (xHeroGUID.IsNull())
	{
		return false;
	}

	NFDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero::GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::HeroValue::ThisName());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		const std::string& strColTag = pHeroPropertyRecord->GetColTag(i);
		int64_t nValue = pHeroPropertyRecord->GetInt(nRow, i);

		m_pPropertyModule->SetPropertyValue(self, strColTag, NFIPropertyModule::NPG_FIGHTING_HERO, nValue);
	}

	return true;
}

int NFHeroPropertyModule::CalHeroDiamond(const int star, int & nStar)
{
	if (star < 0 || star >= NFIHeroModule::EConstDefine_Hero::ECONSTDEFINE_HERO_MAX_STAR)
	{
		nStar = 0;
		return 0;
	}

	int nHeroDia = 0;
	int nHeroStar = 0;

	if (star <= 5)
	{
		nHeroDia = 0;
		nHeroStar = star;
	}
	else if (star <= 10)
	{
		nHeroDia = 1;
		nHeroStar = star - 6;
	}
	else if (star <= 14)
	{
		nHeroDia = 2;
		nHeroStar = star - 11;
	}
	else if (star <= 17)
	{
		nHeroDia = 3;
		nHeroStar = star - 15;
	}
	else if (star <= 19)
	{
		nHeroDia = 4;
		nHeroStar = star - 18;
	}
	else if (star <= 20)
	{
		nHeroDia = 5;
		nHeroStar = 0;
	}

	nStar = nHeroStar;

	return nHeroDia;
}
