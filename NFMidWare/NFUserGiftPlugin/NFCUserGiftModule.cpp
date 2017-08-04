// -------------------------------------------------------------------------
//    @FileName			:    NFCUserGiftModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-18
//    @Module           :    NFCUserGiftModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCUserGiftModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCUserGiftModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pItemModule = pPluginManager->FindModule<NFIItemModule>();
	
	return true;
}

bool NFCUserGiftModule::AfterInit()
{
	std::vector<std::string> xGiftItemList = m_pElementModule->GetListByProperty(NFrame::Item::ThisName(), NFrame::Item::ItemType(), NFMsg::EItemType::EIT_ITEM);
	for (int i = 0; i < xGiftItemList.size(); ++i)
	{
		const std::string& strItemID = xGiftItemList[i];
		int nSubItem = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::ItemSubType());
		if (nSubItem == NFMsg::EGameItemSubType::EGIT_ITEM_PACK)
		{
			int nLevel = m_pElementModule->GetPropertyInt32(strItemID, NFrame::Item::Level());
			NF_SHARE_PTR<std::vector<std::string>> xItemList = mxGiftMap.GetElement(nLevel);
			if (!xItemList)
			{
				xItemList = NF_SHARE_PTR<std::vector<std::string>>(NF_NEW std::vector<std::string>());
				mxGiftMap.AddElement(nLevel, xItemList);
			}

			xItemList->push_back(strItemID);
		}
	}

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCUserGiftModule::OnObjectClassEvent);


	return true;
}

bool NFCUserGiftModule::CheckConfig()
{

	return true;
}

int NFCUserGiftModule::OnObjectClassEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (eClassEvent == CLASS_OBJECT_EVENT::COE_CREATE_AFTER_EFFECT)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFCUserGiftModule::OnLevelPropertyEvent);
		if (m_pKernelModule->GetPropertyInt(self, NFrame::Player::OnlineCount()) <= 0)
		{
			DoLevelAward(self, 1);

			ActiveteHero(self);
		}
	}

	return 0;
}

int NFCUserGiftModule::OnLevelPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
	int nNewLevel = newVar.GetInt32();

	DoLevelAward(self, nNewLevel);

	return 0;
}

bool NFCUserGiftModule::DoLevelAward(const NFGUID & self, const int nLevel)
{
	NF_SHARE_PTR<std::vector<std::string>> xItemList = mxGiftMap.GetElement(nLevel);
	if (xItemList)
	{
		for (int i = 0; i < xItemList->size(); ++i)
		{
			const std::string& strGiftItemID = xItemList->at(i);
			//
			NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pElementModule->GetPropertyManager(strGiftItemID);
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(NFrame::Item::AwardData());
			const NF_SHARE_PTR<NFMapEx<std::string, std::string>> xEmbeddedMap = xProperty->GetEmbeddedMap();
			if (xEmbeddedMap)
			{
				std::string strItemID;
				NF_SHARE_PTR<std::string> strItemCount = xEmbeddedMap->First(strItemID);
				for (strItemCount; strItemCount; strItemCount = xEmbeddedMap->Next(strItemID))
				{
					int nCount = lexical_cast<int>(*strItemCount);

					m_pPackModule->CreateItem(self, strItemID, nCount);
				}
			}
		}

		return true;
	}

	return false;
}

bool NFCUserGiftModule::ActiveteHero(const NFGUID & self)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, NFrame::Player::PlayerHero::ThisName());

	//configuration
	m_pItemModule->UseItem(self, "Item_HeroCard_Abaddon", self);

	return true;
}

bool NFCUserGiftModule::Shut()
{

	return true;
}

bool NFCUserGiftModule::Execute()
{
	return true;
}
