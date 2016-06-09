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
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pEquipPropertyModule = pPluginManager->FindModule<NFIEquipPropertyModule>("NFCEquipPropertyModule");
	
	assert(NULL != m_pKernelModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pGameServerNet_ServerModule);
	assert(NULL != m_pUUIDModule);
	assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pEquipPropertyModule);

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCHeroPropertyModule::OnPlayerClassEvent);
	m_pKernelModule->AddClassCallBack(NFrame::NPC::ThisName(), this, &NFCHeroPropertyModule::OnNPCClassEvent);
	return true;

}

bool NFCHeroPropertyModule::BeforeShut()
{
	return true;
}

int NFCHeroPropertyModule::OnPlayerClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	switch (eClassEvent)
	{
	case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
	{
		m_pKernelModule->AddRecordCallBack(self, NFrame::Player::R_PlayerHero(), this, &NFCHeroPropertyModule::OnObjectHeroRecordEvent);
	}
	break;
	default:
		break;
	}

	return 0;
}


int NFCHeroPropertyModule::OnNPCClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	switch (eClassEvent)
	{
	case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
	{
	}
	break;
	default:
		break;
	}

	return 0;
}

int NFCHeroPropertyModule::OnObjectHeroRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
	std::ostringstream stream;
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
	if (nullptr == pHeroRecord)
	{
		return 0;
	}

	switch (xEventData.nOpType)
	{
	case NFIRecord::RecordOptype::Add:
	{
		const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_GUID);
		OnHeroPropertyUpdate(self, xHeroGUID);
	}
	break;
	case NFIRecord::RecordOptype::Del:
	{

	}
	break;
	case NFIRecord::RecordOptype::UpData:
	{
		switch (xEventData.nCol)
		{
		case NFrame::Player::PlayerHero::PlayerHero_Level:
		case NFrame::Player::PlayerHero::PlayerHero_Star:
		case NFrame::Player::PlayerHero::PlayerHero_Equip1:
		case NFrame::Player::PlayerHero::PlayerHero_Equip2:
		case NFrame::Player::PlayerHero::PlayerHero_Equip3:
		case NFrame::Player::PlayerHero::PlayerHero_Equip4:
		case NFrame::Player::PlayerHero::PlayerHero_Equip5:
		case NFrame::Player::PlayerHero::PlayerHero_Equip6:
		case NFrame::Player::PlayerHero::PlayerHero_Talent1:
		case NFrame::Player::PlayerHero::PlayerHero_Talent2:
		case NFrame::Player::PlayerHero::PlayerHero_Talent3:
		case NFrame::Player::PlayerHero::PlayerHero_Talent4:
		case NFrame::Player::PlayerHero::PlayerHero_Talent5:
		{
			const NFGUID& xHeroGUID = pHeroRecord->GetObject(xEventData.nRow, NFrame::Player::PlayerHero::PlayerHero_GUID);
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

int NFCHeroPropertyModule::OnHeroPropertyUpdate(const NFGUID & self, const NFGUID & xHeroGUID)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFCDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	NFCDataList xHeroPropertyValue;
	CalHeroBaseProperty(self, xHeroGUID, xHeroPropertyValue);

	NFCDataList xHeroTalentValue;
	CalHeroTalentProperty(self, xHeroGUID, xHeroTalentValue);

	NFCDataList xHeroEqupValue;
	CalHeroEquipProperty(self, xHeroGUID, xHeroEqupValue);

	if (xHeroPropertyValue.GetCount() != pHeroPropertyRecord->GetRows()
		|| xHeroTalentValue.GetCount() != pHeroPropertyRecord->GetRows()
		|| xHeroEqupValue.GetCount() != pHeroPropertyRecord->GetRows())
	{
		return false;
	}

	NFCDataList xHeroAllValue;
	for (int i = 0; i < pHeroPropertyRecord->GetRows(); ++i)
	{
		const int nPropertyValue = xHeroPropertyValue.Int(i);
		const int nTalentValue = xHeroTalentValue.Int(i);
		const int nEquipValue = xHeroEqupValue.Int(i);

		int nAllValue = nPropertyValue + nTalentValue + nEquipValue;

		xHeroAllValue.AddInt(nAllValue);
	}

	pHeroPropertyRecord->AddRow(nRow, xHeroAllValue);

	return 0;
}

bool NFCHeroPropertyModule::CalHeroBaseProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFCDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	/////////////PropertyBase/////////////////////////////////////////

	const std::string& strConfigID = pHeroRecord->GetString(nRow, NFrame::Player::PlayerHero::PlayerHero_ConfigID);
	const std::string& strPropertyEffectData = m_pElementInfoModule->GetPropertyString(strConfigID, NFrame::NPC::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pHeroPropertyRecord->GetColTag(i);
			int nValue = m_pElementInfoModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCHeroPropertyModule::CalHeroTalentProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFCDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	////////////////////Talent//////////////////////////////////////////////////////
	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	for (int i = NFrame::Player::PlayerHero_Talent1; i <= NFrame::Player::PlayerHero_Talent5; ++i)
	{
		const std::string& strTalentID = pHeroRecord->GetString(nRow, i);
		const std::string& strTalentEffectData = m_pElementInfoModule->GetPropertyString(strTalentID, NFrame::Talent::EffectData());
		if (!strTalentEffectData.empty())
		{
			//one talent
			for (int j = 0; j < pHeroPropertyRecord->GetCols(); ++j)
			{
				const std::string& strColTag = pHeroPropertyRecord->GetColTag(j);
				int nValue = m_pElementInfoModule->GetPropertyInt(strTalentEffectData, strColTag);
				int nOldValue = xDataList.Int(j);

				xDataList.SetInt(j, nOldValue + nValue);
			}
		}
	}

	return true;
}

bool NFCHeroPropertyModule::CalHeroEquipProperty(const NFGUID& self, const NFGUID& xHeroGUID, NFIDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pHeroRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_PlayerHero());
	if (nullptr == pHeroRecord)
	{
		return false;
	}

	NFCDataList varFind;
	if (pHeroRecord->FindObject(NFrame::Player::PlayerHero_GUID, xHeroGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);

	NF_SHARE_PTR<NFIRecord> pHeroPropertyRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_HeroPropertyValue());
	if (nullptr == pHeroPropertyRecord)
	{
		return false;
	}

	////////////////////Equip//////////////////////////////////////////////////////
	for (int i = 0; i < pHeroPropertyRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	for (int i = NFrame::Player::PlayerHero_Equip1; i <= NFrame::Player::PlayerHero_Equip6; ++i)
	{
		NFCDataList EquipDataList;
		const NFGUID xEquipID = pHeroRecord->GetObject(nRow, i);
		if (!xEquipID.IsNull() && m_pEquipPropertyModule->CalEquipProperty(self, xEquipID, EquipDataList))
		{
			//one equip
			for (int j = 0; j < pHeroPropertyRecord->GetCols(); ++j)
			{
				int nOldValue = xDataList.Int(j);
				int nEquipValue = EquipDataList.Int(j);

				xDataList.SetInt(j, nOldValue + nEquipValue);
			}
		}
	}

	return true;
}
