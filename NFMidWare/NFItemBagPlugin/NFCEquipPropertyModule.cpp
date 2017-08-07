// -------------------------------------------------------------------------
//    @FileName			:    NFCEquipPropertyModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEquipPropertyModule
//
// -------------------------------------------------------------------------

#include "NFCEquipPropertyModule.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif


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
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
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
		int64_t nBaseValue = 0;
		int64_t nRandonValue = 0;
		int64_t nGemValue = 0;
		int64_t nIntensifyValue = 0;
		int64_t nElementValue = 0;

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

		int64_t nAllValue = nBaseValue + nRandonValue + nGemValue + nIntensifyValue + nElementValue;

		xDataList.AddInt(nAllValue);
	}


	return true;
}

bool NFCEquipPropertyModule::CalEquipBaseProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	xDataList.Clear();

	/////////////PropertyBase/////////////////////////////////////////

	const std::string& strConfigID = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::BagEquipList::ConfigID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::Equip::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pCommPropertyValueRecord->GetColTag(i);
			int64_t nValue = m_pElementModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipRandomProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	xDataList.Clear();

	/////////////RandomBase/////////////////////////////////////////

	const std::string& strConfigID = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::BagEquipList::RandPropertyID);
	const std::string& strPropertyEffectData = m_pElementModule->GetPropertyString(strConfigID, NFrame::Equip::EffectData());
	if (!strPropertyEffectData.empty())
	{
		for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
		{
			const std::string& strColTag = pCommPropertyValueRecord->GetColTag(i);
			int64_t nValue = m_pElementModule->GetPropertyInt(strPropertyEffectData, strColTag);
			xDataList.AddInt(nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipGemProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	xDataList.Clear();

	/////////////GemBase/////////////////////////////////////////
	int nSlotCount = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::SlotCount);
	if (nSlotCount <= 0)
	{
		return false;
	}

	for (int i = 0; i < pCommPropertyValueRecord->GetCols(); ++i)
	{
		xDataList.AddInt(0);
	}

	for (int i = NFrame::Player::BagEquipList::InlayStone1; i <= NFrame::Player::BagEquipList::InlayStone10; ++i)
	{
		int nIndex = i - NFrame::Player::BagEquipList::InlayStone1;
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
			int64_t nValue = m_pElementModule->GetPropertyInt(strGemEffectData, strColTag);
			int64_t nOldValue = xDataList.Int(j);

			xDataList.SetInt(j, nOldValue + nValue);
		}
	}

	return true;
}

bool NFCEquipPropertyModule::CalEquipIntensifyProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	xDataList.Clear();

	/////////////GemBase/////////////////////////////////////////
	const int nIntensify = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::IntensifyLevel);
	if (nIntensify <= 0)
	{
		return false;
	}

	const std::string& strEquipConfig = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::ConfigID);
	const int nHeroType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::HeroTye());
	const int nItemType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::ItemSubType());
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

	xDataList.SetInt(NFrame::Player::CommValue::MAXHP, nMAXHPValue);
	xDataList.SetInt(NFrame::Player::CommValue::ATK_VALUE, nAttackValue);
	xDataList.SetInt(NFrame::Player::CommValue::DEF_VALUE, nDefValue);

	return true;
}

bool NFCEquipPropertyModule::CalEquipElementProperty(const NFGUID& self, const NFGUID& xEquipGUID, NFDataList& xDataList)
{
	NF_SHARE_PTR<NFIRecord> pBagRecord = m_pKernelModule->FindRecord(self, NFrame::Player::BagEquipList::ThisName());
	if (nullptr == pBagRecord)
	{
		return false;
	}

	NF_SHARE_PTR<NFIRecord> pCommPropertyValueRecord = m_pKernelModule->FindRecord(self, NFrame::Player::CommValue::ThisName());
	if (nullptr == pCommPropertyValueRecord)
	{
		return false;
	}

	NFDataList varFind;
	if (pBagRecord->FindObject(NFrame::Player::BagEquipList::GUID, xEquipGUID, varFind) != 1)
	{
		return false;
	}

	const int nRow = varFind.Int32(0);
	xDataList.Clear();

	/////////////element/////////////////////////////////////////
	const std::string& strEquipConfig = pBagRecord->GetString(nRow, NFrame::Player::BagEquipList::ConfigID);
	const int nHeroType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::HeroTye());
	const int nItemType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::ItemType());
	const int nItemSubType = m_pElementModule->GetPropertyInt32(strEquipConfig, NFrame::Item::ItemSubType());
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
	int nFireLevel = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::ElementLevel1_FIRE);
	int nLightLevel = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::ElementLevel2_LIGHT);
	int nWindLevel = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::ElementLevel3_Wind);
	int nIceLevel = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::ElementLevel4_ICE);
	int nPoisonLevel = pBagRecord->GetInt32(nRow, NFrame::Player::BagEquipList::ElementLevel5_POISON);

	int nWindValue = nWindLevel * nWindLevel;
	int nFireValue = nFireLevel * nFireLevel;
	int nLightValue = nLightLevel * nLightLevel;
	int nIceValue = nIceLevel * nIceLevel;
	int nPoisonValue = nPoisonLevel * nPoisonLevel;

	if (nItemSubType == NFMsg::EGameEquipSubType::EQUIPTYPE_WEAPON)
	{
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nWindValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nFireValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nLightValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nIceValue * dwCoefficientAtk);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nPoisonValue * dwCoefficientAtk);
	}
	else
	{
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nWindValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nFireValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nLightValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nIceValue * dwCoefficientDef);
		xDataList.SetInt(NFrame::Player::CommValue::ATK_FIRE, nPoisonValue * dwCoefficientDef);
	}



	return true;

	return true;
}
