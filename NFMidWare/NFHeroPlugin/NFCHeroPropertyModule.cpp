// -------------------------------------------------------------------------
//    @FileName			:    NFCHeroPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-02-16
//    @Module           :    NFCHeroPropertyModule
//
// -------------------------------------------------------------------------

#include "NFCHeroPropertyModule.h"

bool NFCHeroPropertyModule::Init()
{
	return true;
}

bool NFCHeroPropertyModule::Shut()
{
	return true;
}

bool NFCHeroPropertyModule::Execute()
{
	return true;
}

bool NFCHeroPropertyModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pEquipPropertyModule = pPluginManager->FindModule<NFIEquipPropertyModule>();

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCHeroPropertyModule::OnPlayerClassEvent);
	return true;

}

bool NFCHeroPropertyModule::BeforeShut()
{
	return true;
}

int NFCHeroPropertyModule::OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	switch (eClassEvent)
	{
	case CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT:
	{
		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::PlayerHero::ThisName(), this, &NFCHeroPropertyModule::OnObjectHeroRecordEvent);
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFCHeroPropertyModule::OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
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
		const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::GUID);
		OnHeroPropertyUpdate(self, xHeroGUID);
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
		case NFrame::Player::PlayerHero::Equip1:
		case NFrame::Player::PlayerHero::Equip2:
		case NFrame::Player::PlayerHero::Equip3:
		case NFrame::Player::PlayerHero::Equip4:
		case NFrame::Player::PlayerHero::Equip5:
		case NFrame::Player::PlayerHero::Equip6:
		case NFrame::Player::PlayerHero::Talent1:
		case NFrame::Player::PlayerHero::Talent2:
		case NFrame::Player::PlayerHero::Talent3:
		case NFrame::Player::PlayerHero::Talent4:
		case NFrame::Player::PlayerHero::Talent5:
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

bool NFCHeroPropertyModule::OnHeroPropertyUpdate(const NFGUID & self, const NFGUID & xHeroGUID)
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

	return true;
}

bool NFCHeroPropertyModule::CalHeroAllProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
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

bool NFCHeroPropertyModule::CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
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

	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::ConfigID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::NPC::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pHeroPropertyRecord->GetColTag(i);
			int64_t nValue = m_pElementModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCHeroPropertyModule::CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
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

	return true;
}

bool NFCHeroPropertyModule::CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFDataList& xDataList)
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

	return true;
}