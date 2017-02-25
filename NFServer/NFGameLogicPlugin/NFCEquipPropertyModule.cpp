// -------------------------------------------------------------------------
//    @FileName			:    NFCEquipPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEquipPropertyModule
//
// -------------------------------------------------------------------------

#include "NFCEquipPropertyModule.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

bool NFCEquipPropertyModule::Init()
{
	return true;
}

bool NFCEquipPropertyModule::Shut()
{
	return true;
}

bool NFCEquipPropertyModule::Execute()
{
	return true;
}

bool NFCEquipPropertyModule::AfterInit()
{
	return true;
}

bool NFCEquipPropertyModule::CalEquipProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	NFDataList xEquipBaseValue;
	CalEquipBaseProperty(self, xEquipGUID, xEquipBaseValue);

	NFDataList xEquipRandonValue;
	CalEquipRandomProperty(self, xEquipGUID, xEquipRandonValue);

	NFDataList xEquipGemValue;
	CalEquipGemProperty(self, xEquipGUID, xEquipGemValue);

	NFDataList xEquipIntensifyValue;
	CalEquipIntensifyProperty(self, xEquipGUID, xEquipIntensifyValue);

	NFDataList xEquipElementValue;
	CalEquipElementProperty(self, xEquipGUID, xEquipElementValue);

	for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
	{
		int nBaseValue = 0;
		int nRandonValue = 0;
		int nGemValue = 0;
		int nIntensifyValue = 0;
		int nElementValue = 0;

		if (xEquipBaseValue.GetCount() == pCommPropertyValueRecord->GetCols())
		{
			nBaseValue = xEquipBaseValue.Int(i);
		}
		if (xEquipRandonValue.GetCount() == pCommPropertyValueRecord->GetCols())
		{
			nRandonValue = xEquipRandonValue.Int(i);
		}
		if (xEquipGemValue.GetCount() == pCommPropertyValueRecord->GetCols())
		{
			nGemValue = xEquipGemValue.Int(i);
		}
		if (xEquipIntensifyValue.GetCount() == pCommPropertyValueRecord->GetCols())
		{
			nIntensifyValue = xEquipIntensifyValue.Int(i);
		}
		if (xEquipElementValue.GetCount() == pCommPropertyValueRecord->GetCols())
		{
			nElementValue = xEquipElementValue.Int(i);
		}

		int nAllValue = nBaseValue + nRandonValue + nGemValue + nIntensifyValue + nElementValue;

		xDataList.AddInt(nAllValue);
	}


	return true;
}

bool NFCEquipPropertyModule::CalEquipBaseProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	/////////////PropertyBase/////////////////////////////////////////

	const std::string& strConfigID = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::BagEquipList_ConfigID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::Equip::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pCommPropertyValueRecord->GetColTag(i);
			int nValue = m_pElementModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipRandomProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	/////////////RandomBase/////////////////////////////////////////

	const std::string& strConfigID = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::BagEquipList_RandPropertyID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::Equip::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pCommPropertyValueRecord->GetColTag(i);
			int nValue = m_pElementModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipGemProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	/////////////GemBase/////////////////////////////////////////
	int nSlotCount = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_SlotCount);
	if (nSlotCount <= 0)
	{
		return false;
	}

	for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	for (int i = NFrame::Player::BagEquipList_InlayStone1; i <= NFrame::Player::BagEquipList_InlayStone10; ++i)
	{
		int nIndex = i - NFrame::Player::BagEquipList_InlayStone1;
		if (nIndex > nSlotCount)
		{
			break;
		}

		const std::string& strGemID = pBagRecord->GetString(nRow, i);
		if (strGemID.empty())
		{
			continue;
		}

		const std::string& strGemEffectData = m_pElementModule->GetPropertyString(strGemID, NFrame::Item::EffectData());
		if (strGemEffectData.empty())
		{
			continue;
		}

		//one gem
		for (int j = 0; j < pCommPropertyValueRecord->GetCols(); ++j)
		{
			const std::string& strColTag = pCommPropertyValueRecord->GetColTag(j);
			int nValue = m_pElementModule->GetPropertyInt(strGemEffectData, strColTag);
			int nOldValue = xDataList.Int(j);

			xDataList.SetInt(j, nOldValue + nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipIntensifyProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	/////////////GemBase/////////////////////////////////////////
	const int nIntensify = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_IntensifyLevel);
	if (nIntensify <= 0)
	{
		return false;
	}

	const std::string& strEquipConfig = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList_ConfigID);
	const int nHeroType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::HeroTye());
	const int nItemType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::ItemSubType());
	if (nItemType != NFMsg::EItemType::EIT_EQUIP)
	{
		return false;
	}

	for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	//conditional the item type to define what property to give
	double dwCoefficientAtk = 2.0;
	double dwCoefficientDef = 1.0;
	int nMAXHPValue = nIntensify * nIntensify;
	int nAttackValue = nIntensify * nIntensify;
	int nDefValue = nIntensify * nIntensify;

	if (nItemSubType == NFMsg::EGameEquipSubType::EQUIPTYPE_WEAPON)
	{
		nAttackValue *= dwCoefficientAtk;
	}
	else
	{
		nDefValue *= dwCoefficientDef;
	}

	xDataList.SetInt(NFrame::Player::CommPropertyValue_MAXHP, nMAXHPValue);
	xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_VALUE, nAttackValue);
	xDataList.SetInt(NFrame::Player::CommPropertyValue_DEF_VALUE, nDefValue);

	return true;
}

bool NFCEquipPropertyModule::CalEquipElementProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_BagEquipList());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::R_CommPropertyValue());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList_GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int(0);
	xDataList.Clear();

	/////////////element/////////////////////////////////////////
	const std::string& strEquipConfig = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList_ConfigID);
	const int nHeroType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::HeroTye());
	const int nItemType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt(strEquipConfig, NFrame::Item::ItemSubType());
	if (nItemType != NFMsg::EItemType::EIT_EQUIP)
	{
		return false;
	}

	for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	//conditional the item type to define what property to give
	double dwCoefficientAtk = 2.0;
	double dwCoefficientDef = 1.0;
	int nFireLevel = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_ElementLevel1_FIRE);
	int nLightLevel = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_ElementLevel2_LIGHT);
	int nWindLevel = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_ElementLevel3_Wind);
	int nIceLevel = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_ElementLevel4_ICE);
	int nPoisonLevel = pBagRecord->GetInt(nRow, NFrame::Player::BagEquipList_ElementLevel5_POISON);

	int nWindValue = nWindLevel * nWindLevel;
	int nFireValue = nFireLevel * nFireLevel;
	int nLightValue = nLightLevel * nLightLevel;
	int nIceValue = nIceLevel * nIceLevel;
	int nPoisonValue = nPoisonLevel * nPoisonLevel;

	if (nItemSubType == NFMsg::EGameEquipSubType::EQUIPTYPE_WEAPON)
	{
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nWindValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nFireValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nLightValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nIceValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nPoisonValue * dwCoefficientAtk);
	}
	else
	{
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nWindValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nFireValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nLightValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nIceValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommPropertyValue_ATK_FIRE, nPoisonValue * dwCoefficientDef);
	}



	return true;

	return true;
}
